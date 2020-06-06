#define _CRT_SECURE_NO_WARNINGS
#include"manage.h"
#include "map.h"
#include<iostream>
#include<fstream>
#include<string>

using namespace std;

extern BusMap g_sMap;

void addBus()
{
	int nstation;
	int allstation = g_sMap.station_num;//站点的总数量
	Bus temp;
	char name[30];
	cout << "输入公交车名:";
	cin >> temp.name;
	while (findBus(temp.name) != None) {
		cout << "\n          该公交已经存在 \n重新输入";
		cin >> temp.name;
	}
	cout << "请输入该公交车的站点数：";
	do {
		cin >> temp.station_num;
	} while (temp.station_num <= 1);
	temp.stations = (int*)malloc(sizeof(int) * temp.station_num);
	string* extro_stations = new string[temp.station_num];

	for (int i = 0; i < temp.station_num; i++) {
		nstation = None;
		cout << (i == 0 ? "输入起点站：" : "下一站：");
		cin >> name;
		nstation = findStation(name);
		int j;
		for (j = 0; j < i; j++) {//判断是否之前新建的站
			if (temp.stations[j] == nstation)
				break;
		}
		if (j == i && nstation == None) {//若不存在该站
			cout << "\n 该站不存在 创建(A) 重新输入(B) 退出(C)\n";
			char choice;
			do {
				choice = getchar();
			} while (choice == '\n');
			choice = tolower(choice);
			if (choice == 'a') {
				temp.stations[i] = allstation;
				extro_stations[allstation-g_sMap.station_num]=name;
				allstation++;
			}
			else if (choice == 'b') {
				i--;
				continue;
			}
			else {
				return;
			}
		}
		else {
			temp.stations[i] = nstation;
		}
	}
	temp.start = temp.stations[0];
	temp.end = temp.stations[temp.station_num - 1];

	void * newptr = realloc(g_sMap.stations, sizeof(Station) * allstation);
	if (newptr==nullptr) {
		cout << "Error realloc memory";
		return;
	}
	g_sMap.stations = (Station *)newptr;

	for (int i = g_sMap.station_num; i < allstation; i++) {
		strcpy(g_sMap.stations[i].name, extro_stations[i - g_sMap.station_num].data()); 
		g_sMap.stations[i].route_num = 0;
		g_sMap.stations[i].routes = NULL;
	}
	g_sMap.station_num = allstation;
	newptr = realloc(g_sMap.buses, sizeof(Bus) * (g_sMap.bus_num+1));
	if(newptr==nullptr){
		cout << "Error realloc memory";
		return;
	}
	g_sMap.buses = (Bus *)newptr;
	g_sMap.buses[g_sMap.bus_num] = temp;
	g_sMap.bus_num++;

	for (int i = 0; i < temp.station_num-1; i++) {
		loadRoute(g_sMap.bus_num - 1 , temp.stations[i], temp.stations[i+1]);
	}
	g_sMap.buses[g_sMap.bus_num - 1].station_num = temp.station_num;
	
	saveData();
}

void updateBus()
{
	char name[30];
	char choice;
	int nbus = None;
	cout << "请输入要更改的公交车名称:";
	cin >> name;
	nbus = findBus(name);
	if (nbus == None) {
		cout << "不存在该公交车";
		return;
	}
	QueryBus(name);
	cout << endl << "修改名字（A）返回(B):";
	do {
		cin >> choice;
	} while (choice == '\n');
	choice = tolower(choice);
	if (choice == 'a') {
		cout << "输入该公交新的名字:";
		cin >> name;
		if (findBus(name) != None) {
			cout << "该公交已经存在";
			return;
		}
		strcpy(g_sMap.buses[nbus].name, name);
	}
}

void deleteBus()
{
	char name[30];
	int nbus = None;
	cout << "请输入要删除的公交车名称:"; 
	cin >> name;
	nbus = findBus(name);
	if (nbus == None) {
		cout << "不存在该公交车";
		return;
	}
	for (int i = 0; i < g_sMap.buses[nbus].station_num-1; i++) {//对每一个站 删除和该公交车有关的route
		int nstation = g_sMap.buses[nbus].stations[i];//该公交车 第 i站 的 序号
		Route* proute = g_sMap.stations[nstation].routes;
		if (proute != NULL) {
			if (proute->bus == nbus) {
				g_sMap.stations[nstation].routes = proute->next;
				g_sMap.stations[nstation].route_num--;
				free(proute);
			}
			else {
				while (proute->next != NULL) {
					if (proute->next->bus == nbus) {
						Route* q = proute->next;
						proute->next = proute->next->next;
						g_sMap.stations[nstation].route_num--;
						free(q);
						break;
					}
					proute = proute->next;
				}
			}
		}
		else
		{
			return;
		}

	}
	for (int i = 0; i < g_sMap.buses[nbus].station_num; i++) {//从 站点数据中删除空站点
		bool endStation = false;
		int nstation = g_sMap.buses[nbus].stations[i];//该公交车 第 i站 的 序号
		for (int j = 0; j < g_sMap.bus_num; j++) {//该站仅为其他路线的终点站时 route 未空 但不能删除
			if (g_sMap.buses[j].end == nstation && j != nbus) {
				endStation = true;
				break;
			}
		}
		if (g_sMap.stations[nstation].route_num == 0 && !endStation) {
			for (int j = nstation; j < g_sMap.station_num-1; j++) {
				g_sMap.stations[j] = g_sMap.stations[j + 1];
			}
			g_sMap.station_num--;
		}
	}

	free(g_sMap.buses[nbus].stations);
	for (int i = nbus; i < g_sMap.bus_num-1; i++) {//删除车辆信息
		g_sMap.buses[i] = g_sMap.buses[i + 1];
		for (int j = 0; j < g_sMap.buses[i].station_num; j++) {//将 i+1 线路的 所有rote -1  
			int* sta = g_sMap.buses[i].stations;
			Route* r = g_sMap.stations[sta[j]].routes;
			while (r != NULL) {
				if (r->bus == i + 1) {
					r->bus = i;
				}
				r = r->next;
			}
		}
	}
	g_sMap.bus_num--;
	saveData();
}

void saveData()
{
	fstream fp("stations.txt",ios::out|ios::trunc);
	if (!fp.good()) {
		cout << "未保存,文件打开失败";
		return;
	}
	fp << g_sMap.station_num<<endl;

	int route_sum = 0;
	for (int i = 0; i < g_sMap.station_num; i++) {
		fp << g_sMap.stations[i].name << endl;
		route_sum += g_sMap.stations[i].route_num;
	}
	fp.close();
	fp.clear();

	fp.open("buses.txt", ios::out|ios::trunc);
	fp << g_sMap.bus_num << endl;
	for (int i = 0; i < g_sMap.bus_num;i++) {
		fp << g_sMap.buses[i].name << ' ' << g_sMap.buses[i].start << ' ' << g_sMap.buses[i].end <<endl;
	}
	fp.close();
	fp.clear();

	fp.open("routes.txt", ios::out | ios::trunc);
	fp << route_sum << endl;
	for (int i = 0; i < g_sMap.station_num; i++) {
		Route* p = g_sMap.stations[i].routes;
		while (p!= NULL)
		{
			fp << p->bus << ' ' << i << ' '<< p->station << endl;
			p = p->next;
		}
	}
	fp.close();
	fp.clear();
}
