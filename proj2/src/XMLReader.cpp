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
    std::shared_ptr<CDataSource> Source;
    XML_Parser Parser;
    std::deque<SXMLEntity> EntityQueue; // Queue to store parsed entities
    bool EndOfFile;
    bool SkipCData;

    SImplementation(std::shared_ptr<CDataSource> src)
        : Source(src), EndOfFile(false), SkipCData(false) {
        Parser = XML_ParserCreate(nullptr);
        XML_SetUserData(Parser, this);
        XML_SetElementHandler(Parser, StartElementHandler, EndElementHandler);
        XML_SetCharacterDataHandler(Parser, CharDataHandler);
    }

    ~SImplementation() {
        XML_ParserFree(Parser);
    }

    bool ReadEntity(SXMLEntity &entity, bool skipcdata);
    static void StartElementHandler(void *userData, const char *name, const char **atts);
    static void EndElementHandler(void *userData, const char *name);
    static void CharDataHandler(void *userData, const char *data, int len);
};

// Constructor for XML Reader
CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src) : DImplementation(std::make_unique<SImplementation>(src)) {}

// Destructor for XML Reader
CXMLReader::~CXMLReader() {}

// Check if the end of file has been reached
bool CXMLReader::End() const { return DImplementation->EndOfFile; }

// Read an XML entity from the data source
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata) {
    return DImplementation->ReadEntity(entity, skipcdata);
}

bool CXMLReader::SImplementation::ReadEntity(SXMLEntity &entity, bool skipcdata) {
    SkipCData = skipcdata;

    // Keep parsing until we have entities or reach EOF
    while (EntityQueue.empty() && !EndOfFile) {
        std::vector<char> buffer(1024); // Create a buffer
        size_t bytesRead = Source->Read(buffer, buffer.size()); // Pass the vector by reference

        if (bytesRead == 0) {
            EndOfFile = true;
            XML_Parse(Parser, nullptr, 0, XML_TRUE); // Finalize parsing
            break;
        }

        // Resize the buffer to the actual bytes read
        buffer.resize(bytesRead);

        // Parse the chunk using the buffer's data
        if (XML_Parse(Parser, buffer.data(), bytesRead, XML_FALSE) == XML_STATUS_ERROR) {
            EndOfFile = true;
            return false;
        }
    }

    if (!EntityQueue.empty()) {
        entity = EntityQueue.front();
        EntityQueue.pop_front();
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

    // Add attributes
    for (int i = 0; atts[i] != nullptr; i += 2) {
        entity.DAttributes.emplace_back(atts[i], atts[i + 1]);
    }

    impl->EntityQueue.push_back(entity); // Add to queue
}

void CXMLReader::SImplementation::EndElementHandler(void *userData, const char *name) {
    SImplementation *impl = static_cast<SImplementation*>(userData);
    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::EndElement;
    entity.DNameData = name;

    impl->EntityQueue.push_back(entity); // Add to queue
}

void CXMLReader::SImplementation::CharDataHandler(void *userData, const char *data, int len) {
    SImplementation *impl = static_cast<SImplementation*>(userData);
    if (impl->SkipCData) return;

    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::CharData;
    entity.DNameData.assign(data, len);

    impl->EntityQueue.push_back(entity); // Add to queue
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
