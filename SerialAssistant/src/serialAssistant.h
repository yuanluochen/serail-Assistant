#pragma once
#include "ui_serialAssistant.h"
#include <QMainWindow>
#include <QSerialPort>
#include <QChart>
#include <QTime>
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
    //串口接收并显示到界面上
    void serialPortReadyRead_slot(void);
    //串口数据接收
    void sendSerial(void);

private:
    Ui_serialAssistant* ui;
    //串口
    QSerialPort* serialPort;
    //时间
    QTime curTime;

private:
    bool openSerialflag;//串口打开标志位

};