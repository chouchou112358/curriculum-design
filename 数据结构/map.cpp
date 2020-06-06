#define _CRT_SECURE_NO_WARNINGS
#include "map.h"
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

extern BusMap g_sMap;


int findBus(char* p)
{
	for (int i = 0; i < g_sMap.bus_num; i++) {
		if (strcmp(p, g_sMap.buses[i].name) == 0) {
			return i;
			}
	}
	return None;
}

int findStation(char* s)
{
	for (int i = 0; i < g_sMap.station_num; i++) {
		if (strcmp(s, g_sMap.stations[i].name) == 0) {
			return i;
		}
	}
	return None;
}

void LoadMapData()
{
	ifstream fp("stations.txt");
	if (!fp.is_open()) {
		cout << "Error opening file";
		system("pause");
		exit(1);
	}
	// 加载站点信息
	fp >> g_sMap.station_num;
	g_sMap.stations = (Station*)malloc(sizeof(Station) * g_sMap.station_num);
	if (g_sMap.stations == nullptr) {
		cout << "内存不足";
		system("pause");
		exit(1);
	}
	for (int i = 0; i < g_sMap.station_num; i++) {
		fp >>g_sMap.stations[i].name;
		g_sMap.stations[i].route_num = 0;
		g_sMap.stations[i].routes = NULL;
	}
	fp.close();
	fp.clear();
	fp.open("buses.txt", ios::in);

	// 加载公交线路信息
	fp >> g_sMap.bus_num;
	g_sMap.buses = (Bus*)malloc(sizeof(Bus) * g_sMap.bus_num);
	if(g_sMap.buses == nullptr){
		cout << "内存不足";
		system("pause");
		exit(1);
	}
	for (int i = 0; i < g_sMap.bus_num; i++) {
		fp >> g_sMap.buses[i].name;
		g_sMap.buses[i].station_num = 1;//默认有一个起点  站点数等于 route 数 +1
		fp >> g_sMap.buses[i].start;
		fp >> g_sMap.buses[i].end;
	}
	fp.close();
	fp.clear();
	fp.open("routes.txt", ios::in);
	int route_num;
	fp >> route_num;
	// 添加路段信息
	for (int i = 0; i < route_num; i++) {
		int nbus, nstart, nend;
		fp >> nbus >> nstart >> nend;
		loadRoute(nbus,nstart,nend);
	}
	fp.close();
	fp.clear();
	//补充公交线路信息
	for (int i = 0; i < g_sMap.bus_num; i++) {
		g_sMap.buses[i].stations = (int*)(malloc(sizeof(int) * g_sMap.buses[i].station_num));
		g_sMap.buses[i].station_num = 0;
		loadStationOfBus(i);
	}
}

bool loadRoute(int nBus, int nStart, int nEnd)
{
	//插入起点的出边链域
	Station* pStStation = &g_sMap.stations[nStart];
	Route* pStRoute = pStStation->routes;
	while (pStRoute != NULL && pStRoute->next != NULL) {
		//判断该边是否已存在，如果已经存在，则不插入
		if (pStRoute->bus == nBus && pStRoute->station == nEnd)
			return false;
		pStRoute = pStRoute->next;
	}
	//创建新的线路
	Route* pNewRoute = (Route*)malloc(sizeof(Route)*1);
	if (pNewRoute == NULL)
		return false;
	pNewRoute->bus = nBus;
	pNewRoute->station = nEnd;
	pNewRoute->next = NULL;
	 //若是其实顶点第一条边
	if (pStRoute == NULL)
		pStStation->routes = pNewRoute;
	else
		pStRoute->next = pNewRoute;
	pStStation->route_num++;
	g_sMap.buses[nBus].station_num++;//每添加一个路线 为车增加一个车站数
	return true;
}
bool loadStationOfBus(int nBus) {
	Bus b = g_sMap.buses[nBus];
	int next = None;
	do {
		next = nextStation(nBus, next);
		g_sMap.buses[nBus].stations[g_sMap.buses[nBus].station_num++] = next;
	} while (next != b.end);
	return true;
}
int nextStation(int nBus, int nstaition) {
	if (nstaition == None) {
		return g_sMap.buses[nBus].start;
	}
	Route* p = g_sMap.stations[nstaition].routes;
	while (p != NULL) {
		if (p->bus == nBus) {
			return p->station;
		}
		p = p->next;
	}
	return None;
}

