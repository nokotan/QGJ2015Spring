#pragma once


#include <string>
#include <vector>
#include <sstream>
#include <DxLib.h>

// using namespace std;


// FileReader.h ＠色男
//
//　主に区切り文字で表形式に保存されたテキストファイルを読み込むためのクラス。
//	クラスの知識はなくとも使えます
//	
//	【使い方】
//	①CFileRead型の変数を宣言　
//	②メンバ関数のFireReadを呼び出す（第1引数はファイルパス、第2引数は区切り文字）
//	③メンバ関数のGetDataを使って読み込んだデータを取得する（引数で何行目何番目のデータなのかを指定する）
//	
//　付属のサンプルを見てなんとなく使い方を把握してもらえれば
//
//　※分かる人向け注意
//  stdをusgin namespaceしているのでしたくないという修正どうぞ。

class CFileRead{
	std::vector<std::vector<std::string>> result;
public:
	std::vector<std::string> split(std::string str, std::string delim)
	{
		std::vector<std::string> result;
		int cutAt;
		while( (cutAt = str.find_first_of(delim)) != str.npos )
		{
			if(cutAt > 0)
			{
				result.push_back(str.substr(0, cutAt));
			}
			str = str.substr(cutAt + 1);
		}
		if(str.length() > 0)
		{
			result.push_back(str);
		}
		return result;
	}
	void FileRead(char *filename, char *delim){
		int hFile;
		hFile = FileRead_open(filename);
		
		char line[1000];
		std::string line_s;
		int count = 0;
		while(FileRead_eof(hFile) == 0){
			
			FileRead_gets(line, 1000, hFile);
			line_s = line;
			if(line_s == "")continue;
			result.resize(result.size() + 1);
			result[count] = split(line_s, delim);
			if(count > 90){
				int a;
				a=0;
			}

			count++;
		}
		FileRead_close(hFile);
	}
	int GetData(int row, int column){
		return std::stoi(result[row][column]);
		// return atoi(result[row][column].c_str());
	}
	float GetDataf(int row, int column){
		return (float)std::stof(result[row][column].c_str());
		// return (float)atof(result[row][column].c_str());
	}
	const char* GetDatac(int row, int column){
		return result[row][column].c_str();
	}
	std::string GetDatas(int row, int column){
		return result[row][column];
	}

	template<class Ty>
	Ty GetDatat(int row, int column)
	{
		std::basic_stringstream<TCHAR> ss;
		ss<<result[row][column];
		Ty x;
		ss>>x;
		return x;
	}

	int GetSizeRow(){ return result.size(); }
	int GetSizeColumn(int row){ return result[row].size(); }

};


