#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QDebug>
#include <QDoubleValidator>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);



    connect(ui->P_SRTF_4, &QPushButton::released,[=](){
        this->hide();
        srtf.show();
    });

    connect(ui->P_SJF_4, &QPushButton::released,[=](){
        this->hide();
        sjf.show();
    });

    void(SJF::*a)() = &SJF::sjf_signal;
    connect(&sjf, a, this, &Widget::back_sjf);
    void(SRTF::*b)() = &SRTF::srtf_signal;
    connect(&srtf, b, this, &Widget::back_srtf);

}

void Widget::back_sjf(){
    sjf.close();
    this->show();
}

void Widget::back_srtf(){
    srtf.close();
    this->show();
}

Widget::~Widget()
{
    delete ui;
}

