#pragma once
#include"model.h"
#define None -1

int findBus(char* s);
int findStation(char* s);
void LoadMapData();//加载地图数据
bool loadRoute(int nBus, int nStart, int nEnd);//将Route信息加载到 Station 中
bool loadStationOfBus(int nBus);//通过Station 的Route 信息 完善 Bus 的 stations 字段
int nextStation(int nBus, int nstaition);


void QueryStation(char* pStation); 

void QueryBus(char* pBus);
void QueryPath(int nbegin, int ndestination);
void showOne(int nbegin, int ndestination, int nbus);
bool hasPath(int nbegin, int ndestination, int* enbus);