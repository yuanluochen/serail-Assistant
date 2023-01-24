#ifndef SERIALCHARTS_H
#define SERIALCHARTS_H

#include <QWidget>
#include <QMainWindow>

namespace Ui {
class SerialCharts;
}

class SerialCharts : public QWidget
{
    Q_OBJECT

public:
    explicit SerialCharts(QWidget *parent = nullptr);
    ~SerialCharts();
private slots:
    //返回按键
    void back_clicked(void);
private:
    Ui::SerialCharts *ui;
};

#endif // SERIALCHARTS_H
