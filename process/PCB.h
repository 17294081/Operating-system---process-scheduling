#ifndef PCB_H
#define PCB_H

#include <QString>
#include <vector>

struct PCB {
    QString name;    //进程名
    double arr_time;   //到达时间
    double ser_time;   //服务时间
    double need_arm;   //需要内存
    double rem_ser_time;  //仍需服务时间
    double end_time;   //结束时间
    double cyc_time = 0;   //周转时间
    double wt_time = 0;    //带权周转时间
    double ave_time = 0;   //平均周转时间
    double ave_t_time = 0; //平均带权周转时间
    double pri = -1;  //优先级
    char state;      //进程状态  执行——E（execute） 就绪——R(ready) 完成——F(finish) 阻塞——W(wait)
    double printer;  //打印机

    PCB();
};

bool com_needtime(PCB &a,PCB &b);
bool com_pri(PCB &a,PCB &b);

bool com_shorttime(PCB &a,PCB &b);
bool com_pri_sjf(PCB &a,PCB &b);

#endif // PCB_H
