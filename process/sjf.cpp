#include "sjf.h"
#include "ui_sjf.h"
#include "ARM.h"
#include "PCB.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QString>
#include <QTimer>
#include <QMessageBox>
#include <iostream>
#include <vector>
using namespace std;


int ji1 = 0;
SJF::SJF(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SJF)
{
    ui->setupUi(this);

    connect(ui->back, &QPushButton::released,[=](){emit sjf_signal();});    //返回按钮

    connect(ui->pri_ture, &QRadioButton::clicked,[=](){ui->pri_edit->setEnabled(true);});    //启用优先级
    connect(ui->pri_false, &QRadioButton::clicked,[=](){ui->pri_edit->setEnabled(false);});  //禁用优先级

    //禁止部分编辑
    ui->pri_false->setChecked(true);
    ui->pri_edit->setEnabled(false);
    ui->stop_run->setEnabled(false);
    ui->continue_run->setEnabled(false);
    ui->add->setEnabled(false);
    ui->begin->setEnabled(false);

    set_table();  //表设置

    connect(ui->set_os,&QPushButton::released,[=](){set_os();});             //系统设置
    connect(ui->add,&QPushButton::released,[=](){creat_pushButton();});      //添加进程
    connect(ui->begin,&QPushButton::released,[=](){begin_pushbutton();});    //开始按钮
    connect(ui->stop_run,&QPushButton::released,[=](){stop_pushbutton();});  //暂停按钮
    connect(ui->continue_run,&QPushButton::released,[=](){continue_pushbutton();});  //继续按钮
    connect(ui->sleep,&QPushButton::released,[=](){sleep_pushbutton();});    //阻塞按钮
    connect(ui->wakeup,&QPushButton::released,[=](){wakeup_pushbutton();});  //唤醒按钮
    connect(ui->reback,&QPushButton::released,[=](){reback_pushbutton();});  //重置按钮
}

void SJF::set_table(){
    //执行进程表设置
    ui->run_table->setColumnCount(5);
    ui->run_table->setRowCount(1);
    //ui->run_table->horizontalHeader()->setStretchLastSection(true);  //列自动顶满格
    ui->run_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  //使列完全填充并平分
    ui->run_table->verticalHeader()->setVisible(false);  //不显示列号
    QStringList header1;
    header1<<"进程名"<<"到达时间"<<"服务时间"<<"剩余时间"<<"优先级";
    ui->run_table->setHorizontalHeaderLabels(header1);
    ui->run_table->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置不可修改
    ui->run_table->setSelectionMode(QAbstractItemView::NoSelection);  //设置不可选择

    //创建队列表
    ui->creat_table->setColumnCount(6);
    ui->creat_table->setRowCount(40);
    ui->creat_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->creat_table->verticalHeader()->setVisible(false);
    QStringList header2;
    header2<<"进程名"<<"到达时间"<<"服务时间"<<"内存"<<"打印机"<<"优先级";
    ui->creat_table->setHorizontalHeaderLabels(header2);
    ui->creat_table->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置不可修改
    ui->creat_table->setSelectionMode(QAbstractItemView::NoSelection);  //设置不可选择

    //就绪队列表
    ui->ready_table->setColumnCount(7);
    ui->ready_table->setRowCount(10);
    ui->ready_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->ready_table->verticalHeader()->setVisible(false);
    QStringList header3;
    header3<<"进程名"<<"到达时间"<<"服务时间"<<"内存"<<"打印机"<<"剩余时间"<<"优先级";
    ui->ready_table->setHorizontalHeaderLabels(header3);
    ui->ready_table->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置不可修改
    ui->ready_table->setSelectionMode(QAbstractItemView::NoSelection);  //设置不可选择

    //阻塞队列表
    ui->wait_table->setColumnCount(7);
    ui->wait_table->setRowCount(30);
    ui->wait_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->wait_table->verticalHeader()->setVisible(false);
    QStringList header4;
    header4<<"进程名"<<"到达时间"<<"服务时间"<<"内存"<<"打印机"<<"剩余时间"<<"优先级";
    ui->wait_table->setHorizontalHeaderLabels(header4);
    ui->wait_table->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置不可修改
    ui->wait_table->setSelectionMode(QAbstractItemView::NoSelection);  //设置不可选择

    //完成队列表
    ui->finish_table->setColumnCount(7);
    ui->finish_table->setRowCount(30);
    ui->finish_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->finish_table->verticalHeader()->setVisible(false);
    QStringList header_finish;
    header_finish<<"进程名"<<"到达时间"<<"服务时间"<<"内存"<<"打印机"<<"结束时间"<<"优先级";
    ui->finish_table->setHorizontalHeaderLabels(header_finish);
    ui->finish_table->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置不可修改
    ui->finish_table->setSelectionMode(QAbstractItemView::NoSelection);  //设置不可选择

    //内存显示
    ui->arm_table->setColumnCount(4);
    ui->arm_table->setRowCount(30);
    ui->arm_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->arm_table->verticalHeader()->setVisible(false);
    QStringList header5;
    header5<<"始址"<<"大小"<<"进程"<<"状态";
    ui->arm_table->setHorizontalHeaderLabels(header5);
    ui->arm_table->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置不可修改
    ui->arm_table->setSelectionMode(QAbstractItemView::NoSelection);  //设置不可选择;

    //运行结果
    ui->end_table->setColumnCount(8);
    ui->end_table->setRowCount(40);
    //ui->end_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->end_table->verticalHeader()->setVisible(false);
    QStringList header_end;
    header_end<<"进程名"<<"到达时间"<<"服务时间"<<"结束时间"<<"周转时间"<<"带权周转时间"<<"平均周转时间"<<"平均带权周转时间";
    ui->end_table->setHorizontalHeaderLabels(header_end);
    ui->end_table->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置不可修改
    ui->end_table->setSelectionMode(QAbstractItemView::NoSelection);  //设置不可选择
}
//各显示表初始化设置

