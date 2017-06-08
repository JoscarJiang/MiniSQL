#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H
#include "structs.h"
#include "macro.h"
#include "filemanip.h"
#include <unistd.h>
void DoAppend(char* fn);  // ��д���������ļ����ӵ��ļ��� 
int GetReadBuffer(char* FileName, int line); // �������ݴ����16��readbuffer�е���һ�� �����ѵ��ļ�β�����򷵻�-1 
void Append(char* fn, char* data, int size); // ��fn�ļ�ĩβ����data��ʼ����size���ֽ�
int Read(char* fn, int line, char* bufin); // ��fn�ļ���line�п�ʼ�����ݣ�����bufin�У�����ֵ�Ƕ��˶����ֽڣ����BUFFER_SIZE����-1��ʾ�Ѿ������� 
void BufsInit();
void Write(char* fn,int line, char* data, int size); // ��Ҫ�ĳɴ���ʽ�ϣ�������write buffer 
void DeleteLine(char* fn, int line, int unit, int howmany);
void DeleteLines(char* fn, IntList il, int unit);
void ReWrite(char* fn, char* data, int size);

#endif

