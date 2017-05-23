/*
main函数只负责打印软件信息，并调用Interpreter类函数interpreter
*/


#include "interpreter.h"

void main()
{
	int Continue_Signal = 1;
	Interpreter ip;
	
	cout << endl;
	cout << "\t\t***********************************************" << endl;
	cout << "\t\t             Welcome to MiniSQL !" << endl;
	cout << "\t\t               Version (1.0)  " << endl;
	cout << "\t\t            " << endl;
	cout << "\t\t        copyright(2017) all right reserved !" << endl;
	cout << "\t\t***********************************************" << endl;
	cout << endl << endl;
	while (Continue_Signal)
	{
		cout << "MiniSQL-->> ";
		Continue_Signal = ip.interpreter();
	}
}