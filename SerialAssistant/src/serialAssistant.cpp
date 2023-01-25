#include "serialAssistant.h"
#include <QToolBar>
#include <QSerialPortInfo>
#include <QString>
#include <QPushButton>
#include <QIcon>
#include <QSerialPort>
#include <QMessageBox>
#include <QLabel>

#define INIT_CHART_AXIS_RANGE             \
    {                                     \
        lineSeries->clear();              \
        xMin = 0;                         \
        xMax = 50;                        \
        yMin = 0;                         \
        yMax = 50;                        \
        chartAxisX->setRange(xMin, xMax); \
        chartAxisY->setRange(yMin, yMax); \
    }

// 串口打开的按键状态
#define PBT_OPEN_STATUS                             \
    {                                               \
        ui->openSerialPbt->setEnabled(false);       \
        ui->searchSearchportPbt->setEnabled(false); \
        ui->clearMessagePbt->setEnabled(true);      \
        ui->closeSerialPbt->setEnabled(true);       \
        ui->sendMessagePbt->setEnabled(true);       \
    }
// 串口关闭时的按键状态
#define PBT_CLOSE_STATUS                           \
    {                                              \
        ui->openSerialPbt->setEnabled(true);       \
        ui->searchSearchportPbt->setEnabled(true); \
        ui->clearMessagePbt->setEnabled(true);     \
        ui->closeSerialPbt->setEnabled(false);     \
        ui->sendMessagePbt->setEnabled(false);     \
    }

serialAssistant::serialAssistant(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_serialAssistant)
{
    ui->setupUi(this);
    //实例化串口
    serialPort = new QSerialPort(this);
    QStringList serialNamePort;
    //搜索当前串口数据，将当前搜索到的串口数据添加到serialName中
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        serialNamePort << info.portName();
    }
    ui->serialPortCb->addItems(serialNamePort);

    //获取当前时间
    curTime = QTime::currentTime();
    //初始化串口状态
    serialStatus = false;

    //初始化图表
    //创建一个图表
    serialDataCharts = new QChart();
    //创建坐标轴
    chartAxisX = new QValueAxis();
    chartAxisY = new QValueAxis();
    //设置标题
    chartAxisX->setTitleText("time/s");
    chartAxisY->setTitleText("value");
    //设置标签
    chartAxisX->setLabelFormat("%f");
    chartAxisY->setLabelFormat("%f");
    //向图表添加坐标轴
    serialDataCharts->addAxis(chartAxisX, Qt::AlignBottom);
    serialDataCharts->addAxis(chartAxisY, Qt::AlignLeft);
    //设置图例标签
    serialDataCharts->setTitle("Serial Real-Time Data");
    //设置图例不显示
    serialDataCharts->legend()->setVisible(false);
    //实例化线
    lineSeries = new QLineSeries(this);
    //设置线的颜色,为蓝色
    QPen pen(QColor(0x0000FF));
    lineSeries->setPen(pen);
    //向图表中添加线
    serialDataCharts->addSeries(lineSeries);
    //将曲线与坐标轴联系起来
    lineSeries->attachAxis(chartAxisX);
    lineSeries->attachAxis(chartAxisY);
    //将图表放到图表视图中
    ui->chartview->setChart(serialDataCharts);
    //初始化图表数据
    INIT_CHART_AXIS_RANGE;
    //设置串口按键状态
    PBT_CLOSE_STATUS;
    //设置图表按键状态
    ui->runPbt->setEnabled(true);
    ui->stopPbt->setEnabled(false);
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
    //连接搜索开关
    connect(ui->searchSearchportPbt, SIGNAL(clicked()), this, SLOT(searchSerialPort_clicked()));
    //连接图表打开开关
    connect(ui->runPbt, SIGNAL(clicked()), this, SLOT(runSerialDataChart_clicked()));
    //连接图表关闭开关
    connect(ui->stopPbt, SIGNAL(clicked()), this, SLOT(stopSerialDataChart_clicked()));
    

}

