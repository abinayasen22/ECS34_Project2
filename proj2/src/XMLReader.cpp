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
    
    for (int i = 0; atts[i]; i += 2) { // Process attributes
        entity.DAttributes.emplace_back(atts[i], atts[i + 1]);
    }
    
    impl->CurrentEntity = entity;
    impl->EntityReady = true;
}

// Handler for XML end elements
void CXMLReader::SImplementation::EndElementHandler(void *userData, const char *name) {
    SImplementation *impl = static_cast<SImplementation*>(userData);
    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::EndElement;
    entity.DNameData = name;
    
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
