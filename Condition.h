

#ifndef minisql_Condition_h
#define minisql_Condition_h
#include <string>
#include <sstream>
#include"Parameter.h"
using namespace std;

class Condition
{
    
public:


	string attributeName;
	string value;           // the value to be compared
	int operate;            // the type to be compared

    
	//initialization
	Condition(string a, string v, int o) {
			attributeName = a;
			value = v;
			operate = o;
	}
    
	bool Condition::ifRight(int content)
	{
		stringstream ss;
		ss << value;
		int myContent;
		ss >> myContent;

		switch (operate)
		{
		case OPERATOR_EQUAL:
			return content == myContent;
			break;
		case OPERATOR_NOT_EQUAL:
			return content != myContent;
			break;
		case OPERATOR_LESS:
			return content < myContent;
			break;
		case OPERATOR_MORE:
			return content > myContent;
			break;
		case OPERATOR_LESS_EQUAL:
			return content <= myContent;
			break;
		case OPERATOR_MORE_EQUAL:
			return content >= myContent;
			break;
		default:
			return false;
			break;
		}
	}

	bool Condition::ifRight(float content)
	{
		//turing string to float 
		stringstream ss;
		ss << value;
		float myContent;
		ss >> myContent;

		switch (operate)
		{
		case OPERATOR_EQUAL:
			return content == myContent;
			break;
		case OPERATOR_NOT_EQUAL:
			return content != myContent;
			break;
		case OPERATOR_LESS:
			return content < myContent;
			break;
		case OPERATOR_MORE:
			return content > myContent;
			break;
		case OPERATOR_LESS_EQUAL:
			return content <= myContent;
			break;
		case OPERATOR_MORE_EQUAL:
			return content >= myContent;
			break;
		default:
			return false;
			break;
		}
	}

	bool Condition::ifRight(string content)
	{
		string myContent = value;
		switch (operate)
		{
		case OPERATOR_EQUAL:
			return content == myContent;
			break;
		case OPERATOR_NOT_EQUAL:
			return content != myContent;
			break;
		case OPERATOR_LESS:
			return content < myContent;
			break;
		case OPERATOR_MORE:
			return content > myContent;
			break;
		case OPERATOR_LESS_EQUAL:
			return content <= myContent;
			break;
		case OPERATOR_MORE_EQUAL:
			return content >= myContent;
			break;
		default:
			return false;
			break;
		}
	}



};

#endif
