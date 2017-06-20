#ifndef BufferManager_H
#define BufferManager_H
#include "Minisql.h"
#include <stdio.h>

class BufferManager
{
private:
	fileNode *fileHead;
	fileNode file_pool[MAX_FILE_NUM];
	blockNode block_pool[MAX_BLOCK_NUM];
	int total_block;//正在使用的块
	int total_file;
	static const int BLOCK_SIZE = 4096;
	//函数部分
	void init_block(blockNode & block);
	void init_file(fileNode & file);
	blockNode* getBlock(fileNode* file, blockNode* position, bool pin = false);
	void writtenBackToDiskAll();
	void writtenBackToDisk(const char* fileName, blockNode* block);
	void clean_dirty(blockNode & block);
	void addBlocktoList(fileNode* file,blockNode* position, blockNode* btmp);
	blockNode* replaceBlock();
	size_t getUsingSize(blockNode* block);
public:
	BufferManager();
	~BufferManager();
	void delete_fileNode(const char* fileName);
	fileNode* getFile(const char* fileName, bool pin = false);
	void set_dirty(blockNode& block);
	void set_pin(blockNode& block, bool pin);
	void set_pin(fileNode& file, bool pin);//重载
	void set_usingSize(blockNode& block, size_t usage);
	size_t get_usingSize(blockNode& block);
	char* get_content(blockNode& block);
	static int getBlockSize();

	blockNode* getNextBlock(fileNode* file, blockNode* block);
	blockNode* getBlockHead(fileNode* file);
	blockNode* getBlockByOffset(fileNode* file, int offsetNumber);
};
#endif


