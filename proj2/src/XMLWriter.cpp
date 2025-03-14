#include "XMLWriter.h"
#include "DataSink.h"
#include "XMLWriter.h"
#include <vector>
#include <sstream>
#include <stdexcept>

// Private implementation of CXMLWriter using the Pimpl idiom
struct CXMLWriter::SImplementation {
    std::shared_ptr<CDataSink> DDataSink; // Data sink for writing XML
    std::vector<std::string> DElementStack; // Stack to track open elements
    std::ostringstream DBuffer; // Buffer to accumulate XML data

    // Constructor: Initializes the data sink
    SImplementation(std::shared_ptr<CDataSink> sink)
        : DDataSink(std::move(sink)) {}

    // Flush the buffer to the data sink
    void FlushBuffer() {
        std::string data = DBuffer.str(); // Get buffer content
        if (!data.empty()) {
            // Convert std::string to std::vector<char>
            std::vector<char> buffer(data.begin(), data.end());
            DDataSink->Write(buffer); // Write to sink
            DBuffer.str(""); // Clear the buffer
            DBuffer.clear();
        }
    }

    // Escape special XML characters in a string
    std::string EscapeXML(const std::string &input) {
        std::string output;
        for (char c : input) {
            switch (c) {
                case '&': output += "&amp;"; break;
                case '<': output += "&lt;"; break;
                case '>': output += "&gt;"; break;
                case '"': output += "&quot;"; break;
                case '\'': output += "&apos;"; break;
                default: output += c; break;
            }
        }
        return output;
    }

    // Write a start element (e.g., <element>)
    void WriteStartElement(const std::string &name, const std::vector<SXMLEntity::TAttribute> &attrs) {
        DBuffer << "<" << name; // Write element name
        // Write attributes
        for (const auto &attr : attrs) {
            DBuffer << " " << attr.first << "=\"" << EscapeXML(attr.second) << "\"";
        }
        DBuffer << ">"; // Close the start tag
        FlushBuffer(); // Flush the buffer
    }

    // Write an end element (e.g., </element>)
    void WriteEndElement(const std::string &name) {
        DBuffer << "</" << name << ">"; // Write end tag
        FlushBuffer(); // Flush the buffer
    }

    // Write a complete element (e.g., <element />)
    void WriteCompleteElement(const std::string &name, const std::vector<SXMLEntity::TAttribute> &attrs) {
        DBuffer << "<" << name; // Write element name
        // Write attributes
        for (const auto &attr : attrs) {
            DBuffer << " " << attr.first << "=\"" << EscapeXML(attr.second) << "\"";
        }
        DBuffer << "/>"; // Close the self-closing tag
        FlushBuffer(); // Flush the buffer
    }

    // Write character data (e.g., text content)
    void WriteString(const std::string &str) {
        DBuffer << EscapeXML(str); // Write escaped string
        FlushBuffer(); // Flush the buffer
    }
};

// Constructor: Initializes the implementation with a data sink
CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
    : DImplementation(std::make_unique<SImplementation>(std::move(sink))) {}

// Destructor: Ensures all open elements are closed
CXMLWriter::~CXMLWriter() {
    Flush();
}

// Flush all open elements by writing their end tags
bool CXMLWriter::Flush() {
    while (!DImplementation->DElementStack.empty()) {
        std::string name = DImplementation->DElementStack.back();
        DImplementation->DElementStack.pop_back();
        DImplementation->DBuffer << "</" << name << ">"; // Write end tag
    }
    DImplementation->FlushBuffer(); // Flush the buffer
    return true;
}

// Write an XML entity to the output
bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
    switch (entity.DType) {
        case SXMLEntity::EType::StartElement:
            DImplementation->WriteStartElement(entity.DNameData, entity.DAttributes);
            DImplementation->DElementStack.push_back(entity.DNameData); // Track open element
            break;
        case SXMLEntity::EType::EndElement:
            if (!DImplementation->DElementStack.empty() && DImplementation->DElementStack.back() == entity.DNameData) {
                DImplementation->DElementStack.pop_back(); // Remove from stack
            }
            DImplementation->WriteEndElement(entity.DNameData);
            break;
        case SXMLEntity::EType::CompleteElement:
            DImplementation->WriteCompleteElement(entity.DNameData, entity.DAttributes);
            break;
        case SXMLEntity::EType::CharData:
            DImplementation->WriteString(entity.DNameData);
            break;
        default:
            return false; // Invalid entity type
    }
    return true;
}
// struct CXMLWriter::SImplementation {
// };

// // Constructor for XML writer, sink specifies the data destination
// CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> /*sink*/) {
//     // You can initialize your internal state here if needed, using 'sink'
// }

// // Destructor for XML writer
// CXMLWriter::~CXMLWriter() {
//     // Cleanup code here if needed
// }

// // Outputs all end elements for those that have been started
// bool CXMLWriter::Flush() {
//     // Implement the logic to flush the XML content
//     return true;
// }

// // Writes out the entity to the output stream
// bool CXMLWriter::WriteEntity(const SXMLEntity &/*entity*/) {
//     // Implement the logic to write an entity to the data sink
//     return true;
// }
