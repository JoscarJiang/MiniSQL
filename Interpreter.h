#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <string>
#include <vector>
#include "API.h"

using namespace std;
class Interpreter {
public:

	Interpreter() {}

	string getWord(string s, int &st);
	int interpreter(string s);

	int insert(string s, int &tmp);
	string toLowerString(string s);
	int createTable(string s, int &tmp);
	bool isLegalName(string s);
	int createIndex(string s, int &tmp);
	int select(string s, int &tmp);
	int drop(string s, int &tmp);
	int Delete(string s, int &tmp);
	API* ap;
	string fileName;
	~Interpreter() {}


};

#endif