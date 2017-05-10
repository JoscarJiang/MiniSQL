#if 0
要求：根据说明处理用户的SQL语句、脚本执行、退出程序。即助教下发的指导文件中的interpreter部分。 
不要求判定是否存在表、索引、重名等，只要求与SQL格式不符的错误提示即可。
根据用户输入，生成相应结构、调用恰当的函数（调用即可，不需实现。这些函数还没写完，请先简单替代）：

void CreateIndex(char* in, char* tn, char* an, char force); // 依次的含义是：index name, table name, attribute name, force为0即可 

void CreateTable(MetaTable T);

void DropIndex(char* in);

void DropTable(char* tn);

void SelectFromWhere(char* tn, Cond cond); // Cond 含义是 condition 

void DeleteFromWhere(char* tn, Cond cond); 

void InsertIntoValues(char* tn, Values v);


其中涉及到的结构：

struct cond{
	char an[MAX_ATTR_NAME]; // 列名
	char op;
	// op 约定如下： 
	// 000 =
	// 001 <=
	// 011 <
	// 100 >
	// 110 >=
	// 111 <>
	struct values* value; // 此结构中的next可以不使用 
	struct cond* next; // 下一个条件，遇到一个and就分配一个 
};

struct values{
	char type; // 00 int, 01 float, 11 char(n)
	char size; // int = float = 4; char = n
	void* data;
	struct values* next;
};

struct attr{
	char pk; // 是否主键 
	char unique; // 是否独一无二 
	char type; // 数据类型 
	unsigned char size; // 数据大小 
	char name[MAX_ATTR_NAME];
};

struct metaTable{
	char tn[MAX_TABLE_NAME];
	int valid; // 一共多少个属性 
	Attr a[MAX_ATTR];
};

typedef struct cond* Cond;
typedef struct values* Values;
typedef struct attr* Attr;
typedef struct metaTable* MetaTable; 

#endif

