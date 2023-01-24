#include "serialcharts.h"
#include "ui_serialcharts.h"
#include <QPushButton>
#include <QToolBar>



SerialCharts::SerialCharts(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialCharts)
{
    ui->setupUi(this);
    //添加左侧工具栏
    QToolBar* toolBar = new QToolBar(this);

    //连接槽函数
    connect(ui->BackPbt, SIGNAL(clicked()), this, SLOT(back_clicked())); 
    

}

void SerialCharts::back_clicked(void)
{
    //关闭窗口
    this->close();
}

SerialCharts::~SerialCharts()
{
    delete ui;
}