void SJF::pcbsc(){
    if(creat.size() == 0 && ready.size() == 0 && wait.size() == 0){
        t1->stop();
        ui->run_table->clear();
        update_run();
        ui->end_table->clear();
        update_ending();
        QMessageBox::warning(NULL,"完成","已无进程!请添加");
        ui->continue_run->setEnabled(true);
        ui->stop_run->setEnabled(false);
        ui->reback->setEnabled(true);
    }else if(creat.size() == 0 && ready.size() == 0 && wait.size() != 0){
        if(reback_timer == 0){
            QMessageBox::warning(NULL,"警告","所有进程都已阻塞!将在5s后自动唤醒一进程!");
        }
        reback_timer++;
        if(reback_timer == 5){
            reback_timer = 0;
            wakeup_pushbutton();
        }
    }else if(creat.size() != 0 && ready.size() == 0 && wait.size() != 0){
        if(reback_timer == 0){
            QMessageBox::warning(NULL,"警告","先有进程都已阻塞!将等待新进程创建并将在10s后自动唤醒一进程!");
        }
        reback_timer++;
        if(reback_timer == 5){
            reback_timer = 0;
            wakeup_pushbutton();
        }
    }else{
        if(creat.size()!=0){
            for(vector<PCB>::iterator it = creat.begin();it!=creat.end();){
                if(ready.size() + wait.size() >= 10){
                    break;
                }
                if((*it).arr_time>t){break;}
                if(A.add((*it),HAVE_PRINTER)){
                    HAVE_PRINTER -= (*it).printer;
                    update_memory();
                    ready.insert(ready.end(),*it);
                    it = creat.erase(it);
                    ui->printer_num->setText(QString::number(HAVE_PRINTER));
                }else{
                    it++;
                }
            }
        }

        if(ready.size()!=0){
            update_creat();
            if(ui->pri_ture->isEnabled()){
                sort(ready.begin(),ready.end(),com_pri);
            }else if(ui->pri_false->isEnabled()){
                sort(ready.begin(),ready.end(),com_needtime);
            }

            run.insert(run.begin(),*ready.begin());
            ready.erase(ready.begin());
            update_ready();
            ui->run_table->clear();
            update_run();
            ui->run_table->setItem(0,0,new QTableWidgetItem(run.begin()->name));
            ui->run_table->setItem(0,1,new QTableWidgetItem(QString::number(run.begin()->arr_time)));
            ui->run_table->setItem(0,2,new QTableWidgetItem(QString::number(run.begin()->ser_time)));
            ui->run_table->setItem(0,3,new QTableWidgetItem(QString::number(run.begin()->rem_ser_time)));
            ui->run_table->setItem(0,4,new QTableWidgetItem(QString::number(run.begin()->pri)));
            run.begin()->rem_ser_time--;
            if(run.begin()->rem_ser_time==0){
                N++;
                A.cyc(run.begin()->name);
                update_memory();
                run.begin()->end_time = t+1;
                run.begin()->cyc_time = run.begin()->end_time - run.begin()->arr_time;
                run.begin()->wt_time = run.begin()->cyc_time / run.begin()->ser_time;
                cyc_sum += run.begin()->cyc_time;
                wt_cyc_sum += run.begin()->wt_time;
                HAVE_PRINTER += run.begin()->printer;
                ui->printer_num->setText(QString::number(HAVE_PRINTER));
                finish.insert(finish.end(),*run.begin());
                update_finish();
                run.pop_back();
            }else{
                ready.insert(ready.begin(),*run.begin());
            }
        }
    }
}
//进程调度

