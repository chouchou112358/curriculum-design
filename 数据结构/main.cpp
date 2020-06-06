#include<iostream>
#include"model.h"
#include "map.h"
#include "manage.h"
using namespace std;

void function0();
void function1();
void function2();
void function3();
void function4();

BusMap g_sMap;


int main() {
    void (*p[5])() = { function0,function1,function2,function3,function4 };
    LoadMapData();
    while (true) {
        //�������
        system("cls");
        cout << "====��������ϵͳ====" << endl;
        cout << "1.��ѯ������·" << endl;
        cout << "2.��ѯվ����Ϣ" << endl;
        cout << "3.��ѯ������·" << endl;
        cout << "4.��̨���ݹ���" << endl;
        cout << "0.�˳�" << endl;
        cout << "������������(0~4):" << endl;
        char choice;
        fflush(stdin);//�����׼���뻺����
        do {
            choice = getchar();
        } while (choice == '\n');
        if (!isdigit(choice) || choice - '0' > 4) {
            cout << choice;
            cout << "�������Ϊ 1~4,�ټ������ϣ�";
            //system("shutdown /s /t 5");
        }
        else
            p[choice - '0']();
        system("pause");
    }
    return 0;
}

void function0()
{
    for (int i = 0; i < g_sMap.bus_num; i++) {
        free(g_sMap.buses[i].stations);
    }
    free(g_sMap.buses);
    for (int i = 0; i < g_sMap.station_num; i++) {
        Route* pre;
        while (g_sMap.stations[i].routes != NULL) {
            pre = g_sMap.stations[i].routes;
            g_sMap.stations[i].routes = g_sMap.stations[i].routes->next;
            free(pre);

        }
    }
    free(g_sMap.stations);
    exit(0);
}

void function1()
{
    char bus[20];
    char choice;
    cout << "������Ҫ��ѯ�Ĺ�������";
    cin >> bus;
    while (findBus(bus) == None) {
        cout << "\n                  ��û����������";
        cout << "\nA.����  B.��������  C.��������ʲô�� �����루a,b,c��\n";
        do {
            choice = getchar();
        } while (choice == '\n');
        switch (choice)
        {
        case'a': case'A':
            return;
        case 'c': 
            for (int i = 0; i < g_sMap.bus_num; i++) {
                cout << g_sMap.buses[i].name << "  ";
            }
            cout << endl;
        default:
            break;
        }
        cout << "������Ҫ��ѯ�Ĺ�������";
        cin >> bus;
    }
    QueryBus(bus);
}

void function2()
{
    char station[20];
    char choice;
    cout << "������Ҫ��ѯ��վ������";
    cin >> station;
    while (findStation(station) == None) {
        cout << "\n     ��û�����վ";
        cout << "\n  A.����  B.��������  C.����������Щվ ���������ѡ��\n";
        do {
            choice = getchar();
        } while (choice == '\n');
        switch (choice)
        {
        case'a': case'A':
            return;
        case 'c':
            for (int i = 0; i < g_sMap.station_num; i++) {
                cout << g_sMap.stations[i].name<< "  ";
            }
            cout << endl;
        default:
            break;
        }
        cout << "������Ҫ��ѯ��վ������";
        cin >> station;;
   }
    QueryStation(station);
}

void function3()
{
    char begin[20],destination[20];
    int nbegin = 0, ndestination = 0;
    cout << "=====��ѯ����վ�㹫��·�ߣ���໻��һ�Σ�=====" << endl
        << "������Ҫ��ѯ����㣺";
    cin >> begin;
    nbegin = findStation(begin);
    if (nbegin == None) {
        cout << "վ�㲻����,���Ȳ�ѯվ��";
        return;
    }
    cout << "������Ҫ��ѯ���յ㣺";
    cin >> destination;
    ndestination = findStation(destination);
    if (ndestination == None) {
        cout << "վ�㲻����,���Ȳ�ѯվ��";
        return;
    }
    QueryPath(nbegin, ndestination);

}

void function4()
{
    cout << "======�������ݲ˵�======\n"
        << "a. ��ӹ���\n"
        << "b. ɾ������\n"
        << "c. �޸Ĺ���\n"
        << "0. back"
        << "�������ѡ��\n";
    char choice;
    do {
        cin >> choice;
    } while (choice == '\n');
    choice = tolower(choice);
    if (choice > 'f' || choice<'a') {
        return;
    }
    else {
        void (*p[3])() = { addBus,deleteBus,updateBus };
        p[choice - 'a']();
    }
}
