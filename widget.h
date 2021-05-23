#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QSerialPort>
#include <QString>
#include<QtCore/QObject>

#include <QScatterSeries>               //画点状曲线头文件
#include <QtCharts/QSplineSeries>       //画线曲线头文件
#include <QValueAxis>                   //画坐标轴头文件
#include <QtCharts/QChartView>          //画绘图区域头文件
#include <QTimer>                       //时间定时器头文件
#include<QDateTimeAxis>
#include"QDateTime"

#include <QDebug>


QT_CHARTS_USE_NAMESPACE

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    QSerialPort *serialport;                   //定义一个指针对象
    void initDraw();                           //初始化画布
    QTimer *timer;                             //定义定时器对象
    float arr[1000];
    int BUF_SIZE;
    int index;                                 //用作数组下标
    int temp;
    float VOTE;



private:
    Ui::Widget *ui;
    QChart *chart;          //画布
    QSplineSeries*series;   //线
    QDateTimeAxis *axisX;   //x轴
    QValueAxis*axisY;       //y轴


private slots:
    void start_button();
    void stop_button();
    void serialPortReadyReady();
    void DrawLine();//划线，这里可以考虑使用数组
    void on_send_clicked();
    void on_clear_clicked();
    void on_line_shape_clicked();
};

#endif // WIDGET_H
