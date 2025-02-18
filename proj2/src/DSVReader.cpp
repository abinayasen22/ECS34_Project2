#include "DSVReader.h"
#include "DataSource.h"
#include <sstream>
#include <algorithm>


struct CDSVReader::SImplementation {
    std::shared_ptr<CDataSource> source; // Data source to read from 
    char delimiter; // Delimiter character
    // constructor 
    SImplementation(std::shared_ptr<CDataSource> src, char del){
        source = src;
        delimiter = del;
    }

};

CDSVReader::CDSVReader(std::shared_ptr<CDataSource> src, char del) {
    implementation = std::make_unique<SImplementation>(src, del);
}

CDSVReader::~CDSVReader() {}

bool CDSVReader::End() const {
    // Implementation for End()
    return implementation->source->End();;
}

bool CDSVReader::ReadRow(std::vector<std::string>& row) {
    if (End()){
        return false;   // end of DSV file
    }
    char ch;
    std::string value; // string of a single value within DSV file
    row.clear();    // clear vector for new row

    while (!implementation->source->End()) {
        implementation->source->Get(ch); // read next character
        if (ch == '\n') {   // end oof row
            break;
        }
        if (ch == implementation->delimiter) {  //delimeter reached
            row.push_back(value);   //add value to row vector
            value.clear();  // clear value for next one
        } else {
            value += ch;    //add curr char to value 
        }
    }

    return true; // Row was successfully read
}