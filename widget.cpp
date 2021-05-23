#include "widget.h"
#include "ui_widget.h"
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>
#include<QSplineSeries>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    timer=new QTimer(this);
    QStringList serialNameport;

    serialport =new QSerialPort(this);//实例化对象
    connect(ui->open_serial,SIGNAL(clicked()),this,SLOT( start_button()));
    connect(ui->close_serial,SIGNAL(clicked()),this,SLOT( stop_button()));
    //connect(serialport,SIGNAL(readyRead()),this,SLOT(serialPortReadyReady()));这里不需要的原因是，两个信号同时操作一个数据数组会造成冲突导致画图异常。
    connect(timer,SIGNAL(timeout()),this,SLOT(DrawLine()));//关联定时器与定时溢出处理函数

    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts() )
    {

        serialNameport<<info.portName();


    }
    ui->serialNUM->addItems(serialNameport);
}

void Widget::serialPortReadyReady(){//接受数据函数


}





void Widget::start_button(){
    QSerialPort::BaudRate baudRate;

    QSerialPort::DataBits dataBits;
    QSerialPort::StopBits stopBits;
    QSerialPort::Parity   checkbits;//校验位
    /*波特率*/
    if(ui->baundrate->currentText()=="4800")
    {
        baudRate=QSerialPort::Baud4800;
    }
    else if(ui->baundrate->currentText()=="9600")
    {
        baudRate=QSerialPort::Baud9600;
    }
    else if(ui->baundrate->currentText()=="115200")
    {
        baudRate=QSerialPort::Baud115200;
    }


    /*数据位*/
    if(ui->data_bits->currentText()=="5")
    {
        dataBits=QSerialPort::Data5;

    }
    else if(ui->data_bits->currentText()=="6")
    {
        dataBits=QSerialPort::Data6;

    }
    else if(ui->data_bits->currentText()=="7")
    {
        dataBits=QSerialPort::Data7;

    }
    else if(ui->data_bits->currentText()=="8")
    {
        dataBits=QSerialPort::Data8;
    }


    /*停止位*/
    if(ui->stop_bits->currentText()=="1")
    {
        stopBits=QSerialPort::OneStop;
    }
    else if(ui->stop_bits->currentText()=="1.5")
    {
        stopBits=QSerialPort::OneAndHalfStop;
    }
    else if(ui->stop_bits->currentText()=="2")
    {
        stopBits=QSerialPort::TwoStop;
    }


    /*校验位*/
    if(ui->check_bits->currentText()=="none")
    {
        checkbits=QSerialPort::NoParity;
    }


    serialport->setPortName(ui->serialNUM->currentText());
    serialport->setBaudRate(baudRate);
    serialport->setDataBits(dataBits);
    serialport->setStopBits(stopBits);
    serialport->setParity(checkbits);



    if(serialport->open(QIODevice::ReadWrite)==true){
        QMessageBox::information(this,"提示","成功");
    }
      else{
           QMessageBox::critical(this,"提示","失败");
      }

}

void Widget::stop_button()
{
    serialport->close();
    timer->stop();




}
Widget::~Widget()
{
    delete ui;
}

void Widget::on_send_clicked()
{
    serialport->write(ui->send_pannel->text().toLocal8Bit().data());

}

void Widget::on_clear_clicked()
{
    ui->receive_pannel->clear();

}


//初始化画布
void Widget::initDraw()
{
    QPen penY(Qt::darkBlue,3,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);//设置画笔
    chart=new QChart();
    series=new QSplineSeries;//实例化对象
    axisX=new QDateTimeAxis();
    axisY=new QValueAxis();
    chart->legend()->hide();
    chart->addSeries(series);
    axisX->setTickCount(10);
    axisY->setTickCount(5);
    axisX->setFormat("ms");
    axisY->setMin(0);
    axisY->setMax(4);
    axisX->setTitleText("实时时间");
    axisY->setTitleText("实时电压监测");
    axisY->setLinePenColor(QColor(Qt::darkBlue));
    axisY->setGridLineColor(QColor(Qt::darkBlue));
    axisY->setGridLineVisible(false);
    axisY->setLinePen(penY);
    axisX->setLinePen(penY);
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);//设置坐标轴位置
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    //将chart显示到窗口上
    ui->Widget::main_Draw->setChart(chart);
    ui->Widget::main_Draw->setRenderHint(QPainter::Antialiasing);
}

/*绘制曲线的函数*/
void Widget::DrawLine()
{
    for(int i = 0 ; i < 1000 ; i++){
        arr[i] = NULL;//数据数组全部置为NULL，防止干扰
    }
    QByteArray buf;
    QString show_receive;
    unsigned char arr1[2]={'0'};
    buf =serialport->readAll();
    int temp=0;
    index=0;//arr存储数据数组下标，每次开始置零
   // qDebug()<<buf.toHex();
   // qDebug()<<buf;
   // qDebug()<<buf.size();


    for(int i=0;i<buf.size();i++){

        if(i<buf.size()){
            if(buf.at(i)==(char)0xaa){
                if(i+1<buf.size()){
                    if(buf.at(i+1)==(char)0x08){
                        if(i+4<buf.size()){
                            if(buf.at(i+4)==(char)0xab){
                                arr1[0]=buf.at(i+2);
                                arr1[1]=buf.at(i+3);
                                temp=arr1[0]*256+arr1[1];
                                VOTE=temp*(3.3/4096);
                                arr[index]=VOTE;
                                if(index<=buf.size()){
                                    index++;
                                }
                                qDebug("v=%.4lf",VOTE);
                              //  show_receive.sprintf("%s=%0.2f%%",VOTE);
                              //  ui->receive_pannel->appendPlainText(show_receive);

                            }
                        }
                    }
                }
            }
        }
    }
    QDateTime currentTime=QDateTime::currentDateTime(); //设置坐标值显示范围
    chart->axisX()->setMin(QDateTime::currentDateTime().addSecs(-10*1));
    chart->axisX()->setMax(QDateTime::currentDateTime().addSecs(0));
    for(int i=0;i<index;i++){
        series->append(currentTime.toMSecsSinceEpoch(),arr[i]);
        arr[i]=NULL;//绘完图后,数据归NULL.+
    }
    buf.clear();



}


void Widget::on_line_shape_clicked()
{
    initDraw();
    timer->start();
    timer->setInterval(1);
}
