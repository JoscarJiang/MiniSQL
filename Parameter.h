#pragma once
#ifndef Parameter_H
#define Parameter_H

/*= 0
 <> 1
 <  2
 >  3
 <= 4
 >= 5
*/
#define OPERATOR_EQUAL 0
#define OPERATOR_NOT_EQUAL 1 
#define OPERATOR_LESS  2
#define OPERATOR_MORE  3
#define OPERATOR_LESS_EQUAL 4 
#define OPERATOR_MORE_EQUAL 5

#define TYPE_FLOAT -1
#define TYPE_INT 0


#define UNKNOWN_FILE 8
#define TABLE_FILE 9
#define INDEX_FILE 10

#include <set>
#include <string>
using namespace std;

static set<string> Protected{ "and" ,"char" ,"create" ,"delete" ,"drop" ,"execfile" ,"float" ,"from", "index", "insert", "int", "into", "key", "on", "primary", "quit", "select", "table", "unique", "values", "where" };//±£Áô×Ö


#endif