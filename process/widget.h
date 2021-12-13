#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "srtf.h"
#include "sjf.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void back_sjf();
    void back_srtf();
    SJF sjf;
    SRTF srtf;

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
