#include "serialAssistant.h"
#include <QToolBar>
#include <QSerialPortInfo>
#include <QString>
#include <QPushButton>
#include <QIcon>
#include <QSerialPort>
#include <QMessageBox>
#include <QLabel>


serialAssistant::serialAssistant(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_serialAssistant)
{
    ui->setupUi(this);
    //实例化串口
    serialPort = new QSerialPort(this);
    //搜索当前串口数据，将当前搜索到的串口数据添加到serialName中
    QStringList serialNamePort;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        serialNamePort << info.portName();
    }
    ui->serialPortCb->addItems(serialNamePort);

    //连接开关按键
    connect(ui->openSerialPbt, SIGNAL(clicked()), this, SLOT(openSerial_clicked()));
    //连接关闭按键
    connect(ui->closeSerialPbt, SIGNAL(clicked()), this, SLOT(closeSerial_clicked())); 
    //连接清空按键
    connect(ui->clearMessagePbt, SIGNAL(clicked()), this, SLOT(clearSerial_clicked()));
    //连接发送开关的槽
    connect(ui->sendMessagePbt, SIGNAL(clicked()), this, SLOT(sendSerial()));
    //连接接收槽函数
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(serialPortReadyRead_slot()));

}

void serialAssistant::openSerial_clicked(void)
{
    //读取按键数据，进行串口初始化
    QSerialPort::BaudRate baudRate;//波特率
    QSerialPort::DataBits dataBits;//数据位
    QSerialPort::StopBits stopBits;//停止位
    QSerialPort::Parity parity;//校验位

    //设置波特率
    if (ui->baudRateCb->currentText() == "4800")
    {
        baudRate = QSerialPort::Baud4800;
    }
    else if (ui->baudRateCb->currentText() == "9600")
    {
        baudRate = QSerialPort::Baud9600; 
    }
    else if (ui->baudRateCb->currentText() == "115200")
    {
        baudRate = QSerialPort::Baud115200;
    }


    //设置数据位
    if (ui->dataBitCb->currentText() == "7")
    {
        dataBits = QSerialPort::Data7;
    }
    else if (ui->dataBitCb->currentText() == "8")
    {
        dataBits = QSerialPort::Data8;
    }
    
    //设置停止位
    if (ui->stopBitCB->currentText() == "1")
    {
        stopBits = QSerialPort::OneStop;
    }
    else if (ui->stopBitCB->currentText() == "1.5")
    {
        stopBits = QSerialPort::OneAndHalfStop;
    }
    else if (ui->stopBitCB->currentText() == "2")
    {
        stopBits = QSerialPort::TwoStop;
    }

    //设置校验位
    if (ui->ParityCb->currentText() == "none")
    {
        parity = QSerialPort::NoParity;
    }

    //赋值串口号
    serialPort->setPortName(ui->serialPortCb->currentText());
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(dataBits);
    serialPort->setStopBits(stopBits);
    serialPort->setParity(parity);

    //输出状态
    qDebug("open seial port");
    //判断串口是否打开成功
    if (serialPort->open(QIODevice::ReadWrite) == true)
    {
        QMessageBox::information(this, "output", "successful");
        qDebug("open serial port successful");
    }
    else
    {
        QMessageBox::critical(this, "output", "fail");
        qDebug("open serila port fail");
    }
}

void serialAssistant::closeSerial_clicked(void)
{
    serialPort->close();
    qDebug("close serial port");
}

void serialAssistant::clearSerial_clicked(void)
{
    ui->recvEdit->clear();
    qDebug("clear output edit");
}

void serialAssistant::serialPortReadyRead_slot(void)
{
    QString buf;
    buf = QString(serialPort->readAll());
    ui->recvEdit->appendPlainText(buf);
    qDebug("receive message");
}

void serialAssistant::sendSerial(void)
{
    serialPort->write(ui->sendMessageEdit->text().toLocal8Bit().data());
    qDebug("send serial message");
}


serialAssistant::~serialAssistant()
{
    delete ui;
}

