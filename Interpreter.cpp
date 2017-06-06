/*
Interpreter模块直接与用户交互，主要实现以下功能：
1.程序流程控制，即“启动并初始化  ‘接收命令、处理命令、显示命令结果’循环  退出”流程。
2.接收并解释用户输入的命令，生成命令的内部数据结构表示，同时检查命令的语法正确性和语义正确性，
  对正确的命令调用API层提供的函数执行并显示执行结果，对不正确的命令显示错误信息。
*/

#include<cctype>
#include<algorithm>
#include "interpreter.h"

Interpreter::Interpreter()
{
}

Interpreter::~Interpreter()
{
}

int Interpreter::interpreter() {
	string SQL = ReadInput();
	int start = 0;
	for (; SQL[0] == ' ';) {
		SQL = SQL.substr(1, SQL.length - 1);
	}
	string key = FindType(SQL);
	switch (KeyType) {
		case 0:start = 6; CreateClause(SQL, start); break;
		case 1:start = 4; DropClause(SQL, start); break;
		case 2:start = 6; SelectClause(SQL, start); break;
		case 3:start = 6; DeleteClause(SQL, start); break;
		case 4:start = 6; InsertClause(SQL, start); break;
		case 5:start = 4; UseClause(SQL, start); break;
		case 6:start = 4; ExecfileClause(SQL, start); break;
		case 7:start = 4; QuitClause(SQL, start); break;
		case 8:start = 4; HelpClause(SQL, start); break;
		default: {
			cout << "syntax error:" << " " << key << "---is not a valid key word!" << endl;
		}
	}
	return 1;
}
/*
function:
*/
string Interpreter::ReadInput()
{
	string SQL;
	string temp;
	bool finish = false;
	while (!finish)
	{
		cin >> temp;
		SQL = SQL + " " + temp;
		for (; SQL[SQL.length - 1] == ' ';) {
			SQL = SQL.substr(0, SQL.length - 1);
		}
		if (SQL[SQL.length - 1] == ';')
		{
			//don't know -1 or -2
			SQL.insert(SQL.length - 2, " ");
			finish = true;
		}
	}
	transform(SQL.begin(), SQL.end(), SQL.begin(), tolower);
	return SQL;
}
/*
function
*/

string Interpreter::FindType(string SQL) {
	
	int end = SQL.find_first_of(' ');
	string instr = SQL.substr(0, end);
	//quit
	int if_no = 1;
	int i = 0;
	for (; i < KEY_WORDS_NUM && if_no != 0; i++) {
		//return 0 ->same
		if_no = instr.compare(KeyWords[i]);
	}
	if (i >= KEY_WORDS_NUM) {
		KeyType = -2;//Wrong signal
	}
	else {
		KeyType = i - 1;
	}
	return instr;

}
void Interpreter::PrintError(int Error_Info) {

}

void Interpreter::CreateClause(string SQL, int start) {
	string second_key = SQL.substr(start, SQL.length - start);
	for (; second_key[second_key.length - 1] == ' ';) {
		second_key = second_key.substr(0, second_key.length - 1);
	}
	int end = second_key.find_first_of(" ");
	if (end ==-1) //if no finding


	second_key = second_key.substr(0, end);
	int i = 0, if_no =1, object_type;
	for (; i < OBJECT_NUM && if_no != 0; i++) {
		//return 0 ->same
		if_no = second_key.compare(Object[i]);
	}
	if (i >= KEY_WORDS_NUM) {
		object_type = -2;//Wrong signal
	}
	else {
		object_type = i - 1;
	}

	if (object_type == -2) {

	}
	else {
		switch (object_type)
		{
		case 0: create_database(SQL, start); break;
		default:
			break;
		}
	}
}