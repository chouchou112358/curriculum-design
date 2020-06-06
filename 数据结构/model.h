#ifndef _MODEL_H_
#define _MODEL_H_

typedef struct Route {//������·�ṹ
    int station;//ָ���վ��������
    int bus; //����������
    struct Route* next; //�����ͬ�ģ���һ������·��
}Route;

typedef struct Station
{
    char name[30];//վ����
    int route_num;
    Route* routes;//�Ӹ�վ�����������·�ߵ�����
}Station;

typedef struct Bus {//���幫����·
    char name[30];
    int start, end;
    int station_num;//����·���Ե��ĳ�վ��
    int *stations;
}Bus;

typedef struct BusMap
{
    Bus* buses;//������·����
    Station* stations;//վ������
    int station_num;//վ����
    int bus_num;//������·��
}BusMap;

#endif