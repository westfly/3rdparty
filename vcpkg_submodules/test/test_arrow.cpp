#include <arrow/api.h>
#include <arrow/csv/api.h>
#include <arrow/io/file.h>
#include <fmt/format.h>
#include <iostream>
#include <parquet/arrow/reader.h>
#include <parquet/stream_reader.h>
#include <parquet/stream_writer.h>
#include <string>
#include <vector>

struct Article {
    std::string name;
    float       price;
    int         quantity;
};

std::vector<Article> get_articles() {
    std::vector<Article> articles{Article{"南昌好景色", 35.0f, 20},
                                  Article{"武汉好风景", 24.0f, 30},
                                  Article{"北京王府井", 50.0f, 10}};
    return std::move(articles);
}

std::shared_ptr<arrow::Table> generate_table() {
    arrow::Int64Builder i64builder;
    PARQUET_THROW_NOT_OK(i64builder.AppendValues({1, 2, 3, 4, 5}));
    std::shared_ptr<arrow::Array> i64array;
    PARQUET_THROW_NOT_OK(i64builder.Finish(&i64array));

    arrow::StringBuilder strbuilder;
    PARQUET_THROW_NOT_OK(strbuilder.Append("some"));
    PARQUET_THROW_NOT_OK(strbuilder.Append("string"));
    PARQUET_THROW_NOT_OK(strbuilder.Append("content"));
    PARQUET_THROW_NOT_OK(strbuilder.Append("in"));
    PARQUET_THROW_NOT_OK(strbuilder.Append("rows"));
    std::shared_ptr<arrow::Array> strarray;
    PARQUET_THROW_NOT_OK(strbuilder.Finish(&strarray));

    std::shared_ptr<arrow::Schema> schema =
        arrow::schema({arrow::field("int", arrow::int64()),
                       arrow::field("str", arrow::utf8())});

    return arrow::Table::Make(schema, {i64array, strarray});
}
int ReadParquetDynamicFromFile(const std::string& filename) {
    std::shared_ptr<arrow::io::ReadableFile> infile;
    PARQUET_ASSIGN_OR_THROW(infile, arrow::io::ReadableFile::Open(filename));
    std::unique_ptr<parquet::arrow::FileReader> reader;
    parquet::arrow::OpenFile(infile, arrow::default_memory_pool(), &reader);
    std::shared_ptr<arrow::Table> table;
    auto                          st = reader->ReadTable(&table);
    if (!st.ok()) {
        return -1;
    }
    fmt::print("Loaded {} rows in {} columns;\n",
                table->num_rows(),
                table->num_columns());
    return 0;
}
int ReadParquetFromFile(const std::string& filename) {
    std::shared_ptr<arrow::io::ReadableFile> infile;
    PARQUET_ASSIGN_OR_THROW(infile, arrow::io::ReadableFile::Open(filename));

    parquet::StreamReader is{parquet::ParquetFileReader::Open(infile)};

    Article arti;

    while (!is.eof()) {
        is >> arti.name >> arti.price >> arti.quantity >> parquet::EndRow;
        std::cout << arti.name << " " << arti.price << " " << arti.quantity
                  << std::endl;
    }
    return 0;
}
int WriteParquetToFile(const std::string& filename) {
    std::shared_ptr<arrow::io::FileOutputStream> out_file;
    PARQUET_ASSIGN_OR_THROW(out_file,
                            arrow::io::FileOutputStream::Open(filename));

    parquet::WriterProperties::Builder builder;

    parquet::schema::NodeVector fields;

    fields.push_back(
        parquet::schema::PrimitiveNode::Make("name",
                                             parquet::Repetition::OPTIONAL,
                                             parquet::Type::BYTE_ARRAY,
                                             parquet::ConvertedType::UTF8));

    fields.push_back(
        parquet::schema::PrimitiveNode::Make("price",
                                             parquet::Repetition::REQUIRED,
                                             parquet::Type::FLOAT,
                                             parquet::ConvertedType::NONE,
                                             -1));

    fields.push_back(
        parquet::schema::PrimitiveNode::Make("quantity",
                                             parquet::Repetition::REQUIRED,
                                             parquet::Type::INT32,
                                             parquet::ConvertedType::INT_32,
                                             -1));

    std::shared_ptr<parquet::schema::GroupNode> schema =
        std::static_pointer_cast<parquet::schema::GroupNode>(
            parquet::schema::GroupNode::Make(
                "schema", parquet::Repetition::REQUIRED, fields));

    parquet::StreamWriter os{
        parquet::ParquetFileWriter::Open(out_file, schema, builder.build())};

    for (const auto& a : get_articles()) {
        os << a.name << a.price << a.quantity << parquet::EndRow;
    }
    return 0;
}
int ConvertCSV(const std::string& filename) {
    std::shared_ptr<arrow::io::ReadableFile> infile;
    PARQUET_ASSIGN_OR_THROW(infile, arrow::io::ReadableFile::Open(filename));

    parquet::StreamReader is{parquet::ParquetFileReader::Open(infile)};
    arrow::io::IOContext  io_context = arrow::io::default_io_context();

    auto read_options = arrow::csv::ReadOptions::Defaults();
    auto parse_options = arrow::csv::ParseOptions::Defaults();
    auto convert_options = arrow::csv::ConvertOptions::Defaults();

    // Instantiate TableReader from input stream and options
    /*
    std::shared_ptr<arrow::io::InputStream> input = is;
    auto maybe_reader = arrow::csv::TableReader::Make(
        io_context, input, read_options, parse_options, convert_options);
    */
    return 0;
}
int main(int argc, char* argv[]) {
    std::string filename = "../data/test.parquet";
    WriteParquetToFile(filename);
    ReadParquetFromFile(filename);
    ReadParquetDynamicFromFile(filename);
    return 0;
}