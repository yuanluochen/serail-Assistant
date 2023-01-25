#pragma once
#include "ui_serialAssistant.h"
#include <QMainWindow>
#include <QSerialPort>
#include <QChart>
#include <QTime>
#include <QTimer>
#include <QValueAxis>
#include <qlineseries.h>

class serialAssistant : public QMainWindow {
    Q_OBJECT
    
public:
    serialAssistant(QWidget* parent = nullptr);
    ~serialAssistant();

private slots:
    //串口开辟函数
    void openSerial_clicked(void);
    //串口关闭函数
    void closeSerial_clicked(void);
    //显示数据清除
    void clearSerial_clicked(void);
    //搜索串口
    void searchSerialPort_clicked(void);
    //串口接收并显示到界面上
    void serialPortReadyRead_slot(void);
    //串口数据接收
    void sendSerial(void);

    //图表开启函数
    void runSerialDataChart_clicked(void);
    //图表关闭
    void stopSerialDataChart_clicked(void);
    //连接图表清理
    //图表数据显示
    void serialDataChartShow(void);

private:
    Ui_serialAssistant* ui;
    //串口
    QSerialPort* serialPort;
    //时间
    QTime curTime;
    //定时器
    QTime serialTim;
    //串口状态
    bool serialStatus;

    //图表
    QChart* serialDataCharts;
    //坐标轴
    QValueAxis* chartAxisX;//x轴    
    QValueAxis* chartAxisY;//y轴    
    //线
    QLineSeries* lineSeries;
    //设置x轴y轴的起始大小
    int xMin;
    int yMin;
    int xMax;
    int yMax;
};