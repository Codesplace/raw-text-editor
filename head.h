#pragma once
#ifndef _HEAD_H_ 
#define _HEAD_H_ 
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include <stack>
#include <string>
#include <iomanip>
#include <tchar.h>
using namespace std;
static const HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE); //ȡ��׼����豸��Ӧ�ľ��
static const HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);   //ȡ��׼�����豸��Ӧ�ľ��

/* ���������̬ */
#define CURSOR_VISIBLE_FULL					0	//�����ʾ��ȫ��ɫ��
#define CURSOR_VISIBLE_HALF					1	//�����ʾ�����ɫ��
#define CURSOR_VISIBLE_NORMAL				2	//�����ʾ�����ߣ�ȱʡΪ�˷�ʽ��
#define CURSOR_INVISIBLE					3	//��겻��ʾ

class Editor {
private:
	COORD pos; //��Ļ�ϵ�����1
	COORD loc; //��Ļ�ϵ�����2   
	string line;//���ټ�¼ÿһ�е��ı�
	CONSOLE_SCREEN_BUFFER_INFO csbi;//����̨��Ļ��������Ϣ                                                                                               x
	vector<string> text;//��¼ҳ���ϵ����е��ı�
	struct _operator{
		string str;
		int x;
		int y;
	};                   
	stack<_operator> history;                                            //������ʷջ,��¼ҳ����ʷ

	int  generateBadChar(char* b, int m, int* badchar);            //BM���������㷨1
	int  generateGS(char* b, int m, int* suffix, bool* prefix);    //BM���������㷨2
	int moveByGS(int j, int m, int* suffix, bool* prefix);         //BM���������㷨3
	int str_bm(char* a, int n, char* b, int m);                    //BM���������㷨4
	int BM_Search(string str);                                       //BM���������㷨5
	string GetText(int i, int j);                                  //BM���������㷨6

	char Normal_GetInput();                                        //��ͨ�����жϺ���
	char Insert_GetInput();                                        //���������жϺ���

	int OpenFile(string address);                                  //�ļ���������1
	int StoreFile(string address);                                 //�ļ���������2

	                                                               //��������
	int ShutDown();                                                //�˳�����
	int Del(COORD pos);                                            //ɾ������
	int Cancel();                                              //��������
	int cct_setconsoleborder(int set_cols, int set_lines, int set_buffer_cols, int set_buffer_lines);//��ʼ���ô��ڲ���
	int cct_cls(); 
	int Goto(const int X, const int Y);                       //��λ����1
	int GetPos(int& x, int& y);                               //��λ����2
	int setcursor(const int options);                         //���ù��״̬
	int _insert(char ch);                                      //����ģʽ���뺯��
public:
	Editor() {
		cct_setconsoleborder(80, 25, 80, 50);
		GetConsoleScreenBufferInfo(hOutput, &csbi);
		for (int i = 0; i < 50; i++) text.push_back(string(80, ' '));
	}

	int Normal_Pattern();                                                                                 //��ͨģʽ
	int Insert_Pattern();                                                                                 //����ģʽ
};
#endif
