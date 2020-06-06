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
        //输出界面
        system("cls");
        cout << "====公交管理系统====" << endl;
        cout << "1.查询公交线路" << endl;
        cout << "2.查询站点信息" << endl;
        cout << "3.查询公交线路" << endl;
        cout << "4.后台数据管理" << endl;
        cout << "0.退出" << endl;
        cout << "请输入操作编号(0~4):" << endl;
        char choice;
        fflush(stdin);//清除标准输入缓存区
        do {
            choice = getchar();
        } while (choice == '\n');
        if (!isdigit(choice) || choice - '0' > 4) {
            cout << choice;
            cout << "操作编号为 1~4,再见了您嘞！";
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
    cout << "请输入要查询的公交名：";
    cin >> bus;
    while (findBus(bus) == None) {
        cout << "\n                  就没有这辆车！";
        cout << "\nA.返回  B.重新输入  C.告诉我有什么车 请输入（a,b,c）\n";
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
        cout << "请输入要查询的公交名：";
        cin >> bus;
    }
    QueryBus(bus);
}

void function2()
{
    char station[20];
    char choice;
    cout << "请输入要查询的站点名：";
    cin >> station;
    while (findStation(station) == None) {
        cout << "\n     就没有这个站";
        cout << "\n  A.返回  B.重新输入  C.告诉我有那些站 请输入你的选择\n";
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
        cout << "请输入要查询的站点名：";
        cin >> station;;
   }
    QueryStation(station);
}

void function3()
{
    char begin[20],destination[20];
    int nbegin = 0, ndestination = 0;
    cout << "=====查询两个站点公交路线（最多换乘一次）=====" << endl
        << "请输入要查询的起点：";
    cin >> begin;
    nbegin = findStation(begin);
    if (nbegin == None) {
        cout << "站点不存在,请先查询站点";
        return;
    }
    cout << "请输入要查询的终点：";
    cin >> destination;
    ndestination = findStation(destination);
    if (ndestination == None) {
        cout << "站点不存在,请先查询站点";
        return;
    }
    QueryPath(nbegin, ndestination);

}

void function4()
{
    cout << "======管理数据菜单======\n"
        << "a. 添加公交\n"
        << "b. 删除公交\n"
        << "c. 修改公交\n"
        << "0. back"
        << "输入你的选项\n";
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
