#pragma once
#if !defined(_INTERPRETER_H_)
#define _INTERPRETER_H_

#define MAX_CHAR_NUM 255
#define MAX_ATTR 32
#define MAX_INPUT_LENGTH 100
#define KEY_WORDS_NUM 16
#define OBJECT_NUM 3
#define WRONG_INST -2
#include <string>
#include <iostream>
using namespace std;

string KeyWords[KEY_WORDS_NUM] = { "create","drop","select","delete","insert","use","execfile","quit",\
								"into","values","from","where","unique","primary key"\
								};
string Object[OBJECT_NUM] = { "table","index","index" };
string VarType[] = { "int","char","float" };
string Compare[] = { "and","or","=","<>","<",">","<=",">=" };


class  Interpreter
{
public:
	 Interpreter();
	~ Interpreter();
	int interpreter();
	string ReadInput();
	string FindType(string SQL);
	void PrintError(int Error_Info);

	void CreateClause(string SQL, int start);
	void CreateDatabase(CString SQL, int start);
	//验证create table语句是否有效
	void CreateTable(CString SQL, int start);
	void CreateIndex(CString SQL, int start);
	void SelectClause(string SQL, int start);
	void InsertClause(string SQL, int start);
	void DropClause(string SQL, int start);
	void UseClause(string SQL, int start);
	void DeleteClause(string SQL, int start);
	void ExecfileClause(string SQL, int start);
	void HelpClause(string SQL, int start);
	void QuitClause(string SQL, int start);
private:
	int KeyType;
};

#endif
