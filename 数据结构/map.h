#pragma once
#include"model.h"
#define None -1

int findBus(char* s);
int findStation(char* s);
void LoadMapData();//���ص�ͼ����
bool loadRoute(int nBus, int nStart, int nEnd);//��Route��Ϣ���ص� Station ��
bool loadStationOfBus(int nBus);//ͨ��Station ��Route ��Ϣ ���� Bus �� stations �ֶ�
int nextStation(int nBus, int nstaition);


void QueryStation(char* pStation); 

void QueryBus(char* pBus);
void QueryPath(int nbegin, int ndestination);
void showOne(int nbegin, int ndestination, int nbus);
bool hasPath(int nbegin, int ndestination, int* enbus);