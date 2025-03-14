#include "XMLReader.h"
#include <expat.h>
#include <iostream>
#include <queue>

struct CXMLReader::SImplementation { 
    std::shared_ptr<CDataSource> source;     // shared ptr to data source
    XML_Parser parser;  //expat parser
    bool parseEnd = false;  // at end of parsing
    std::queue<SXMLEntity> entityQueue; // queue for XML entities

    SImplementation(std::shared_ptr<CDataSource> src) : source(src) {
        parser = XML_ParserCreate(nullptr);
        XML_SetUserData(parser, this);  // set user data to parser instance
        XML_SetElementHandler(parser, StartEHand, EndEHand);    // set start and end handlers
        XML_SetCharacterDataHandler(parser, CharDataHand);  // set char data handler
    }

    ~SImplementation() {
        XML_ParserFree(parser);
    }
    //convert escaped entities back to their og chars
    static std::string UnescapeXML(const std::string &str) {
        std::string result;
        result.reserve(str.size());   // reserves memory for ressult
        for (size_t i = 0; i < str.size(); ++i) {
            if (str[i] == '&') {
                if (str.substr(i, 4) == "&lt;") {
                    result += '<';  
                    i += 3;
                } else if (str.substr(i, 4) == "&gt;") {
                    result += '>';
                    i += 3;
                } else if (str.substr(i, 5) == "&amp;") {
                    result += '&';
                    i += 4;
                } else if (str.substr(i, 6) == "&quot;") {
                    result += '"';
                    i += 5;
                } else if (str.substr(i, 6) == "&apos;") {
                    result += '\'';
                    i += 5;
                } else {
                    result += str[i];
                }
            } else {
                result += str[i];
            }
        }
        return result;
    }

    static void StartEHand(void *userData, const char *name, const char **attrs) {
        SImplementation *implementation = static_cast<SImplementation*>(userData);
        SXMLEntity entity;  // entity for parsed data
        entity.DType = SXMLEntity::EType::StartElement; // set entity type to start element
        entity.DNameData = name;

        while (*attrs) {    //add atributes to entity
            std::string attrName = *attrs++;
            std::string attrValue = *attrs++;
            entity.SetAttribute(attrName, attrValue);
        }

        implementation->entityQueue.push(entity);
    }

    static void EndEHand(void *userData, const char *name) {
        SImplementation *implementation = static_cast<SImplementation*>(userData);
        SXMLEntity entity;
        entity.DType = SXMLEntity::EType::EndElement;
        entity.DNameData = name;

        implementation->entityQueue.push(entity);
    }

    static void CharDataHand(void *userData, const char *data, int len) {
        SImplementation *implementation = static_cast<SImplementation*>(userData);
        std::string charData(data, len);    // character data to string
        if (!charData.empty() && charData.find_first_not_of(" \t\n\r") != std::string::npos) {  // ignore empty data
            SXMLEntity entity;
            entity.DType = SXMLEntity::EType::CharData;
            entity.DNameData = charData;    // stores char data
            implementation->entityQueue.push(entity); 
        }
    }
};

// Constructor for CXMLReader class
CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src) 
    : implementation(std::make_unique<SImplementation>(src)) {}

// Destructor
CXMLReader::~CXMLReader() = default;

// End of parsing check
bool CXMLReader::End() const {
    //true if parsing finished and queue empty
    return implementation->parseEnd && implementation->entityQueue.empty();
}

// Reads xml entities
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata) {
    // loop while the entity queue is empty and parsing is not finished
    while (implementation->entityQueue.empty() && !implementation->parseEnd) {
        std::vector<char> buf;
        bool wasRead = implementation->source->Read(buf, 1024);  // read from source by chunks

        if (wasRead) {
            int len = buf.size();
            // parsing
            if (XML_Parse(implementation->parser, buf.data(), len, XML_FALSE) == XML_STATUS_ERROR) {
                return false;   //parse failed
            }
        }

        // Check if the source has reached the end
        if (implementation->source->End()) {
            implementation->parseEnd = true;    // parse has reached end
            XML_Parse(implementation->parser, "", 0, XML_TRUE); // signal end of parsing
            break;
        }
    }

    // If entity queue is empty and parseEnd is true, parsing done
    if (implementation->entityQueue.empty() && implementation->parseEnd) {
        return false;
    }

    // Process entities from queue
    while (!implementation->entityQueue.empty()) {
        entity = implementation->entityQueue.front();  
        implementation->entityQueue.pop();      // remove front entity from queue

        if (skipcdata && entity.DType == SXMLEntity::EType::CharData) {
            return ReadEntity(entity, skipcdata);   // Recurse if character data should be skipped
        }
        return true;  // Entity processed
    }
    return false;   // no entity processed
}