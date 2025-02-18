#include "gtest/gtest.h"
#include "DSVReader.h"
#include "DSVWriter.h"
#include "StringDataSource.h"
#include "StringDataSink.h"
#include <vector>
#include <string>

// Test CDSVReader with a simple DSV file (two rows)
TEST(DSVReaderTest, SimpleDSV) {
    std::string data = "Name,Age,Location\nJane,20,Davis\n";
    auto source = std::make_shared<CStringDataSource>(data);
    CDSVReader reader(source, ',');

    std::vector<std::string> row;
    ASSERT_TRUE(reader.ReadRow(row)); // First row
    EXPECT_EQ(row, (std::vector<std::string>{"Name", "Age", "Location"}));

    ASSERT_TRUE(reader.ReadRow(row)); // Second row
    EXPECT_EQ(row, (std::vector<std::string>{"Jane", "20", "Davis"}));

    ASSERT_TRUE(reader.End()); // End of file
}

// Test CDSVReader with quoted fields (two rows)
TEST(DSVReaderTest, QuotedFields) {
    std::string data = "\"Name\",\"Age\",\"Location\"\n\"Jane\",\"25\",\"New, Jersey\"\n";
    auto source = std::make_shared<CStringDataSource>(data);
    CDSVReader reader(source, ',');

    std::vector<std::string> row;
    ASSERT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row, (std::vector<std::string>{"\"Name\"", "\"Age\"", "\"Location\""}));

    ASSERT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row, (std::vector<std::string>{"Jane", "25", "New, Jersey"}));

    ASSERT_TRUE(reader.End()); // End of file (EOF)
}

// Test CDSVReader with empty fields (two rows)
TEST(DSVReaderTest, EmptyFields) {
    std::string data = "Name,Age,Location\nJane,,Davis\n";
    auto source = std::make_shared<CStringDataSource>(data);
    CDSVReader reader(source, ',');

    std::vector<std::string> row;
    ASSERT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row, (std::vector<std::string>{"Name", "Age", "Location"}));

    ASSERT_TRUE(reader.ReadRow(row)); 
    EXPECT_EQ(row, (std::vector<std::string>{"Jane", "", "Davis"}));

    ASSERT_TRUE(reader.End()); 
}

// Test CDSVWriter with simple DSV (two rows)
TEST(DSVWriterTest, SimpleDSV) {
    auto sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',');

    std::vector<std::string> row1 = {"Name", "Age", "Location"};
    std::vector<std::string> row2 = {"Jane", "25", "Davis"};

    ASSERT_TRUE(writer.WriteRow(row1));
    ASSERT_TRUE(writer.WriteRow(row2));

    std::string expected = "Name,Age,Location\nJane,25,Davis\n";
    EXPECT_EQ(sink->String(), expected);
}

// Test CDSVWriter with quoted fields (two rows)
TEST(DSVWriterTest, QuotedFields) {
    auto sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',');

    std::vector<std::string> row1 = {"Name", "Age", "Location"};
    std::vector<std::string> row2 = {"Jane", "25", "New, York"};

    ASSERT_TRUE(writer.WriteRow(row1));
    ASSERT_TRUE(writer.WriteRow(row2));

    std::string expected = "Name,Age,Location\nJane,25,\"New, York\"\n";
    EXPECT_EQ(sink->String(), expected);
    std::cout << sink->String() << std::endl;
}

// Test CDSVWriter with quoteall flag (two rows)
TEST(DSVWriterTest, QuoteAll) {
    auto sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',', true); // Quote all fields

    std::vector<std::string> row1 = {"Name", "Age", "Location"};
    std::vector<std::string> row2 = {"Jane", "25", "Davis"};

    ASSERT_TRUE(writer.WriteRow(row1));
    ASSERT_TRUE(writer.WriteRow(row2));

    std::string expected = "\"Name\",\"Age\",\"Location\"\n\"Jane\",\"25\",\"Davis\"\n";
    EXPECT_EQ(sink->String(), expected);
}

// Test CDSVWriter with trailing delimiter (two rows)
TEST(DSVWriterTest, TrailingDelimiter) {
    auto sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',');

    std::vector<std::string> row1 = {"Name", "Age", "Location", ""};
    std::vector<std::string> row2 = {"Jane", "25", "Davis", ""};

    ASSERT_TRUE(writer.WriteRow(row1));
    ASSERT_TRUE(writer.WriteRow(row2));

    std::string expected = "Name,Age,Location,\nJane,25,Davis,\n";
    EXPECT_EQ(sink->String(), expected);
}