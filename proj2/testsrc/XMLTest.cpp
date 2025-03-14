#include "gtest/gtest.h"
#include "XMLReader.h"
#include "XMLWriter.h"
#include "StringDataSource.h"
#include "StringDataSink.h"

TEST(XMLReaderTest, SimpleXML) {
    std::string xml = "<root>Hello, World!</root>";
    CXMLReader reader(std::make_shared<CStringDataSource>(xml));
    SXMLEntity entity;

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "root");

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(entity.DNameData, "Hello, World!");

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "root");

    EXPECT_FALSE(reader.ReadEntity(entity, false));
}

TEST(XMLReaderTest, NestedElements) {
    std::string xml = "<root><child>Text</child></root>";
    CXMLReader reader(std::make_shared<CStringDataSource>(xml));
    SXMLEntity entity;

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "root");

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "child");

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(entity.DNameData, "Text");

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "child");

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "root");

    EXPECT_FALSE(reader.ReadEntity(entity, false));
}

TEST(XMLReaderTest, Attributes) {
    std::string xml = "<tag attr=\"value\"/>";
    CXMLReader reader(std::make_shared<CStringDataSource>(xml));
    SXMLEntity entity;

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "tag");

    auto it = std::find_if(entity.DAttributes.begin(), entity.DAttributes.end(),
        [](const SXMLEntity::TAttribute &attr) {
            return attr.first == "attr";
        });

    ASSERT_NE(it, entity.DAttributes.end());
    EXPECT_EQ(it->second, "value");

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "tag");

    EXPECT_FALSE(reader.ReadEntity(entity, false));
}

TEST(XMLWriterTest, SimpleXML) {
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(sink);

    writer.WriteEntity(SXMLEntity{SXMLEntity::EType::StartElement, "root", {}});
    writer.WriteEntity(SXMLEntity{SXMLEntity::EType::CharData, "Hello, World!", {}});
    writer.WriteEntity(SXMLEntity{SXMLEntity::EType::EndElement, "root", {}});

    EXPECT_EQ(sink->String(), "<root>Hello, World!</root>");
}

TEST(XMLWriterTest, NestedElements) {
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(sink);

    writer.WriteEntity(SXMLEntity{SXMLEntity::EType::StartElement, "root", {}});
    writer.WriteEntity(SXMLEntity{SXMLEntity::EType::StartElement, "child", {}});
    writer.WriteEntity(SXMLEntity{SXMLEntity::EType::CharData, "Text", {}});
    writer.WriteEntity(SXMLEntity{SXMLEntity::EType::EndElement, "child", {}});
    writer.WriteEntity(SXMLEntity{SXMLEntity::EType::EndElement, "root", {}});

    EXPECT_EQ(sink->String(), "<root><child>Text</child></root>");
}

TEST(XMLWriterTest, Attributes) {
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(sink);

    SXMLEntity entity{SXMLEntity::EType::StartElement, "tag", {{"attr", "value"}}};
    writer.WriteEntity(entity);
    writer.WriteEntity(SXMLEntity{SXMLEntity::EType::EndElement, "tag", {}});

    EXPECT_EQ(sink->String(), "<tag attr=\"value\"/>");
}