#include "widget.h"
#include <QApplication>

#include <QtWidgets/QLabel>          //标签
#include <QtCharts/QChartView>       //图表
#include <QtWidgets/QPushButton>     //按钮
#include <QtWidgets/QApplication>    //
#include <QtWidgets/QPlainTextEdit>  //留言板
#include <QtCharts>                  //画图
#include <QtCharts/QScatterSeries>   //点
#include <QtCharts/QLineSeries>      //线
#include <QtCharts/QValueAxis>       //坐标系

#include <QResizeEvent>              //绘图区域大小变化事件

#include <QDebug>                    //打印参数头文件，最好加上，最好用上，很方便

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    //QSerialPort::readData()

    return a.exec();
}
