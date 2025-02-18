#include "XMLReader.h"
#include "XMLEntity.h"
#include "DataSource.h"
#include <expat.h>

struct CXMLReader::SImplementation {
};

CXMLReader::CXMLReader(std::shared_ptr< CDataSource > src) {
}
CXMLReader::~CXMLReader() {
}
bool CXMLReader::End() const {
    return false;  
}
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata) {
    return false;  
}