#ifndef _MODEL_H_
#define _MODEL_H_

typedef struct Route {//定义线路结构
    int station;//指向的站点索引号
    int bus; //公交索引号
    struct Route* next; //起点相同的，下一条下行路线
}Route;

typedef struct Station
{
    char name[30];//站点名
    int route_num;
    Route* routes;//从该站点出发的所有路线的链域
}Station;

typedef struct Bus {//定义公交线路
    char name[30];
    int start, end;
    int station_num;//该线路可以到的车站数
    int *stations;
}Bus;

typedef struct BusMap
{
    Bus* buses;//公交线路数组
    Station* stations;//站点数组
    int station_num;//站点数
    int bus_num;//公交线路数
}BusMap;

#endif