void SJF::update_creat(){
    int index=0;
    QStringList header;
    header<<"进程名"<<"到达时间"<<"服务时间"<<"内存"<<"打印机"<<"优先级";
    ui->creat_table->clear();
    ui->creat_table->setHorizontalHeaderLabels(header);
    vector<PCB>::iterator it=creat.begin();
    while(it!=creat.end()){
        ui->creat_table->setItem(index,0,new QTableWidgetItem((*it).name));
        ui->creat_table->setItem(index,1,new QTableWidgetItem(QString::number((*it).arr_time)));
        ui->creat_table->setItem(index,2,new QTableWidgetItem(QString::number((*it).ser_time)));
        ui->creat_table->setItem(index,3,new QTableWidgetItem(QString::number((*it).need_arm)));
        ui->creat_table->setItem(index,4,new QTableWidgetItem(QString::number((*it).printer)));
        ui->creat_table->setItem(index,5,new QTableWidgetItem(QString::number((*it).pri)));
        index++;
        it++;
    }
}
//更新创建队列

void SJF::update_ready(){
    int index=0;
    QStringList header;
    header<<"进程名"<<"到达时间"<<"服务时间"<<"内存"<<"打印机"<<"剩余时间"<<"优先级";
    ui->ready_table->clear();
    ui->ready_table->setHorizontalHeaderLabels(header);
    vector<PCB>::iterator it=ready.begin();
    while(it!=ready.end()){
        ui->ready_table->setItem(index,0,new QTableWidgetItem((*it).name));
        ui->ready_table->setItem(index,1,new QTableWidgetItem(QString::number((*it).arr_time)));
        ui->ready_table->setItem(index,2,new QTableWidgetItem(QString::number((*it).ser_time)));
        ui->ready_table->setItem(index,3,new QTableWidgetItem(QString::number((*it).need_arm)));
        ui->ready_table->setItem(index,4,new QTableWidgetItem(QString::number((*it).printer)));
        ui->ready_table->setItem(index,5,new QTableWidgetItem(QString::number((*it).rem_ser_time)));
        ui->ready_table->setItem(index,6,new QTableWidgetItem(QString::number((*it).pri)));
        index++;
        it++;
    }
}
//更新就绪队列

void SJF::update_run(){
    QStringList header;
    header<<"进程名"<<"到达时间"<<"服务时间"<<"剩余时间"<<"优先级";
    ui->run_table->clear();
    ui->run_table->setHorizontalHeaderLabels(header);
}
//更新运行表

void SJF::update_memory(){
    int index=0;
    QStringList header;
    header<<"始址"<<"大小"<<"进程"<<"状态";
    ui->arm_table->clear();
    ui->arm_table->setHorizontalHeaderLabels(header);
    Arm *p = A.head->next;
    while(p != NULL){
        ui->arm_table->setItem(index,0,new QTableWidgetItem(QString::number(p->address)));
        ui->arm_table->setItem(index,1,new QTableWidgetItem(QString::number(p->size)));
        ui->arm_table->setItem(index,2,new QTableWidgetItem(p->id));
        ui->arm_table->setItem(index,3,new QTableWidgetItem(p->state));
        index++;
        p = p->next;
    }
}
//更新内存表

void SJF::update_wait(){
    int index=0;
    QStringList header;
    header<<"进程名"<<"到达时间"<<"服务时间"<<"内存"<<"打印机"<<"剩余时间"<<"优先级";
    ui->wait_table->clear();
    ui->wait_table->setHorizontalHeaderLabels(header);
    vector<PCB>::iterator it=wait.begin();
    while(it!=wait.end()){
        ui->wait_table->setItem(index,0,new QTableWidgetItem((*it).name));
        ui->wait_table->setItem(index,1,new QTableWidgetItem(QString::number((*it).arr_time)));
        ui->wait_table->setItem(index,2,new QTableWidgetItem(QString::number((*it).ser_time)));
        ui->wait_table->setItem(index,3,new QTableWidgetItem(QString::number((*it).need_arm)));
        ui->wait_table->setItem(index,4,new QTableWidgetItem(QString::number((*it).printer)));
        ui->wait_table->setItem(index,5,new QTableWidgetItem(QString::number((*it).rem_ser_time)));
        ui->wait_table->setItem(index,6,new QTableWidgetItem(QString::number((*it).pri)));
        index++;
        it++;
    }
}
//更新阻塞队列

