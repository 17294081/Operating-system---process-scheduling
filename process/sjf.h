#ifndef SJF_H
#define SJF_H

#include <QWidget>
#include <vector>
#include <QTimer>
#include "PCB.h"
#include "ARM.h"
using namespace std;

namespace Ui {
class SJF;
}

class SJF : public QWidget
{
    Q_OBJECT

public:
    explicit SJF(QWidget *parent = nullptr);

    QTimer *t1;
    int t = 0;  //显示时钟
    double N = 0;  //进程数
    int MAX_SIZE;  //内存大小
    int MAX_PRINTER;  //最大打印机数量
    int HAVE_PRINTER; //先有打印机数量
    int reback_timer = 0; //用于计时强制唤醒阻塞进程
    double cyc_sum = 0;  //平均周转
    double wt_cyc_sum = 0;  //平均带权周转时间
    vector<PCB> ready;
    vector<PCB> creat;
    vector<PCB> wait;
    vector<PCB> finish;
    vector<PCB> run;
    ARM A;
    void set_os();
    void set_table();
    void creat_pushButton();
    void begin_pushbutton();
    void stop_pushbutton();
    void continue_pushbutton();
    void sleep_pushbutton();
    void wakeup_pushbutton();
    void reback_pushbutton();

    void pcbsc();

    void update_creat();
    void update_ready();
    void update_run();
    void update_wait();
    void update_finish();
    void update_memory();
    void update_ending();

    ~SJF();
signals:
    void sjf_signal();

private:
    Ui::SJF *ui;
};


#endif // SJF_H
