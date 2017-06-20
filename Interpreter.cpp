#include "Interpreter.h"
#include "Condition.h"
#include "Attribute.h"
#include <string>
#include <string.h>
#include <iostream>


class SyntaxException {};
/*
���ܣ���ÿ����ĸ�ĳ�Сд
*/
string Interpreter::toLowerString(string s)
{
	for (auto &c : s)
	{
		c = tolower(c);
	}
	return s;
}

/*
���ܣ��鿴�Ƿ��ǺϷ��ı�����
*/
bool Interpreter::isLegalName(string s)
{
	if (s.empty())
	{
		cerr << "Syntax Error: Name is Empty!" << endl;
		return 0;
	}
	s = toLowerString(s);

	if (s[0] != '_' && !isalpha(s[0])) //��ʼ���ǡ�_�� ���� �ַ��򱨴�
	{
		cerr << "Syntax Error: Illegal name at Start!" << endl;
		return 0;
	}

	for (auto c : s) //����s�е�ÿ���ַ�
	{
		if (c != '_' && !isalnum(c)) //���зǷ��ַ���ʱ��->
		{
			cerr << "Syntax Error: Illegal Character!" << endl;
			return 0;
		}

	}
	if (Protected.find(s) != Protected.end())
	{
		cerr << "Syntax Error: Illegal Name�� Use Protected Name!" << endl;
		return 0;
	}
	return 1;
}

