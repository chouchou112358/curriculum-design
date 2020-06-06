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
	int allstation = g_sMap.station_num;//վ���������
	Bus temp;
	char name[30];
	cout << "���빫������:";
	cin >> temp.name;
	while (findBus(temp.name) != None) {
		cout << "\n          �ù����Ѿ����� \n��������";
		cin >> temp.name;
	}
	cout << "������ù�������վ������";
	do {
		cin >> temp.station_num;
	} while (temp.station_num <= 1);
	temp.stations = (int*)malloc(sizeof(int) * temp.station_num);
	string* extro_stations = new string[temp.station_num];

	for (int i = 0; i < temp.station_num; i++) {
		nstation = None;
		cout << (i == 0 ? "�������վ��" : "��һվ��");
		cin >> name;
		nstation = findStation(name);
		int j;
		for (j = 0; j < i; j++) {//�ж��Ƿ�֮ǰ�½���վ
			if (temp.stations[j] == nstation)
				break;
		}
		if (j == i && nstation == None) {//�������ڸ�վ
			cout << "\n ��վ������ ����(A) ��������(B) �˳�(C)\n";
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
	cout << "������Ҫ���ĵĹ���������:";
	cin >> name;
	nbus = findBus(name);
	if (nbus == None) {
		cout << "�����ڸù�����";
		return;
	}
	QueryBus(name);
	cout << endl << "�޸����֣�A������(B):";
	do {
		cin >> choice;
	} while (choice == '\n');
	choice = tolower(choice);
	if (choice == 'a') {
		cout << "����ù����µ�����:";
		cin >> name;
		if (findBus(name) != None) {
			cout << "�ù����Ѿ�����";
			return;
		}
		strcpy(g_sMap.buses[nbus].name, name);
	}
}

void deleteBus()
{
	char name[30];
	int nbus = None;
	cout << "������Ҫɾ���Ĺ���������:"; 
	cin >> name;
	nbus = findBus(name);
	if (nbus == None) {
		cout << "�����ڸù�����";
		return;
	}
	for (int i = 0; i < g_sMap.buses[nbus].station_num-1; i++) {//��ÿһ��վ ɾ���͸ù������йص�route
		int nstation = g_sMap.buses[nbus].stations[i];//�ù����� �� iվ �� ���
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
	for (int i = 0; i < g_sMap.buses[nbus].station_num; i++) {//�� վ��������ɾ����վ��
		bool endStation = false;
		int nstation = g_sMap.buses[nbus].stations[i];//�ù����� �� iվ �� ���
		for (int j = 0; j < g_sMap.bus_num; j++) {//��վ��Ϊ����·�ߵ��յ�վʱ route δ�� ������ɾ��
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
	for (int i = nbus; i < g_sMap.bus_num-1; i++) {//ɾ��������Ϣ
		g_sMap.buses[i] = g_sMap.buses[i + 1];
		for (int j = 0; j < g_sMap.buses[i].station_num; j++) {//�� i+1 ��·�� ����rote -1  
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
		cout << "δ����,�ļ���ʧ��";
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