void QueryStation(char* pStation)
{
	int* const extro = (int*)malloc(sizeof(int) * g_sMap.bus_num);
	for (int i = 0; i < g_sMap.bus_num; i++) {
		extro[i] = None;
	}
	int* pex = extro;
	int nStation = findStation(pStation);
	if (nStation == None) {
		exit(1);
	}
	for (int i = 0; i < g_sMap.bus_num; i++) {
		if (nStation == g_sMap.buses[i].end) {
			*pex = i;
			pex++;
		}
	}
	cout << "------------------------------" << endl
		<< "[" << pStation << "]  " << "共有" << g_sMap.stations[nStation].route_num+(pex-extro) << "辆车经过" << endl;
	Route* p = g_sMap.stations[nStation].routes;
	for (int i = 0; i < g_sMap.bus_num; i++) {
		if (extro[i] == None) {
			break;
		}
		cout << g_sMap.buses[extro[i]].name << "   ";
	}
	while (p != NULL) {
			cout << g_sMap.buses[p->bus].name << "   ";
		p = p->next;
	}
	cout << endl;
}
void QueryBus(char* pBus)
{
	int nBus = findBus(pBus);
	Bus b = g_sMap.buses[nBus];
	cout << "-----------------------------" << endl
		<< "[" << pBus << "]" << "从 :" << g_sMap.stations[b.start].name << ": 开往" << " :" << g_sMap.stations[b.end].name << " :,"
		<< "共经" << b.station_num << "个站点." << endl << endl;
	for (int i = 0; i < b.station_num; i++) {
		cout << g_sMap.stations[b.stations[i]].name;
		if (i != b.station_num - 1) {
			cout << "-->";
		}

	}
	cout << endl;
}

void QueryPath(int nbegin, int ndestination) {
	int * const enbus = (int *)(malloc(sizeof(int)*g_sMap.bus_num));//可通过的道路
	for (int i = 0; i < g_sMap.bus_num; i++) {
		enbus[i] = None;
	}
	if (hasPath(nbegin, ndestination, nullptr) == true) {
		hasPath(nbegin, ndestination, enbus);
		for (int i = 0; enbus[i] != None; i++) {
			int nBus = enbus[i];
			cout << "乘坐" << g_sMap.buses[nBus].name << "公交车";
			showOne(nbegin, ndestination, nBus);
			cout << endl;
		}
	}
	else
	{
		bool flag = false;
		for (int i = 0; i < g_sMap.bus_num; i++) {
			flag = false;
			for (int j = 0; j < g_sMap.buses[i].station_num; j++) {
				if (g_sMap.buses[i].stations[j] == nbegin) {
					flag = true;
				}
				if (flag && hasPath(g_sMap.buses[i].stations[j], ndestination, enbus)) {
					cout << "乘坐" << g_sMap.buses[i].name << "公交车";
					showOne(nbegin, g_sMap.buses[i].stations[j], i);
					cout << "      换乘" << g_sMap.buses[enbus[0]].name << "公交车";
					showOne(g_sMap.buses[i].stations[j], ndestination, enbus[0]);
					cout << endl;
				}
			}
		}
	}
	free(enbus);
}
void showOne(int nbegin, int ndestination, int nbus) {
	int begin_index = None;
	for (int i = 0; i < g_sMap.buses[nbus].station_num; i++) {
		if (g_sMap.buses[nbus].stations[i] == nbegin) {
			begin_index = i;
			break;
		}
	}
	if (begin_index == None) {
		cout << "请先调用hasPath函数确定存在路线后，再调用此函数\n";
	}
	for (int i = begin_index;i<g_sMap.buses[nbus].station_num; i++) {
		if (g_sMap.buses[nbus].stations[i] == ndestination) {
			cout << g_sMap.stations[g_sMap.buses[nbus].stations[i]].name;
			break;
		}
		else
			cout << g_sMap.stations[g_sMap.buses[nbus].stations[i]].name << "-->";
	}
}
bool hasPath(int nbegin, int ndestination, int *enbus) {//当enbus 为空时 返回 是否有路线 不为空时 在 enbus 数组中记录全部的可达车次
	bool flag=false, exist=false;
	for (int i = 0; i < g_sMap.bus_num; i++) {
		if (g_sMap.buses[i].station_num == 0) {
			return false;
		}
		flag = false;
		for (int j = 0; j < g_sMap.buses[i].station_num; j++) {
			if (nbegin == g_sMap.buses[i].stations[j]) {//找起点站是否存在
				flag = true;
				continue;
			}
			if (flag && ndestination == g_sMap.buses[i].stations[j]) {//当起点存在时找到终点
				if (enbus == nullptr) {//不需要保存车次信息时 直接返回是否找到
					return true;
				}
				*enbus = i;
				enbus++;
				exist = true;
				break;
			}
		}
	}
	return exist;
}