void SJF::update_finish(){
    int index=0;
    QStringList header;
    header<<"进程名"<<"到达时间"<<"服务时间"<<"内存"<<"打印机"<<"结束时间"<<"优先级";
    ui->finish_table->clear();
    ui->finish_table->setHorizontalHeaderLabels(header);
    vector<PCB>::iterator it=finish.begin();
    while(it!=finish.end()){
        ui->finish_table->setItem(index,0,new QTableWidgetItem((*it).name));
        ui->finish_table->setItem(index,1,new QTableWidgetItem(QString::number((*it).arr_time)));
        ui->finish_table->setItem(index,2,new QTableWidgetItem(QString::number((*it).ser_time)));
        ui->finish_table->setItem(index,3,new QTableWidgetItem(QString::number((*it).need_arm)));
        ui->finish_table->setItem(index,4,new QTableWidgetItem(QString::number((*it).printer)));
        ui->finish_table->setItem(index,5,new QTableWidgetItem(QString::number((*it).end_time)));
        ui->finish_table->setItem(index,6,new QTableWidgetItem(QString::number((*it).pri)));
        index++;
        it++;
    }
}
//更新完成队列

void SJF::update_ending(){
    qDebug()<<cyc_sum<<wt_cyc_sum<<N;
    int index=0;
    QStringList header;
    header<<"进程名"<<"到达时间"<<"服务时间"<<"结束时间"<<"周转时间"<<"带权周转时间"<<"平均周转时间"<<"平均带权周转时间";
    ui->end_table->clear();
    ui->end_table->setHorizontalHeaderLabels(header);
    vector<PCB>::iterator it=finish.begin();
    while(it!=finish.end()){
        ui->end_table->setItem(index,0,new QTableWidgetItem((*it).name));
        ui->end_table->setItem(index,1,new QTableWidgetItem(QString::number((*it).arr_time)));
        ui->end_table->setItem(index,2,new QTableWidgetItem(QString::number((*it).ser_time)));
        ui->end_table->setItem(index,3,new QTableWidgetItem(QString::number((*it).end_time)));
        ui->end_table->setItem(index,4,new QTableWidgetItem(QString::number((*it).cyc_time)));
        ui->end_table->setItem(index,5,new QTableWidgetItem(QString::number((*it).wt_time)));
        ui->end_table->setItem(index,6,new QTableWidgetItem(QString::number(cyc_sum/N)));
        ui->end_table->setItem(index,7,new QTableWidgetItem(QString::number(wt_cyc_sum/N)));
        index++;
        it++;
    }
}
//更新结果表

void SJF::set_os(){
    MAX_SIZE = ui->ram_edit->text().toUInt();
    MAX_PRINTER = ui->print_edit->text().toInt();
    if(ui->ram_edit->text().isEmpty()==true||ui->ram_edit->text().toDouble()<=0||ui->print_edit->text().isEmpty()==true||ui->print_edit->text().toDouble()<=0){
        QMessageBox::critical(this,"错误", "请重新进行系统设置!");
    }else{
        HAVE_PRINTER = MAX_PRINTER;
        ui->printer_num->setText(QString::number(HAVE_PRINTER));
        A.head->next->size = MAX_SIZE;
        ui->ram_edit->setEnabled(false);
        ui->print_edit->setEnabled(false);
        ui->set_os->setEnabled(false);
        ui->add->setEnabled(true);
    }
}
//内存打印机设置按钮

