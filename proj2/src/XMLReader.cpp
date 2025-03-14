#include "XMLReader.h"
#include <expat.h>
#include <vector>
#include <deque>
#include <sstream>
#include <stdexcept>

struct CXMLReader::SImplementation {
    std::shared_ptr<CDataSource> DDataSource;
    XML_Parser DParser;
    std::deque<SXMLEntity> DEntities;
    bool DParsingFinished;

    SImplementation(std::shared_ptr<CDataSource> src)
        : DDataSource(std::move(src)), DParsingFinished(false) {
        DParser = XML_ParserCreate(nullptr);
        XML_SetUserData(DParser, this);
        XML_SetElementHandler(DParser, StartElementHandler, EndElementHandler);
        XML_SetCharacterDataHandler(DParser, CharacterDataHandler);
    }

    ~SImplementation() {
        XML_ParserFree(DParser);
    }

    static void StartElementHandler(void *userData, const XML_Char *name, const XML_Char **atts) {
        auto *self = static_cast<SImplementation*>(userData);
        SXMLEntity entity;
        entity.DType = SXMLEntity::EType::StartElement;
        entity.DNameData = name;
        for (int i = 0; atts[i] != nullptr; i += 2) {
            entity.DAttributes.emplace_back(atts[i], atts[i + 1]);
        }
        self->DEntities.push_back(entity);
    }

    static void EndElementHandler(void *userData, const XML_Char *name) {
        auto *self = static_cast<SImplementation*>(userData);
        SXMLEntity entity;
        entity.DType = SXMLEntity::EType::EndElement;
        entity.DNameData = name;
        self->DEntities.push_back(entity);
    }

    static void CharacterDataHandler(void *userData, const XML_Char *s, int len) {
        auto *self = static_cast<SImplementation*>(userData);
        std::string data(s, len);
        if (!data.empty()) {
            SXMLEntity entity;
            entity.DType = SXMLEntity::EType::CharData;
            entity.DNameData = data;
            self->DEntities.push_back(entity);
        }
    }

    bool ParseChunk() {
        std::vector<char> buffer(4096);
        size_t bytesRead = DDataSource->Read(buffer, buffer.size()); // Pass the vector directly
        if (bytesRead > 0) {
            if (XML_Parse(DParser, buffer.data(), bytesRead, bytesRead < buffer.size()) == XML_STATUS_ERROR) {
                DParsingFinished = true;
                return false;
            }
            return true;
        } else {
            XML_Parse(DParser, nullptr, 0, XML_TRUE);
            DParsingFinished = true;
            return true;
        }
    }
};

CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src)
    : implementation(std::make_unique<SImplementation>(std::move(src))) {}

CXMLReader::~CXMLReader() = default;

bool CXMLReader::End() const {
    return implementation->DParsingFinished && implementation->DEntities.empty();
}

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata) {
    while (true) {
        if (!implementation->DEntities.empty()) {
            auto &front = implementation->DEntities.front();
            if (skipcdata && front.DType == SXMLEntity::EType::CharData) {
                implementation->DEntities.pop_front();
                continue;
            }
            entity = front;
            implementation->DEntities.pop_front();
            return true;
        }
        if (implementation->DParsingFinished) {
            return false;
        }
        if (!implementation->ParseChunk()) {
            implementation->DParsingFinished = true;
            return false;
        }
    }
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
