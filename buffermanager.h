#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H
#include "structs.h"
#include "macro.h"
#include "filemanip.h"
#include <unistd.h>
void DoAppend(char* fn);  // 将写缓冲区的文件附加到文件后 
int GetReadBuffer(char* FileName, int line); // 返回数据存进了16个readbuffer中的哪一个 ，若已到文件尾部，则返回-1 
void Append(char* fn, char* data, int size); // 在fn文件末尾，从data开始增附size个字节
int Read(char* fn, int line, char* bufin); // 在fn文件第line行开始读数据，读到bufin中，返回值是读了多少字节，最多BUFFER_SIZE，若-1表示已经无内容 
void BufsInit();
void Write(char* fn,int line, char* data, int size); // 需要改成从形式上，经过了write buffer 
void DeleteLine(char* fn, int line, int unit, int howmany);
void DeleteLines(char* fn, IntList il, int unit);
void ReWrite(char* fn, char* data, int size);

#endif

