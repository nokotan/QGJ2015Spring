#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <regex>

template <class char_type>
struct Expresstion;

template <>
struct Expresstion<char> {
	static const char Regex[];
};

const char Expresstion<char>::Regex[] = "(\"([^\"]|\"\")*\"|[^,\"]+)(,|$)";

template <>
struct Expresstion<wchar_t> {
	static const wchar_t Regex[];
};

const wchar_t Expresstion<wchar_t>::Regex[] = L"(\"([^\"]|\"\")*\"|[^,\"]+)(,|$)";

#include <new>

template <class char_type>
class BasicCsvParser {
	typedef std::basic_string<char_type> string;
	typedef const char_type *cchar_array;

	typedef std::vector<string> DataList;
	typedef std::vector<DataList> ListOfDataList;

	ListOfDataList Data;
	static const int MaxLineCount = 1024;
public:
	BasicCsvParser() {
		//
	}

	BasicCsvParser(const char_type name[]) {
		Load(name);
	}

	bool Load(const char_type name[]) {
		std::basic_ifstream<char_type> Reader(name);
		std::basic_regex<char_type> Regex(Expresstion<char_type>::Regex);

		char_type Buffer[MaxLineCount];	
	
		std::regex_token_iterator<cchar_array> begin;
		std::regex_token_iterator<cchar_array> end;

		if (!Reader)	return false;

		while (Reader.getline(Buffer, MaxLineCount)) {
			Data.resize(Data.size() + 1);
			DataList &NewList = Data.back();

			std::match_results<cchar_array> res;

			auto BufferBegin = std::begin(Buffer);
			auto BufferEnd = std::end(Buffer);
			
			for (auto Iter = BufferBegin; Iter != BufferEnd;) {
				if (std::regex_search((cchar_array)Iter, (cchar_array)BufferEnd, res, Regex) && res.length() > 0) {
					// NewList.push_back(res.str());
					Iter += res.length();
				} else {
					break;
				}
			}

	/*		begin = std::regex_token_iterator<cchar_array>(bufferbegin, bufferend, regex);
		
			for (auto &elem = begin; begin != end; begin++) {
				newlist.push_back(elem->str());
			}*/
		}

		return true;
	}

	DataList &operator[](size_t index) {
		return Data[index];
	}

	int ToInt(size_t row, size_t col) {
		return std::stoi(Data[row][col]);
	}

	float ToFloat(size_t row, size_t col) {
		return std::stof(Data[row][col]);
	}

	double ToDouble(size_t row, size_t col) {
		return std::stod(Data[row][col]);
	}

	cchar_array ToCharArray(size_t row, size_t col) {
		return Data[row][col].c_str();
	}

	string &ToString(size_t row, size_t col) {
		return Data[row][col];
	}
};

typedef BasicCsvParser<char> CsvParser;
typedef BasicCsvParser<wchar_t> wCsvParser;