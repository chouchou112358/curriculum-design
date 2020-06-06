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
	// ����վ����Ϣ
	fp >> g_sMap.station_num;
	g_sMap.stations = (Station*)malloc(sizeof(Station) * g_sMap.station_num);
	if (g_sMap.stations == nullptr) {
		cout << "�ڴ治��";
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

	// ���ع�����·��Ϣ
	fp >> g_sMap.bus_num;
	g_sMap.buses = (Bus*)malloc(sizeof(Bus) * g_sMap.bus_num);
	if(g_sMap.buses == nullptr){
		cout << "�ڴ治��";
		system("pause");
		exit(1);
	}
	for (int i = 0; i < g_sMap.bus_num; i++) {
		fp >> g_sMap.buses[i].name;
		g_sMap.buses[i].station_num = 1;//Ĭ����һ�����  վ�������� route �� +1
		fp >> g_sMap.buses[i].start;
		fp >> g_sMap.buses[i].end;
	}
	fp.close();
	fp.clear();
	fp.open("routes.txt", ios::in);
	int route_num;
	fp >> route_num;
	// ���·����Ϣ
	for (int i = 0; i < route_num; i++) {
		int nbus, nstart, nend;
		fp >> nbus >> nstart >> nend;
		loadRoute(nbus,nstart,nend);
	}
	fp.close();
	fp.clear();
	//���乫����·��Ϣ
	for (int i = 0; i < g_sMap.bus_num; i++) {
		g_sMap.buses[i].stations = (int*)(malloc(sizeof(int) * g_sMap.buses[i].station_num));
		g_sMap.buses[i].station_num = 0;
		loadStationOfBus(i);
	}
}

bool loadRoute(int nBus, int nStart, int nEnd)
{
	//�������ĳ�������
	Station* pStStation = &g_sMap.stations[nStart];
	Route* pStRoute = pStStation->routes;
	while (pStRoute != NULL && pStRoute->next != NULL) {
		//�жϸñ��Ƿ��Ѵ��ڣ�����Ѿ����ڣ��򲻲���
		if (pStRoute->bus == nBus && pStRoute->station == nEnd)
			return false;
		pStRoute = pStRoute->next;
	}
	//�����µ���·
	Route* pNewRoute = (Route*)malloc(sizeof(Route)*1);
	if (pNewRoute == NULL)
		return false;
	pNewRoute->bus = nBus;
	pNewRoute->station = nEnd;
	pNewRoute->next = NULL;
	 //������ʵ�����һ����
	if (pStRoute == NULL)
		pStStation->routes = pNewRoute;
	else
		pStRoute->next = pNewRoute;
	pStStation->route_num++;
	g_sMap.buses[nBus].station_num++;//ÿ���һ��·�� Ϊ������һ����վ��
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
		<< "[" << pStation << "]  " << "����" << g_sMap.stations[nStation].route_num+(pex-extro) << "��������" << endl;
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
		<< "[" << pBus << "]" << "�� :" << g_sMap.stations[b.start].name << ": ����" << " :" << g_sMap.stations[b.end].name << " :,"
		<< "����" << b.station_num << "��վ��." << endl << endl;
	for (int i = 0; i < b.station_num; i++) {
		cout << g_sMap.stations[b.stations[i]].name;
		if (i != b.station_num - 1) {
			cout << "-->";
		}

	}
	cout << endl;
}

void QueryPath(int nbegin, int ndestination) {
	int * const enbus = (int *)(malloc(sizeof(int)*g_sMap.bus_num));//��ͨ���ĵ�·
	for (int i = 0; i < g_sMap.bus_num; i++) {
		enbus[i] = None;
	}
	if (hasPath(nbegin, ndestination, nullptr) == true) {
		hasPath(nbegin, ndestination, enbus);
		for (int i = 0; enbus[i] != None; i++) {
			int nBus = enbus[i];
			cout << "����" << g_sMap.buses[nBus].name << "������";
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
					cout << "����" << g_sMap.buses[i].name << "������";
					showOne(nbegin, g_sMap.buses[i].stations[j], i);
					cout << "      ����" << g_sMap.buses[enbus[0]].name << "������";
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
		cout << "���ȵ���hasPath����ȷ������·�ߺ��ٵ��ô˺���\n";
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
bool hasPath(int nbegin, int ndestination, int *enbus) {//��enbus Ϊ��ʱ ���� �Ƿ���·�� ��Ϊ��ʱ �� enbus �����м�¼ȫ���Ŀɴﳵ��
	bool flag=false, exist=false;
	for (int i = 0; i < g_sMap.bus_num; i++) {
		if (g_sMap.buses[i].station_num == 0) {
			return false;
		}
		flag = false;
		for (int j = 0; j < g_sMap.buses[i].station_num; j++) {
			if (nbegin == g_sMap.buses[i].stations[j]) {//�����վ�Ƿ����
				flag = true;
				continue;
			}
			if (flag && ndestination == g_sMap.buses[i].stations[j]) {//��������ʱ�ҵ��յ�
				if (enbus == nullptr) {//����Ҫ���泵����Ϣʱ ֱ�ӷ����Ƿ��ҵ�
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