/*
�������ļ�飬�Լ�����API�Ĵ���table����
*/
int Interpreter::createTable(string s, int &tmp)
{
	string primaryKey, tableName, word;
	word = getWord(s, tmp);
	if (!isLegalName(word))
	{
		return 0;
	}


	///////////////////*�õ�Tablename*/
	tableName = word;


	////////////////////////�õ�(
	word = getWord(s, tmp);

	if (!word.empty() && word != "(")
	{
		cerr << "Syntax Error: Missing\"(\"" << endl;
		return 0;
	}


	////////////////////////�õ���������
	word = getWord(s, tmp);

	std::vector<Attribute> attributeVector;
	while (!word.empty() && toLowerString(word) != "primary"&& word != ")")
	{
		if (!isLegalName(word))
		{
			return 0;
		}
		string attributeName = word;
		int type = 0;
		bool isUnique = false;
		word = toLowerString(getWord(s, tmp));
		if (word == "int")
			type = 0;
		else if (word == "float")
			type = -1;
		else if (word == "char")/*�õ���������*/
		{
			word = getWord(s, tmp);
			if (word != "(")
			{
				cerr << "Syntax Error: unknown data type" << endl;
				return 0;
			}
			word = getWord(s, tmp);
			istringstream convert(word);
			if (!(convert >> type))
			{
				cerr << "Syntax error: illegal number in char()" << endl;
				return 0;
			}
			word = getWord(s, tmp);
			if (word != ")")
			{
				cerr << "Syntax Error: unknown data type" << endl;
				return 0;
			}
		}
		else
		{
			cerr << "Syntax Error: unknown or missing data type!" << endl;
			return 0;
		}


		word = getWord(s, tmp);/*���ǲ���unique����*/
		if (toLowerString(word) == "unique")
		{
			isUnique = true;
			word = getWord(s, tmp);
		}

		Attribute attr(attributeName, type, isUnique);/*����һ��������*/
		attributeVector.push_back(attr);/*������vector��*/
		if (word != ",")
		{
			if (word != ")") 
			{
				cerr << "Syntax Error for ,!" << endl;
				return 0;
			}
			else
				break;
		}

		word = getWord(s, tmp);//�����һ������ȷ���ǲ����µ�����
	}

	int primaryKeyLocation = 0;

	if (toLowerString(word) == "primary")   //���ǲ��� primary key
	{
		word = getWord(s, tmp);
		if (toLowerString(word) != "key")
		{
			cerr << "Syntax Error :Missing \"key\" after 'primary'!" << endl;
			return 0;
		}
		else
		{
			word = getWord(s, tmp);
			if (word == "(")
			{
				word = getWord(s, tmp);
				primaryKey = word;//���primarykey������
				int i = 0;
				for (i = 0; i<attributeVector.size(); i++)
				{
					if (primaryKey == attributeVector[i].name)//�ҵ�primarykey�����Բ���ǳ�unique
					{
						attributeVector[i].ifUnique = true;
						break;
					}

				}
				if (i == attributeVector.size())//û�ҵ�
				{
					cerr << "Syntax Error: primaryKey does not exist in attributes " << endl;
					return 0;
				}
				primaryKeyLocation = i;//primarykey��λ��
				word = getWord(s, tmp);
				if (word != ")")
				{
					cerr << "Syntax Error: Missing ')' !" << endl;
					return 0;
				}
			}
			else
			{
				cerr << "Syntax Error: Missing '(' !" << endl;
				return 0;
			}
			word = getWord(s, tmp);
			if (word != ")")
			{
				cerr << "Syntax Error: Missing ')' !" << endl;
				return 0;
			}
		}
	}
	else if (word.empty())
	{
		cerr << "Syntax Error: Missing ')' !" << endl;
		return 0;
	}

	ap->tableCreate(tableName, &attributeVector, primaryKey, primaryKeyLocation);//����api�ĺ���������
	return 1;
}
/*
�������ļ�飬�Լ�����API�Ĵ���Index����
*/
int Interpreter::createIndex(string s, int &tmp)
{
	{
		string indexName, tableName, attributeName, word;
		word = getWord(s, tmp);
		if (!word.empty())          //����������
			indexName = word;
		else
		{
			cerr << "Syntax Error : Index name is wrong !" << endl;
			return 0;
		}

		word = getWord(s, tmp);//��ȡon
		try
		{
			if (word != "on")
				throw SyntaxException();
			word = getWord(s, tmp);//��ȡ����
			if (word.empty())
				throw SyntaxException();
			tableName = word;
			word = getWord(s, tmp);//(
			if (word != "(")
				throw SyntaxException();
			word = getWord(s, tmp);//��Ҫ����������������
			if (word.empty())
				throw SyntaxException();
			attributeName = word;
			word = getWord(s, tmp);//)
			if (word != ")")
				throw SyntaxException();
			ap->indexCreate(indexName, tableName, attributeName);
			return 1;
		}
		catch (SyntaxException&)
		{
			cerr << "Syntax Error in Creating Index!" << endl;
			return 0;
		}
	}
}
/*
�������ļ�飬�Լ��õ����ҵ����������ݣ�Ȼ�����API�Ĳ��Һ���
*/
int Interpreter::select(string s, int &tmp)
{
	vector<string> attrSelected;
	string tableName, word;
	word = getWord(s, tmp);/*��ȡ*/
	if (word != "*")
	{
		while (word != "from")
		{
			attrSelected.push_back(word);
			word = getWord(s, tmp);
		}
	}
	else
	{
		word = getWord(s, tmp);/*��ȡfrom*/
	}
	if (word != "from")
	{
		cerr << "Syntax Error : Missing From!" << endl;
		return 0;
	}

	word = getWord(s, tmp);/*��ȡ����*/
	if (!word.empty())
		tableName = word;
	else
	{
		cerr << "Syntax Error : Table name is wrong!" << endl;
		return 0;
	}

	// where�������
	word = getWord(s, tmp);
	if (word.empty())   // û������
	{
		if (attrSelected.size() == 0)
		{
			ap->recordShow(tableName);
		}
		else
			ap->recordShow(tableName, &attrSelected);
		return 1;
	}
	else if (word == "where")
	{
		string attributeName = "";
		string value = "";
		int operate = OPERATOR_EQUAL;
		vector<Condition> conditionVector;
		word = getWord(s, tmp);   
		while (1) {
			try {
				if (word.empty())
					throw SyntaxException();
				attributeName = word;
				word = getWord(s, tmp);
				if (word == "<=")
					operate = OPERATOR_LESS_EQUAL;
				else if (word == ">=")
					operate = OPERATOR_MORE_EQUAL;
				else if (word == "<")
					operate = OPERATOR_LESS;
				else if (word == ">")
					operate = OPERATOR_MORE;
				else if (word == "=")
					operate = OPERATOR_EQUAL;
				else if (word == "<>")
					operate = OPERATOR_NOT_EQUAL;
				else
					throw SyntaxException();
				word = getWord(s, tmp);
				if (word.empty()) 
					throw SyntaxException();
				value = word;
				Condition c(attributeName, value, operate);
				conditionVector.push_back(c);
				word = getWord(s, tmp);
				if (word.empty()) // û��������
					break;
				if (word != "and")
					throw SyntaxException();
				word = getWord(s, tmp);
			}
			catch (SyntaxException) {
				cerr << "Syntax Error in Where ! " << endl;
				return 0;
			}
		}
		if (attrSelected.size() == 0)
			ap->recordShow(tableName, NULL, &conditionVector);
		else
			ap->recordShow(tableName, &attrSelected, &conditionVector);

		return 1;
	}
}

