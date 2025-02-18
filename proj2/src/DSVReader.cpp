#include "DSVReader.h"
#include "DataSource.h"
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

// Constructor for DSV reader, src specifies the data source and delimiter
// specifies the delimiting character

// Constructor for CDSVReader
CDSVReader::CDSVReader(std::shared_ptr<CDataSource> src, char delimiter)
    : implementation(std::make_unique<SImplementation>(src, delimiter)) {}

// Destructor for CDSVReader
CDSVReader::~CDSVReader() {}

// Check if all rows have been read
bool CDSVReader::End() const {
    return implementation->endReached;
}

// Read a row from the DSV file
bool CDSVReader::ReadRow(std::vector<std::string>& row) {
    std::string line;
    if (!implementation->ReadLine(line)) {
        implementation->endReached = true; // End of file reached
        return false;
    }
    row = implementation->SplitLine(line); // Split the line into fields
    return true;
}
struct CDSVReader::SImplementation {
    std::shared_ptr<CDataSource> dataSource; // Data source to read from
    char delimiter; // Delimiter character
    bool endReached; // Flag to indicate if the end of the file is reached

    // Helper function to read a line from the data source
    bool ReadLine(std::string& line) {
        line.clear();
        char ch;
        while (dataSource->Get(ch)) {
            if (ch == '\n') {
                break; // End of line
            }
            line += ch;
        }
        return !line.empty() || !dataSource->End(); // Return true if data was read
    }

    // Helper function to split a line into fields based on the delimiter
    std::vector<std::string> SplitLine(const std::string& line) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string field;
        while (std::getline(ss, field, delimiter)) {
            row.push_back(field);
        }
        return row;
    }
};