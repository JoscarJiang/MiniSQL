#ifndef minisql_Attribute_h
#define minisql_Attribute_h

#include <string>
#include <iostream>
#include "Parameter.h"
using namespace std;

class Attribute
{
public:
	Attribute() {};
	Attribute(string n, int t, bool i){
			name = n;
			type = t;
			ifUnique = i;
			index = "";
	}
	void print()
	{
		cout << "name: " << name << ";type: " << type << ";ifUnique: " << ifUnique << ";index: " << index << endl;
	}

	string indexNameGet() { return index; }
	string name;
	int type;                  //the type of the attribute,-1 represents float, 0 represents int, other positive integer represents char and the value is the number of char)
	bool ifUnique;
	string index;         // default value is "", representing no index
};

#endif