/*
������һ���ؼ�����table����index��������API�ĺ���
*/
int Interpreter::drop(string s, int &tmp)
{
	string word;
	word = getWord(s, tmp);

	if (word == "table")
	{
		word = getWord(s, tmp);
		if (!word.empty())
		{
			ap->tableDrop(word);/*����drop����������table����*/
			return 1;
		}
		else
		{
			cerr << "Syntax Error: Table name is wrong!" << endl;
			return 1;
		}
	}
	else if (word == "index")
	{
		word = getWord(s, tmp);
		if (!word.empty())
		{
			ap->indexDrop(word);/*����drop����������index����*/
			return 1;
		}
		else
		{
			cerr << "Syntax Error: Index name is wrong!" << endl;
			return 1;
		}
	}
	else
	{
		cerr << "Unknown Function!" << endl;
		return 0;
	}
}
/*
�������ļ�飬����API�Ĳ��뺯��
*/
int Interpreter::insert(string s, int &tmp)
{
	string tableName, word;
	vector<string> valueVector;
	word = getWord(s, tmp);
	try {
		if (word != "into")
			throw SyntaxException();
		word = getWord(s, tmp);
		if (word.empty())
			throw SyntaxException();
		tableName = word;
		word = getWord(s, tmp);
		if (word != "values")
			throw SyntaxException();
		word = getWord(s, tmp);
		if (word != "(")
			throw SyntaxException();
		word = getWord(s, tmp);
		while (!word.empty() && word != ")")
		{
			valueVector.push_back(word);
			word = getWord(s, tmp);
			if (word == ",")  // bug here
				word = getWord(s, tmp);
		}
		if (word != ")")
			throw SyntaxException();
	}
	catch (SyntaxException&) {
		cerr << "Syntax Error in Insert!" << endl;
		return 0;
	}
	ap->recordInsert(tableName, &valueVector);
	return 1;
}

/*
ɾ�����ļ�飬�Լ��õ�ɾ�������������ݣ�Ȼ�����API��ɾ������
*/
int Interpreter::Delete(string s, int &tmp)
{
	string tableName, word;
	word = getWord(s, tmp);
	if (word != "from")
	{
		cerr << "Syntax Error: Missing from!" << endl;
		return 0;
	}

	word = getWord(s, tmp);
	if (!word.empty())
		tableName = word;
	else
	{
		cerr << "Syntax Error: Table name is wrong!" << endl;
		return 0;
	}

	//ɾ��������
	word = getWord(s, tmp);
	if (word.empty())   // û������
	{
		ap->recordDelete(tableName);
		return 1;
	}
	else if (word == "where")
	{
		string attributeName, value;
		int operate = OPERATOR_EQUAL;
		std::vector<Condition> conditionVector;
		word = getWord(s, tmp);    
		while (1) {/*��ȡ��ͬ������*/
			try {
				if (word.empty())
					throw SyntaxException();
				attributeName = word;
				word = getWord(s, tmp);
				if (word == "<=")
					operate = OPERATOR_LESS_EQUAL;
				else if (word == ">=")
					operate = OPERATOR_MORE_EQUAL;
				else if (word == "<")
					operate = OPERATOR_LESS;
				else if (word == ">")
					operate = OPERATOR_MORE;
				else if (word == "=")
					operate = OPERATOR_EQUAL;
				else if (word == "<>")
					operate = OPERATOR_NOT_EQUAL;
				else
					throw SyntaxException();
				word = getWord(s, tmp);
				if (word.empty()) // no condition
					throw SyntaxException();
				value = word;
				word = getWord(s, tmp);
				Condition c(attributeName, value, operate);
				conditionVector.push_back(c);
				if (word.empty()) // no condition
					break;
				if (word != "and")
					throw SyntaxException();
				word = getWord(s, tmp);
			}
			catch (SyntaxException&) {
				cerr << "Syntax Error in where !" << endl;
				return 0;
			}
		}
		ap->recordDelete(tableName, &conditionVector);
		return 1;
	}
}

