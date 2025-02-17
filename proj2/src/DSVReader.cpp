#include "DSVReader.h"
#include <iostream>
#include <sstream>

// Implementation structure for CDSVReader
struct CDSVReader::SImplementation {
    std::shared_ptr<CDataSource> DataSource;  // Data source (file, stream, etc.)
    char Delimiter;                          // Delimiter used to separate columns (e.g., comma for CSV)
    bool EndReached;                         // Flag to track the end of data
};

// Constructor for DSV reader, src specifies the data source and delimiter
// specifies the delimiting character
CDSVReader::CDSVReader(std::shared_ptr<CDataSource> src, char delimiter)
    : DImplementation(std::make_unique<SImplementation>()) {
    DImplementation->DataSource = src;
    DImplementation->Delimiter = delimiter;
    DImplementation->EndReached = false;  // Initially not at the end
}