void SJF::creat_pushButton(){
    PCB p;
    QString name = ui->name_edit->text();    //接收输入数据
    QString arr_time = ui->arr_edit->text();
    QString ser_time = ui->ser_edit->text();
    QString arm = ui->arm_edit->text();
    QString printer = ui->printer_edit->text();
    QString pri;
    if(arm.toDouble()>MAX_SIZE||printer.toDouble()>MAX_PRINTER||arm.toDouble()<=0||printer.toDouble()<0){
        QMessageBox::critical(this,"错误", "请重新设置进程需求!");
        ui->name_edit->clear();   //输入后清空输入框
        ui->arr_edit->clear();
        ui->ser_edit->clear();
        ui->arm_edit->clear();
        ui->printer_edit->clear();
        ui->pri_edit->clear();
    }else{
        p.name = name;
        p.arr_time = arr_time.toDouble();
        p.ser_time = ser_time.toDouble();
        p.rem_ser_time = p.ser_time;
        p.need_arm = arm.toDouble();
        p.printer = printer.toDouble();
        if(ui->pri_ture->isChecked()){
            pri = ui->pri_edit->text();
            ui->pri_edit->clear();
            p.pri = pri.toDouble();
        }else{
            pri = "-1";
            p.pri = pri.toDouble();
        }

        if(ji1 == 0){
            ui->begin->setEnabled(true);
            ji1++;
        }
        ui->name_edit->clear();   //输入后清空输入框
        ui->arr_edit->clear();
        ui->ser_edit->clear();
        ui->arm_edit->clear();
        ui->printer_edit->clear();
        creat.push_back(p);
        update_creat();
    }
}
//添加进程按钮

void SJF::begin_pushbutton(){
    ui->stop_run->setEnabled(true);
    if(ui->pri_ture->isChecked()){
        ui->pri_false->setEnabled(false);
    }else{
        ui->pri_ture->setEnabled(false);
    }
    t1 = new QTimer;
    ui->os_time->setText("0");
    connect(t1,&QTimer::timeout,this,[=](){pcbsc();t++;ui->os_time->setText(QString::number(t));});
    t1->start(1000);
    ui->begin->setEnabled(false);
    ui->reback->setEnabled(false);
}
//开始按钮

void SJF::stop_pushbutton(){
    ui->continue_run->setEnabled(true);
    ui->stop_run->setEnabled(false);
    ui->reback->setEnabled(true);
    if(t1->isActive()){
        t1->stop();
        ui->run_table->clear();
        update_run();
        update_ready();
    }
}
//暂停按钮

void SJF::continue_pushbutton(){
    ui->continue_run->setEnabled(false);
    ui->stop_run->setEnabled(true);
    ui->reback->setEnabled(false);
    t1->setInterval(1000);
    t1->start();
}
//继续按钮

void SJF::sleep_pushbutton(){
    if(ready.size()==0){
        QMessageBox::warning(NULL,"警告","就绪队列无进程");
    }else{
        if(ui->continue_run->isEnabled()){
            vector<PCB>::iterator it = ready.begin();
            wait.push_back(*it);
            ready.erase(it);
            update_ready();
            update_wait();
        }else if(ui->stop_run->isEnabled()){
            vector<PCB>::iterator it = run.begin();
            wait.push_back(*it);
            ready.erase(ready.begin());
            run.erase(it);
            update_run();
            update_wait();
        }
    }
}
//阻塞按钮

void SJF::wakeup_pushbutton(){
    if(wait.size() == 0){
        QMessageBox::warning(NULL,"警告","阻塞队列无进程");
    }else{
        vector<PCB>::iterator it = wait.begin();
        ready.insert(ready.begin(),*it);
        wait.erase(it);
        update_ready();
        update_wait();
    }
}
//唤醒按钮

void SJF::reback_pushbutton(){
    t = 0; ji1 = 0;
    ui->ram_edit->setEnabled(true);
    ui->ram_edit->clear();
    ui->print_edit->setEnabled(true);
    ui->print_edit->clear();
    ui->continue_run->setEnabled(false);
    ui->os_time->setText("0");
    ui->printer_num->setText("0");
    creat.resize(0);
    ready.resize(0);
    run.resize(0);
    wait.resize(0);
    finish.resize(0);
    update_creat();
    update_ready();
    update_run();
    update_finish();
    update_wait();
    update_ending();
    ui->arm_table->setColumnCount(4);
    ui->arm_table->setRowCount(30);
    ui->arm_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->arm_table->verticalHeader()->setVisible(false);
    ui->set_os->setEnabled(true);
    ui->add->setEnabled(false);
    //内存格式化
    ui->arm_table->clear();
    QStringList header5;
    header5<<"始址"<<"大小"<<"进程"<<"状态";
    ui->arm_table->setHorizontalHeaderLabels(header5);
    ui->arm_table->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置不可修改
    ui->arm_table->setSelectionMode(QAbstractItemView::NoSelection);  //设置不可选择;
}
//重置按钮

SJF::~SJF()
{
    delete ui;
}