/*
Core Function:ͨ��getWord�õ���һ��(��)�����֣��ֱ������Ӧ�Ĵ�����
0���д���
1���ɹ�
2�����ļ�
587���˳�
*/
int Interpreter::interpreter(string s)
{
	int tmp = 0;
	string word, lowerWord;

	word = getWord(s, tmp);
	lowerWord = toLowerString(word);
	if (lowerWord == "create")
	{
		word = getWord(s, tmp);
		lowerWord = toLowerString(word);
		if (lowerWord == "table")
		{
			return createTable(s, tmp);
		}
		else if (lowerWord == "index")
		{
			return createIndex(s, tmp);
		}
		else
		{
			cerr << " Syntax Error: create error!" << endl;
			return 0;
		}
	}
	else if (lowerWord == "select")
	{
		return select(s, tmp);
	}
	else if (lowerWord == "drop")
	{
		return drop(s, tmp);
	}
	else if (lowerWord == "delete")
	{
		return Delete(s, tmp);
	}
	else if (lowerWord == "insert")
	{
		return insert(s, tmp);
	}
	else if (lowerWord == "execfile")
	{
		fileName = getWord(s, tmp);
		cout << "try to open " << fileName << "..." << endl;
		return 2;
	}
	else if (lowerWord == "quit")
	{
		return 587;
	}
	else if (word != "")
	{
		cerr << "Error: \"" << word << "\" Not Found!" << endl;
		return 0;
	}
}

/*
�Կո�Ϊ����ȡ��������
��������ţ���ȡ�����ڵ���
*/
string Interpreter::getWord(string s, int &tmp)
{
	string word;
	int start;

	/*ȥ���ո��м�*/
	while (isspace(s[tmp]) && s[tmp] != 0)
	{
		tmp++;
	}
	start = tmp;

	switch (s[tmp])
	{
	case'(':case',':case')':
	{
		tmp++;
		word = s.substr(start, tmp - start);
	}/*��ȡ���źͶ���*/
	break;
	case '\'':
	{
		tmp++;
		while (s[tmp] != '\''&&s[tmp] != 0)
		{
			tmp++;
		}
		if (s[tmp] == '\'')
		{
			start++;
			word = s.substr(start, tmp - start);
			tmp++;
		}
	}/*��ȡ�����ڲ�������*/
	break;
	default:
	{
		while (!isspace(s[tmp])
			&& s[tmp] != '('
			&&s[tmp] != '\n'
			&&s[tmp] != 0
			&& s[tmp] != ')'
			&&s[tmp] != ',')
			tmp++;
		if (start != tmp)
			word = s.substr(start, tmp - start);
	}/*��ȡ��ͨ����*/
	break;
	}
	return word;
}



typedef struct sknode *skNode;
typedef struct store *skStore;

//the structure of a node of skip list
struct sknode {
	int value;
	int level;
	skNode next;
	skNode down;
};

//to store route by which found target
struct store {
	skNode found;
	skStore next;
};

class SkipList {

public:
	SkipList() {
		// initiallt top -> head
		now_level = 1;
		num = 0;
		skNode head = create_node(-1, now_level);
		head->down = NULL;
		head->next = NULL;
		top = create_node(MIN_INT, now_level);// update the level when inserting a node
		top->down = NULL;
		top->next = head;

		T = new struct store;
		T->found = NULL;
		T->next = NULL;
		srand((unsigned)time(0));
	}
	~SkipList()
	{
		skNode temp = top->next;
		delete top;
		top = temp;

		for (; top != NULL;) {
			skNode t = top->down;
			for (; top != NULL;) {
				skNode temp = top;
				top = top->next;
				delete temp;
			}
			top = t;
		}

	}
	int find(int x);//return 1 when x is in the skip list, otherwise return 0
	void insert_node(int x);//insert x to skip list. if x exists, it will do nothing
	void delete_node(int x);//delete x from skip list. if x doesn't exist, it will do nothing
	void print(skNode s);

private:
	double myrandom(int start, int end);
	skNode create_node(int value, int level);
	int random_level();//return a random level
	skNode search_node(int x);//search the node that has value which is no larger than x
	skNode top;//this is the head of skip list
	skStore T;//this is the head of the route
	int now_level;//to store the level that current skip list has
	int num;// the number of nodes  in the list
	int MAX_INT = ((unsigned)(-1) >> 1);
	int MIN_INT = (~MAX_INT);
};
/*
fuction: create a node
*/
skNode SkipList::create_node(int value, int level) {
	skNode new_node = new struct sknode;
	new_node->value = value;
	new_node->level = level;//build a new node
	return new_node;
}

