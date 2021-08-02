#pragma once

#ifndef CSVFile_hpp
#define CSVFile_hpp

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

class CSVFile {

	typedef vector<vector<string>> CsvVector;
	friend ostream& print(ostream&, const CSVFile&);

public:
	CSVFile();
	//CSVFile() = default;
	// Disable copy (and move) semantics.
	//CSVFile(CSVFile&&) = default;
	//CSVFile& operator=(CSVFile&&) = default;
			
	vector<string> header;
	void init(string&, bool isInteractive = true);

	inline vector<string>::size_type get_size();
	inline void headers();
	inline vector<string> get_header();
	

	CsvVector get_column(const int&);
	CsvVector get_column(const vector<int>&);
	CsvVector get_column(const string&);
	CsvVector get_column(const vector<string>&);

	vector<string> get_row(const int&);
	CsvVector get_row(const int&, const int&); // Rows in range

	// Planned: Explicitly declare line to write to.
	// Pass vector of strings to write lines for
	void write_row(const map<string, string>&);
	inline CsvVector rows();
	void write_header();
	
	//~CSVFile();
	void close();

private:
	
	CsvVector contents;
	ifstream stream_read;
	ofstream stream_write;
	inline string remove_extension(const string&);
	inline string get_extension(const string&);
	vector<string> column_info(const string&);
	string merge_row_vector(const vector<string>&);
	int index_from_string(const string&);
	map<int, string> index_from_string(const map<string, string>& row_map);
};

ostream& print(ostream&, const CSVFile&);




void CSVFile::headers() {
	for (auto&& i : header)
		cout << i << ", ";
}

vector<string>::size_type CSVFile::get_size() {
	return contents.size();
}

string CSVFile::remove_extension(const string& s) {
	string ret = s;
	ret.erase(ret.find_last_of("."), string::npos);
	return ret;
}

string CSVFile::get_extension(const string& s) {
	string ret = s;
	return s.substr(ret.find_last_of(".") + 1);
}

vector<string> CSVFile::get_header() {
	return header;
}

CSVFile::CsvVector CSVFile::rows() {
	return contents;
}
#endif /* CSVFile_hpp */
