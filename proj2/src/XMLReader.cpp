#include "XMLReader.h"
#include <string>
#include <vector>
#include <memory>

class SXMLEntity {
public:
    enum class EType { StartElement, EndElement, CompleteElement };
    
    EType DType;
    std::string DNameData;
    std::vector<std::pair<std::string, std::string>> DAttributes;

    bool AttributeExists(const std::string &name) const {
        for (const auto &attribute : DAttributes) {
            if (attribute.first == name) {
                return true;
            }
        }
        return false;
    }

    std::string AttributeValue(const std::string &name) const {
        for (const auto &attribute : DAttributes) {
            if (attribute.first == name) {
                return attribute.second;
            }
        }
        return "";
    }

    bool SetAttribute(const std::string &name, const std::string &value) {
        for (auto &attribute : DAttributes) {
            if (attribute.first == name) {
                attribute.second = value;
                return true;
            }
        }
        DAttributes.push_back(std::make_pair(name, value));
        return true;
    }
};

class CXMLReader {
private:
    std::shared_ptr<CDataSource> DSource;

public:
    CXMLReader(std::shared_ptr<CDataSource> src) : DSource(src) {}
    ~CXMLReader() {}

    bool End() const {
        // Implementation to check if all entities have been read
        return true;
    }

    bool ReadEntity(SXMLEntity &entity, bool skipcdata = false) {
        // Implementation to read an entity from the XML
        return true;
    }
};