/*
fuction: find x and return 1 if exist otherwise return 0
*/
int SkipList::find(int x) {
	skNode p;
	p = search_node(x);
	//NULL -> No such Node that has smaller than or equal to x
	//p->value == x -> Find x, return 1
	//otherwise return 0
	if (p == NULL || p == top->next) {
		return 0;
	}
	else if (p->value == x) {
		return 1;
	}
	return 0;
}

/*
fuction: search for x and return the according pointer
*/
skNode SkipList::search_node(int x) {
	skNode p;
	skStore temp;
	//from head to the end
	p = top->next;
	//initial T
	T->found = NULL;
	T->next = NULL;
	//p == NULL means no node
	if (p->next == NULL && p->down == NULL) {
		temp = new struct store;
		temp->found = top->next;
		temp->next = T->next;
		T->next = temp;
		return NULL;
	}
	while (p->value <= x) {
		//case1: get next in the same level
		if ((p->next != NULL) && (p->next->value <= x))
			p = p->next;
		//case2: go to a lower level to find x
		else if (p->down != NULL) {
			temp = new struct store;
			temp->found = p;
			temp->next = T->next;
			T->next = temp;
			p = p->down;
		}
		//case3: return pointer that has value no larger than x
		else {
			temp = new struct store;
			temp->found = p;
			temp->next = T->next;
			T->next = temp;
			return p;
		}
	}
	return NULL;
}

/*
fuction: insert x and return nothing
*/
void SkipList::insert_node(int x) {
	//print(top->next);
	int new_level = 0, i = 0;
	skNode p = NULL, new_node = NULL, temp = NULL;
	skStore s = NULL, t = NULL;
	//get random level starting from 1 
	new_level = random_level();
	//do nothing when x has existed
	if (find(x)) {
		cout << x << " existed" << endl;
		return;
	}
	//get the pointer that is no larger than x 
	s = T->next;
	//new_level <= now_level means no adding a level

	int level = new_level > now_level ? now_level : new_level;

	for (i = 1; i <= level; i++) {
		p = s->found;//the node after which new node need to insert
		new_node = create_node(x, i);
		new_node->down = temp;//point to lower level
		new_node->next = p->next;
		p->next = new_node;
		temp = new_node;//temp records this node
		t = s->next;
		delete s;
		s = t;
	}

	if (new_level > now_level) {
		for (; i <= new_level; i++) {
			p = create_node(-1, i);
			new_node = create_node(x, i);
			p->down = top->next;//down to point to top
			p->next = new_node;//link the new top and new node
			new_node->down = temp;
			new_node->next = NULL;
			temp = new_node;
			top->next = p;//refresh the top
		}

		now_level = new_level;
	}
	num++;
}

/*
function: return a random level with 50% probability jumping up
*/
int SkipList::random_level() {

	int r = rand();
	int level = 1;
	//cout << "r:" << r << endl;
	while (r % 2) {//get random value 0 or 1
		level++;
		//cout << "level:" << level << endl;
		r = rand();
		//cout << "r:" << r << endl;
	}
	return level;
}


/*
fuction: delete x
*/
void SkipList::delete_node(int x) {
	//print(top->next);
	int i;
	skNode p, temp;
	skStore s, t;
	if (!find(x)) {
		cout << x << " doesn't existed" << endl;
		return;//do nothing when x doesn't exist
	}
	search_node(x - 1);//find the node before target and closest to it
	s = T->next;

	for (i = 1; i <= now_level; i++) {
		p = s->found;
		if ((p->next == NULL) || (p->next->value != x))
			break;//break when this level doesn't have x
		temp = p->next;
		p->next = p->next->next;//delete the target
		delete temp;
		t = s->next;
		delete s;
		s = t;
	}
	num--;
}


void SkipList::print(skNode s) {
	skNode t = s;
	skNode t2 = s;
	for (; t2; t2 = t2->down) {
		t = t2;
		for (; t; t = t->next) {
			printf("%d ", t->value);
		}
		printf("\n");
	}
}

