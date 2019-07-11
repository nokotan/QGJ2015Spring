#pragma once


#include <string>
#include <vector>
#include <sstream>
#include <DxLib.h>

// using namespace std;


// FileReader.h ���F�j
//
//�@��ɋ�؂蕶���ŕ\�`���ɕۑ����ꂽ�e�L�X�g�t�@�C����ǂݍ��ނ��߂̃N���X�B
//	�N���X�̒m���͂Ȃ��Ƃ��g���܂�
//	
//	�y�g�����z
//	�@CFileRead�^�̕ϐ���錾�@
//	�A�����o�֐���FireRead���Ăяo���i��1�����̓t�@�C���p�X�A��2�����͋�؂蕶���j
//	�B�����o�֐���GetData���g���ēǂݍ��񂾃f�[�^���擾����i�����ŉ��s�ډ��Ԗڂ̃f�[�^�Ȃ̂����w�肷��j
//	
//�@�t���̃T���v�������ĂȂ�ƂȂ��g������c�����Ă��炦���
//
//�@��������l��������
//  std��usgin namespace���Ă���̂ł������Ȃ��Ƃ����C���ǂ����B

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


