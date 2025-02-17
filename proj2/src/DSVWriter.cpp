#include "DSVWriter.h"
#include "DataSink.h"
#include <vector>
#include <string>
#include <sstream>

// Constructor for DSV writer, sink specifies the data destination, delimiter
// specifies the delimiting character, and quoteall specifies if all values
// should be quoted or only those that contain the delimiter, a double quote,
// or a newline
struct CDSVWriter::SImplementation {
    std::shared_ptr<CDataSink> DDataSink; // Data sink to write to
    char DDelimiter; // Delimiter character
    bool DQuoteAll; // Flag to determine if all fields should be quoted

    // Constructor for SImplementation
    SImplementation(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall)
        : DDataSink(sink), DDelimiter(delimiter), DQuoteAll(quoteall) {}

    // Helper function to escape a field if necessary
    std::string EscapeField(const std::string& field) {
        bool needsQuotes = DQuoteAll || field.find(DDelimiter) != std::string::npos ||
                           field.find('\"') != std::string::npos ||
                           field.find('\n') != std::string::npos;
        if (!needsQuotes) {
            return field;
        }
        std::string escapedField = "\"";
        for (char ch : field) {
            if (ch == '\"') {
                escapedField += "\"\""; // Escape double quotes
            } else {
                escapedField += ch;
            }
        }
        escapedField += "\"";
        return escapedField;
    }

    // Helper function to write a string to the data sink
    bool WriteString(const std::string& str) {
        std::vector<char> buf(str.begin(), str.end());
        return DDataSink->Write(buf);
    }
};

// Constructor for CDSVWriter
CDSVWriter::CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall)
    : DImplementation(std::make_unique<SImplementation>(sink, delimiter, quoteall)) {}

// Destructor for CDSVWriter
CDSVWriter::~CDSVWriter() {}

// Write a row to the DSV file
bool CDSVWriter::WriteRow(const std::vector<std::string>& row) {
    std::stringstream line;
    for (size_t i = 0; i < row.size(); ++i) {
        line << DImplementation->EscapeField(row[i]); // Escape the field
        if (i != row.size() - 1) {
            line << DImplementation->DDelimiter; // Add delimiter between fields
        }
    }
    line << '\n'; // End of line
    return DImplementation->WriteString(line.str()); // Write the line to the sink
}