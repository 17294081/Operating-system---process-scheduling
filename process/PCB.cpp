#include "PCB.h"
#include <QVector>

PCB::PCB(){

}

bool com_needtime(PCB &a,PCB &b){
    return a.rem_ser_time < b.rem_ser_time;
}

bool com_pri(PCB &a,PCB &b){
    if(a.rem_ser_time == b.rem_ser_time){
        return a.pri>b.pri;
    }else {
        return a.rem_ser_time<b.rem_ser_time;
    }
}

bool com_pri_sjf(PCB &a,PCB &b){
    if(a.ser_time == b.ser_time){
        return a.pri>b.pri;
    }else {
        return a.ser_time<b.ser_time;
    }
}

bool com_shorttime(PCB &a,PCB &b){
    return a.ser_time<b.ser_time;
}