void serialAssistant::stopSerialDataChart_clicked(void)
{
    //停止连接串口接收
    disconnect(serialPort, SIGNAL(readyRead()), this, SLOT(serialDataChartShow()));
    ui->recvEdit->appendPlainText("close chart");
    //设置按键状态
    ui->runPbt->setEnabled(true);
    ui->stopPbt->setEnabled(false);
}

void serialAssistant::serialDataChartShow()
{
    //判断x轴y轴坐标是否符合需求
    qreal curTime = serialTim.msec();
    qreal curValue = QString(serialPort->readAll()).toInt();
    if(curTime > xMax)
    {
        xMin += (curTime - xMax) + 20;
        xMax += (curTime - xMax) + 20;
        chartAxisX->setRange(xMin, xMax);
    }
    //判断y轴
    if(curValue > yMax)
    {
        yMax += (curValue - yMax) + yMax;
        chartAxisY->setRange(yMin, yMax);
        
    }
    else if (curValue < yMin)
    {
        yMin -= (yMin - curValue) - yMin;    
        chartAxisY->setRange(yMin, yMax);
    }
    //添加数据
    lineSeries->append(curTime, curValue);
}

void serialAssistant::runSerialDataChart_clicked(void)
{
    //判断串口状态
    if(serialStatus == true)
    {
        //串口状态为打开
        //清空线
        INIT_CHART_AXIS_RANGE;
        //设置图表坐标轴范围
        // 连接串口接收槽函数
        connect(serialPort, SIGNAL(readyRead()), this, SLOT(serialDataChartShow()));
        ui->recvEdit->appendPlainText("open chart");
        //开始计时
        serialTim.restart();
        qDebug("open chart");

        //设置按键状态
        ui->runPbt->setEnabled(false);
        ui->stopPbt->setEnabled(true);
    }
    else
    {
        QMessageBox::critical(this, "Warning", "Don't open serial");
        ui->recvEdit->appendPlainText("open chart fail, please open serial");
        qDebug("open chart fail");
    }

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
    //获取当前时间
    curTime = QTime::currentTime();
    //判断串口是否打开成功
    if (serialPort->open(QIODevice::ReadWrite) == true)
    {
        ui->recvEdit->appendPlainText(curTime.toString() + " " + "serial port connect successful, serial port name is " + ui->serialPortCb->currentText());
        
        // QMessageBox::information(this, "output", "successful");
        qDebug("open serial port successful");
        //设置按键状态
        PBT_OPEN_STATUS;
        serialStatus = true;
    }
    else
    {
        ui->recvEdit->appendPlainText(curTime.toString() + " " + "serial port connect fail");
        // QMessageBox::critical(this, "output", "fail");
        qDebug("open serila port fail");
    }
}
void serialAssistant::closeSerial_clicked(void)
{
    serialPort->close();
    //设置按键状态
    PBT_CLOSE_STATUS;
    //获取当前时间
    curTime = QTime::currentTime();
    // 输出关闭串口信息
    ui->recvEdit->appendPlainText(curTime.toString() + "close serial port");
    qDebug("close serial port");
    serialStatus = false;
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
    ui->recvEdit->appendPlainText(curTime.toString() + " " +  buf);
    qDebug("receive message");
}

void serialAssistant::sendSerial(void)
{
    //获取当前时间
    curTime = QTime::currentTime();
    serialPort->write((ui->beginMessageEdit->text() + ui->sendMessageEdit->text() + ui->endMessageEdit->text()).toLocal8Bit().data());
    qDebug("send serial message");
}

void serialAssistant::searchSerialPort_clicked(void)
{
    qDebug("search serial port");
    //获取当前时间
    curTime = QTime::currentTime();
    //汇报输出
    ui->recvEdit->appendPlainText(curTime.toString() + " search serial port");
    QStringList serialNamePort;
    //搜索当前串口数据，将当前搜索到的串口数据添加到serialName中
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        serialNamePort << info.portName();
    }
    ui->serialPortCb->addItems(serialNamePort);
}

serialAssistant::~serialAssistant()
{
    delete ui;
}

