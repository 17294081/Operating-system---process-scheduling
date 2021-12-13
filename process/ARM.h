#ifndef ARM_H
#define ARM_H

#include<QString>
#include<QVector>
#include<QList>
#include"PCB.h"

struct Arm{
public:
    int address;
    int size;
    QString id = "NONE";
    QString state = "FREE";
    Arm * next;
    Arm * front;
};

class ARM{
public:
    Arm *head = new Arm ;
    Arm *last = new Arm;
    bool add(PCB &p,int a);
    void cyc(QString name);
    ARM();
};

#endif // ARM_H
