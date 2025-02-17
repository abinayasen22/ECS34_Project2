#include "DSVReader.h"
#include "DataSource.h"
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

// Constructor for DSV reader, src specifies the data source and delimiter
// specifies the delimiting character
struct CDSVReader::SImplementation {
    std::shared_ptr<CDataSource> DDataSource; // Data source to read from
    char DDelimiter; // Delimiter character
    bool DEndReached; // Flag to indicate if the end of the file is reached

    // Helper function to read a line from the data source
    bool ReadLine(std::string& line) {
        line.clear();
        char ch;
        while (DDataSource->Get(ch)) {
            if (ch == '\n') {
                break; // End of line
            }
            line += ch;
        }
        return !line.empty() || !DDataSource->End(); // Return true if data was read
    }

    // Helper function to split a line into fields based on the delimiter
    std::vector<std::string> SplitLine(const std::string& line) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string field;
        while (std::getline(ss, field, DDelimiter)) {
            row.push_back(field);
        }
        return row;
    }
};

// Constructor for CDSVReader
CDSVReader::CDSVReader(std::shared_ptr<CDataSource> src, char delimiter)
    : DImplementation(std::make_unique<SImplementation>(src, delimiter)) {}

// Destructor for CDSVReader
CDSVReader::~CDSVReader() {}

// Check if all rows have been read
bool CDSVReader::End() const {
    return DImplementation->DEndReached;
}

// Read a row from the DSV file
bool CDSVReader::ReadRow(std::vector<std::string>& row) {
    std::string line;
    if (!DImplementation->ReadLine(line)) {
        DImplementation->DEndReached = true; // End of file reached
        return false;
    }
    row = DImplementation->SplitLine(line); // Split the line into fields
    return true;
}