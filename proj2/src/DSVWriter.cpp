#include "DSVWriter.h"
#include <sstream>
#include <iostream>
#include <stdexcept>

// Constructor for DSV writer, sink specifies the data destination, delimiter
// specifies the delimiting character, and quoteall specifies if all values
// should be quoted or only those that contain the delimiter, a double quote,
// or a newline

CDSVWriter(std::shared_ptr< CDataSink > sink, char delimiter,bool quoteall = false);

    // Destructor for DSV writer
    ~CDSVWriter();

// Returns true if the row is successfully written, one string per column
// should be put in the row vector
bool WriteRow(const std::vector< std::string > &row);

