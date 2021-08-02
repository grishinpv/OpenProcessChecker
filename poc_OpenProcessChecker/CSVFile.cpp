#include "CSVFile.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <array>

#include <experimental/filesystem>
#include <ctime>

namespace fs = std::experimental::filesystem;

// file doc: http://www.cplusplus.com/doc/tutorial/files/
// stream_readtream constructor: http://www.cplusplus.com/forum/general/17771/#msg89650


CSVFile::CSVFile()
{
}

void CSVFile::init(string& file_name, bool isInteractive) {
	string out_file = file_name;
	string file_ext = get_extension(file_name);

	//check file exist
	int i = 1;
	for (;;) {
		
		if (fs::exists(out_file) && isInteractive) {
			//TRACE_NORMAL(L"File exist: %hs", out_file);
			//cout << "File exist: " << out_file << endl;
			out_file = remove_extension(file_name) + "_" + std::to_string(i) + "." + file_ext;
			//cout << "Try save as: " << out_file << endl;
			i++;
		}
		else
			break;
		
	}

	//TRACE_NORMAL(L"Try save as: %hs", out_file);
	cout << "Try save as: " << out_file << endl;
	
	stream_write.open(out_file, std::ofstream::out);
	if (stream_write.fail()) {
		throw std::runtime_error("File open error");
	}
	file_name = out_file;
}

void CSVFile::write_header() {
	stream_write << merge_row_vector(header);
}


void CSVFile::write_row(const map<string, string>& row_map) {
	vector<string> row_vec(header.size(), "NULL");
	map<int, string> index_map = index_from_string(row_map);
	for (auto it = index_map.begin(); it != index_map.end(); ++it) {
		row_vec[it->first] = it->second;
	}
	// for(auto&& i : row_vec) { cout << i << endl; }

	string v_string = merge_row_vector(row_vec);
	stream_write << endl << v_string;
}

map<int, string> CSVFile::index_from_string(const map<string, string>& row_map) {
	map<int, string> index_map;
	for (auto it = row_map.begin(); it != row_map.end(); ++it) {
		index_map[index_from_string(it->first)] = it->second;
	}
	return index_map;
}

int CSVFile::index_from_string(const string& s) {
	// Given column string, returns index of column
	int pos = find(header.begin(), header.end(), s) - header.begin();
	if (pos == header.size()) {
		throw out_of_range("\nColumn: " + s + " not found in header");
	}
	return pos;
}

// http://stackoverflow.com/questions/1430757/c-vector-to-string
string CSVFile::merge_row_vector(const vector<string>& row_vec) {
	stringstream sstream;
	for (vector<string>::size_type ctr = 0; ctr < row_vec.size(); ++ctr) {
		string current = row_vec[ctr];
		// cout << "called : " << current << endl;
		if (ctr == 0)
			sstream << current << ",";
		else if (current == "NULL")
			sstream << ",";
		else if (ctr == row_vec.size() - 1)
			sstream << current;
		else
			sstream << current << ",";
	}
 	return sstream.str();
}

vector<string> CSVFile::column_info(const string& s) {
	// Returns all data in the passed column(s)
	// Need to overload this function later
	vector<string> data;
	int index = index_from_string(s);

	for (auto&& i : contents) {
		data.push_back(i[index]);
	}

	return data;
}

CSVFile::CsvVector CSVFile::get_column(const int& index) {
	CSVFile::CsvVector column_data;
	vector<string> buffer;

	for (unsigned i = 0; i < contents.size(); ++i) {
		buffer.push_back(contents[i][index]);
	}
	column_data.push_back(buffer);
	return column_data;
}

CSVFile::CsvVector CSVFile::get_column(const vector<int>& indices) {
	CSVFile::CsvVector column_data;
	for (auto&& index : indices)
	{
		vector<string> buffer;
		for (unsigned i = 0; i < contents.size(); ++i)
		{
			buffer.push_back(contents[i][index]);
		}
		column_data.push_back(buffer);
	}
	return column_data;
}

CSVFile::CsvVector CSVFile::get_column(const string& s) {
	CSVFile::CsvVector column_data;
	column_data.push_back(column_info(s));
	return column_data;
}

CSVFile::CsvVector CSVFile::get_column(const vector<string>& col_vec) {
	CSVFile::CsvVector column_data;
	for (auto&& column : col_vec) {
		column_data.push_back(column_info(column));
	}
	return column_data;
}

vector<string> CSVFile::get_row(const int& i) {
	return contents[i];
}

CSVFile::CsvVector CSVFile::get_row(const int& start, const int& end) {
	CsvVector row_range;
	for (unsigned i = start; i != end; ++i) {
		row_range.push_back(contents[i]);
	}
	return row_range;
}

ostream& print(ostream& os, const CSVFile& csv) {
	string csv_string;
	for (auto&& i : csv.contents) {
		stringstream s;
		for (auto&& j : i) {
			s << j << ' ';
		}
		s << "\n";
		csv_string += s.str();
	}
	os << csv_string;
	return os;
}

void CSVFile::close() {
	try {
		stream_read.close();
		stream_write.close();
	}
	catch (...) {
		wprintf(L"[!] Free file exception");
	}
	
}

/*
CSVFile::~CSVFile(void) {
	stream_read.close();
	stream_write.close();
	header.clear();
}
*/
