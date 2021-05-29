#include <DataFrame/DataFrame.h>
#include <DataFrame/DataFrameFinancialVisitors.h>
#include <DataFrame/DataFrameMLVisitors.h>
#include <DataFrame/DataFrameOperators.h>
#include <DataFrame/DataFrameStatsVisitors.h>
#include <DataFrame/RandGen.h>
using MyDataFrame = hmdf::StdDataFrame<unsigned long>;
struct User {
    int id;
    int sex;
};
template<typename I, typename  H, typename CppType, typename ArrType>
void NumericColumnToDataFrame(const Table& tlb, DataFrame<I, H>& df, int c)
{
	const int64_t rows = tlb.num_rows();
	auto f = tlb.field(c);
	const string& name = f->name();
	std::vector<CppType>& vec = df.create_column<CppType>(name.c_str());
	vec.assign(rows, 0);
	auto ch_arr = tlb.column(c);
	const int nchunks = ch_arr->num_chunks();
	int i = 0;
	for (int n = 0; n < nchunks; n++)
	{
		auto arr = ch_arr->chunk(n);
		int64_t arr_rows = arr->length();
		auto typed_arr = std::static_pointer_cast<ArrType>(arr);
		const CppType* data = typed_arr->raw_values();
		for (int j = 0; j < arr_rows; j++)
			vec[i++] = data[j];
	}
}

template<typename I, typename  H>
bool TableToDataFrame(const Table& tlb, DataFrame<I, H>& df)
{
	const int64_t rows = tlb.num_rows();
	const int cols = tlb.num_columns();

	// DataFrame requires sequence index.
	df.load_data(DataFrame<I, H>::gen_sequence_index(1, rows));

	for (int c = 0; c < cols; c++)
	{
		auto f = tlb.field(c);
		const string& name = f->name();
		int type_id = f->type()->id();
		switch (type_id)
		{
		case Type::STRING:
		{
			std::vector<string>& vec = df.create_column<string>(name.c_str());
			vec.assign(rows, "");
			auto ch_arr = tlb.column(c);
			int nchunks = ch_arr->num_chunks();
			int i = 0;
			for (int n = 0; n < nchunks; n++)
			{
				auto arr = ch_arr->chunk(n);
				int64_t arr_rows = arr->length();
				auto typed_arr = std::static_pointer_cast<arrow::StringArray>(arr);
				for (int j = 0; j < arr_rows; j++)
					vec[i++] = typed_arr->GetString(j);
			}
			break;
		}
		case Type::BOOL:
		{
			std::vector<bool>& vec = df.create_column<bool>(name.c_str());
			vec.assign(rows, false);
			auto ch_arr = tlb.column(c);
			int nchunks = ch_arr->num_chunks();
			int i = 0;
			for (int n = 0; n < nchunks; n++)
			{
				auto arr = ch_arr->chunk(n);
				int64_t arr_rows = arr->length();
				auto typed_arr = std::static_pointer_cast<arrow::BooleanArray>(arr);
				for (int j = 0; j < arr_rows; j++)
					vec[i++] = typed_arr->GetView(j);
			}
			break;
		}
		case Type::FLOAT:
			NumericColumnToDataFrame<I, H, float, arrow::FloatArray>(tlb, df, c);
			break;
		case Type::DOUBLE:
			NumericColumnToDataFrame<I, H, double, arrow::DoubleArray>(tlb, df, c);
			break;
		case Type::UINT8:
			NumericColumnToDataFrame<I, H, uint8_t, arrow::UInt8Array>(tlb, df, c);
			break;
		case Type::INT8:
			NumericColumnToDataFrame<I, H, int8_t, arrow::Int8Array>(tlb, df, c);
			break;
		case Type::UINT16:
			NumericColumnToDataFrame<I, H, uint16_t, arrow::UInt16Array>(tlb, df, c);
			break;
		case Type::INT16:
			NumericColumnToDataFrame<I, H, int16_t, arrow::Int16Array>(tlb, df, c);
			break;
		case Type::UINT32:
			NumericColumnToDataFrame<I, H, uint32_t, arrow::UInt32Array>(tlb, df, c);
			break;
		case Type::INT32:
			NumericColumnToDataFrame<I, H, int32_t, arrow::Int32Array>(tlb, df, c);
			break;
		case Type::UINT64:
			NumericColumnToDataFrame<I, H, uint64_t, arrow::UInt64Array>(tlb, df, c);
			break;
		case Type::INT64:
			NumericColumnToDataFrame<I, H, int64_t, arrow::Int64Array>(tlb, df, c);
			break;
		default:
			assert(false); // unsupported type
		}
	}

	return true;
}


int main(int argc, char *argv[])
{
	auto fs = make_shared<fs::LocalFileSystem>();
	auto r_input = fs->OpenInputStream("c:/temp/Test.csv");

	auto pool = default_memory_pool();
	auto read_options = arrow::csv::ReadOptions::Defaults();
	auto parse_options = arrow::csv::ParseOptions::Defaults();
	auto convert_options = arrow::csv::ConvertOptions::Defaults();

	auto r_table_reader = csv::TableReader::Make(pool, r_input.ValueOrDie(),
		read_options, parse_options, convert_options);
	auto r_read = r_table_reader.ValueOrDie()->Read();
	auto pTable = r_read.ValueOrDie();

	PrettyPrintOptions options{0};
	arrow::PrettyPrint(*pTable, options, &std::cout);
	//arrow::PrettyPrint(*pTable->schema(), options, &std::cout);

	// SBW 2020.04.03 Attach Arrow table to DataFrame.
	MyDataFrame df;
	// df_read.read("c:/temp/Test.csv");
	TableToDataFrame(*pTable, df);
	df.write<std::ostream, int, unsigned long, double, std::string>(std::cout);

	return 1;
}