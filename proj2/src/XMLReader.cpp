#include "XMLReader.h"
#include "XMLWriter.h"
#include "DataSource.h"
#include "DataSink.h"
#include <iostream>
#include <sstream>
#include <stack>
#include <expat.h>

// Implementation for XML Reader
struct CXMLReader::SImplementation {
    std::shared_ptr<CDataSource> Source; // Data source for reading XML
    bool EndOfFile; // Flag to indicate end of file
    XML_Parser Parser; // Expat XML parser instance
    SXMLEntity CurrentEntity; // Stores the current parsed entity
    bool EntityReady; // Indicates if an entity is ready to be processed
    bool SkipCData; // Determines whether to skip character data (CDATA)

    // Constructor initializes the parser and sets handlers
    SImplementation(std::shared_ptr<CDataSource> src)
        : Source(src), EndOfFile(false), EntityReady(false), SkipCData(false) {
        Parser = XML_ParserCreate(nullptr);
        XML_SetUserData(Parser, this);
        XML_SetElementHandler(Parser, StartElementHandler, EndElementHandler);
        XML_SetCharacterDataHandler(Parser, CharDataHandler);
    }
    
    // Destructor frees the parser memory
    ~SImplementation() {
        XML_ParserFree(Parser);
    }
    
    bool ReadEntity(SXMLEntity &entity, bool skipcdata);
    static void StartElementHandler(void *userData, const char *name, const char **atts);
    static void EndElementHandler(void *userData, const char *name);
    static void CharDataHandler(void *userData, const char *data, int len);
};

// Constructor for XML Reader
CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src) : implementation(std::make_unique<SImplementation>(src)) {}

// Destructor for XML Reader
CXMLReader::~CXMLReader() {}

// Check if the end of file has been reached
bool CXMLReader::End() const { return implementation->EndOfFile; }

// Read an XML entity from the data source
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata) {
    return implementation->ReadEntity(entity, skipcdata);
}

// Implementation of XML entity reading logic
bool CXMLReader::SImplementation::ReadEntity(SXMLEntity &entity, bool skipcdata) {
    SkipCData = skipcdata;
    std::vector<char> line;
    if (!Source->Read(line, 1024)) { // Read a line from the data source
        EndOfFile = true;
        return false;
    }
    
    if (XML_Parse(Parser, line.data(), line.size(), XML_FALSE) == XML_STATUS_ERROR) { // Parse XML line
        EndOfFile = true;
        return false;
    }
    
    if (EntityReady) { // If an entity is ready, return it
        entity = CurrentEntity;
        EntityReady = false;
        return true;
    }
    
    return false;
}

// Handler for XML start elements
void CXMLReader::SImplementation::StartElementHandler(void *userData, const char *name, const char **atts) {
    SImplementation *impl = static_cast<SImplementation*>(userData);
    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::StartElement;
    entity.DNameData = name;
    
    // Log the element name and the number of attributes
    std::cout << "StartElement: " << name << std::endl;
    std::cout << "No of Attributes: ";
    
    int numAttrs = 0;
    // Process attributes
    while (atts[numAttrs]) {
        std::string attrName = atts[numAttrs];
        std::string attrValue = atts[numAttrs + 1];
        entity.DAttributes.emplace_back(attrName, attrValue);
        numAttrs += 2;
    }
    
    std::cout << numAttrs / 2 << std::endl; // Log the number of attributes
    
    impl->CurrentEntity = entity;
    impl->EntityReady = true;
}

// Handler for XML end elements
void CXMLReader::SImplementation::EndElementHandler(void *userData, const char *name) {
    SImplementation *impl = static_cast<SImplementation*>(userData);
    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::EndElement;
    entity.DNameData = name;
    
    // Log the end element
    std::cout << "EndElement: " << name << std::endl;
    std::cout << "No of Attributes: 0" << std::endl; // No attributes in EndElement
    
    impl->CurrentEntity = entity;
    impl->EntityReady = true;
}

// Handler for character data (CDATA)
void CXMLReader::SImplementation::CharDataHandler(void *userData, const char *data, int len) {
    SImplementation *impl = static_cast<SImplementation*>(userData);
    if (impl->SkipCData) return;
    
    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::CharData;
    entity.DNameData.assign(data, len);
    
    // Log the character data
    std::cout << "CharData: " << entity.DNameData << std::endl;
    
    impl->CurrentEntity = entity;
    impl->EntityReady = true;
}

// CXMLReader::CXMLReader(std::shared_ptr< CDataSource > /*src*/) {
//     // You can initialize your internal state here if needed, using 'src'
// }

// CXMLReader::~CXMLReader() {
// }

// bool CXMLReader::End() const {
//     // Implement the logic to determine if the XML reading has finished
//     return false;
// }

// bool CXMLReader::ReadEntity(SXMLEntity &/*entity*/, bool /*skipcdata*/) {
//     // Implement logic for reading an entity and potentially skipping cdata if needed
//     return false;
// }
