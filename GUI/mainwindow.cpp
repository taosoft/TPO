#include "mainwindow.h"
#include <iostream>
#include "ui_mainwindow.h"
#include <QTimer>
#include <QtCore/QtMath>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QStandardItemModel>
#include "qcustomplot.h"
#include <QVector>
#include <QMessageBox>
#include <QByteArray>
#include <QDebug>
#include <QThread>
#include <QFont>

#include <QTcpSocket>

QT_CHARTS_USE_NAMESPACE

using namespace std;
using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Botonera
    checkLcdFormat();

    timerTimeout = 0;
    timerIncrement = new QTimer(this);
    timerDecrement = new QTimer(this);
    connect(timerIncrement, SIGNAL(timeout()), this, SLOT(doIncrement()));
    connect(timerDecrement, SIGNAL(timeout()), this, SLOT(doDecrement()));

    //TcpClient
    mSocket = new QTcpSocket(this);
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(on_ClientDisconnected()));
    connect(mSocket, SIGNAL(connected()), this, SLOT(on_ClientConnected()));
    connect(mSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(on_ClientStateChange(QAbstractSocket::SocketState)));
    connect(mSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(on_ClientError(QAbstractSocket::SocketError)));
    connect(mSocket, &QTcpSocket::readyRead, [&](){
        QTextStream T(mSocket);
        analyzeRcvData(T.readAll());
    });

    //Gráfico Real Time
    ui->graficoRT->addGraph();
    ui->graficoRT->graph(0)->setPen(QPen(QColor(0, 0, 222)));
    ui->graficoRT->graph(0)->addData(0,0);
    ui->graficoRT->xAxis->setLabel("Tiempo [S]");
    ui->graficoRT->yAxis->setLabel("Temperatura [grados C]");
    ui->graficoRT->xAxis->setRange(0, 100);
    ui->graficoRT->yAxis->setRange(0, 100);

    //Gráfico historial SD
    QValueAxis *axisX = new QValueAxis;
    QValueAxis *axisY = new QValueAxis;
    axisX->setRange(0.1, 10.0);
    axisY->setRange(-1.0, 1.0);
    series = new QLineSeries();
    series->setColor(QColor(0, 0, 222));
    chartHSD = new QChart();
    chartHSD->addSeries(series);
    chartHSD->setTitle("Historial de muestras de temperatura");
    chartHSD->addAxis(axisX, Qt::AlignBottom);
    chartHSD->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    ui->chartViewHSD->setChart(chartHSD);
    ui->chartViewHSD->setRenderHint(QPainter::Antialiasing);

    //Tabs
    ui->tabWidget->setTabEnabled(1, false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::isFloat(double temp)
{
    QByteArray temperatura = QByteArray::number(temp);
    for(int i = 0; i < temperatura.length(); i++)
    {
        if(temperatura[i] == '.')
            return 1;
    }
    return 0;
}

bool MainWindow::sendNewConfigTemp(void)
{
    if(mSocket != nullptr)
    {
        if(mSocket->state() == QAbstractSocket::ConnectedState)
        {
            double tempDOUBLE = ui->lcdTemp->value() * 10;
            QByteArray temp = QByteArray::number(tempDOUBLE);
            QByteArray data = "$$$t" + QByteArray::number(temp.length()) + temp + "\r";
            mSocket->write(data);
            return mSocket->waitForBytesWritten(500);
        }
    }
    return false;
}

void MainWindow::checkLcdFormat()
{
    if(!isFloat(ui->lcdTemp->value()))
    {
        QString mask = QString::number(ui->lcdTemp->value()) + ".0";
        ui->lcdTemp->display(mask);
    }
}

void MainWindow::on_btnTempMas_pressed()
{
    timerTimeout = 1000;
    doIncrement();
}

void MainWindow::on_btnTempMas_released()
{
    timerIncrement->stop();
    if(!sendNewConfigTemp())
        qDebug("No se puede enviar la temp");
}

void MainWindow::doIncrement()
{
    if(ui->lcdTemp->value() >= 0  && ui->lcdTemp->value() < 100)
    {
        ui->lcdTemp->display(ui->lcdTemp->value() + 0.5);
        checkLcdFormat();
        if(timerTimeout > 100)
            timerTimeout = timerTimeout / 2;
        else
            timerTimeout = 50;
        timerIncrement->start(timerTimeout);
    }
}

void MainWindow::on_btnTempMenos_pressed()
{
    timerTimeout = 1000;
    doDecrement();
}

void MainWindow::on_btnTempMenos_released()
{
    timerDecrement->stop();
    if(!sendNewConfigTemp())
        qDebug("No se puede enviar la temp");
}

void MainWindow::doDecrement()
{
    if(ui->lcdTemp->value() > 0  && ui->lcdTemp->value() <= 100)
    {
        ui->lcdTemp->display(ui->lcdTemp->value() - 0.5);
        checkLcdFormat();
        if(timerTimeout > 100)
            timerTimeout = timerTimeout / 2;
        else
            timerTimeout = 50;
        timerDecrement->start(timerTimeout);
    }
}

void MainWindow::realtime(double temp)
{
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0;
    static double lastPointKey = 0;
    if(key - lastPointKey > 0.002)
    {
        ui->graficoRT->graph(0)->addData(key, temp);
        lastPointKey = key;
    }
    ui->graficoRT->graph(0)->rescaleValueAxis();
    ui->graficoRT->xAxis->setRange(key, 20, Qt::AlignRight);
    ui->graficoRT->replot();
}

void MainWindow::on_btnConnectToServer_clicked()
{
    if(mSocket != nullptr)
    {
        if(mSocket->state() == QAbstractSocket::UnconnectedState)
        {
            mSocket->connectToHost("192.168.8.101", 8080);
        }
    }
}

void MainWindow::analyzeRcvData(QString data)
{
    if(data != "timeout")
    {
        if(data.length() > 6)
        {
            QStringRef initData(&data, 0, 3);
            if(initData == "$$$")
            {
                QStringRef dataType (&data, 3, 3);
                if(dataType == "log")
                {
                    QStringRef unixTime(&data, 6, 10);
                    QStringRef largoTempString(&data, 16, 1);
                    int largoTemp = largoTempString.toInt();
                    QStringRef tempString(&data, 17, largoTemp);
                    double entero = tempString.toInt() / 10;
                    float decimal = tempString.toInt() % 10;
                    double temp = double(decimal / 10);
                    temp += entero;

                    ui->lblTempActual->setText("<html><head/><body><p align='center'><span style='font-size:24pt;'>" + QString::number(temp) + "  grados C</span></p></body></html>");
                    realtime(temp);
                }
                if(dataType == "sta")
                {
                    QStringRef estadoCalentador(&data, 6, 1);
                    if(estadoCalentador == "1")
                    {
                        ui->lblReleOnOff->setText("<html><head/><body><p align='center'><span style='font-size:24pt;'>Encendido</span></p></body></html>");
                        ui->lblReleOnOff->setStyleSheet("border: 2px solid black; border-radius: 10px; color: green; background-color: white;");
                    }
                    else
                    {
                        ui->lblReleOnOff->setText("<html><head/><body><p align='center'><span style='font-size:24pt;'>Apagado</span></p></body></html>");
                        ui->lblReleOnOff->setStyleSheet("border: 2px solid black; border-radius: 10px; color: red; background-color: white;");
                    }
                }
            }
        }
    }
    if(mSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data = "timeout\r";
        mSocket->write(data); //write the data itself
        mSocket->waitForBytesWritten();
    }
}

void MainWindow::on_btnLeerTarjetaSD_clicked()
{
    QFile mFile;

    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString nombreUsuario = homePath.first().split(QDir::separator()).last();

    mFile.setFileName("/media/" + nombreUsuario + "/1GB SANDISK/data.txt");
    if(!mFile.open(QFile::ReadOnly | QFile::Text)){
       qDebug () << "No se encuentra la ruta de la SD" ;
    }
    else
    {
        QTextStream in(&mFile);
        seriesData.clear();

        while(!in.atEnd())
        {
            QString text = in.readLine();
            QStringList lista = text.split(":");
            qreal x = lista.value(0).toInt();
            qreal y = lista.value(1).toInt()/10;
            int valor1 = lista.value(1).toInt() % 10;
            float valor = float(valor1/10);
            y += double(valor);
            seriesData.append(QPointF(x, y));
        }
        if(!seriesData.isEmpty())
        {
            seriesData.removeLast();
            mFile.close();

            qreal xMin = std::numeric_limits<int>::max(); // everything is <= this
            qreal xMax = std::numeric_limits<int>::min(); // everything is >= this
            qreal yMin = std::numeric_limits<int>::max();
            qreal yMax = std::numeric_limits<int>::min();

            foreach (QPointF p, seriesData) {
                xMin = qMin(xMin, p.x());
                xMax = qMax(xMax, p.x());
                yMin = qMin(yMin, p.y());
                yMax = qMax(yMax, p.y());
            }

            QValueAxis *axisX = new QValueAxis;
            QValueAxis *axisY = new QValueAxis;

            axisX->setRange(xMin, xMax);
            axisY->setRange(yMin, yMax);

            seriesHSD = new QLineSeries();
            seriesHSD->setColor(QColor(0, 0, 222));

            chartHSD = new QChart();
            chartHSD->addSeries(seriesHSD);
            chartHSD->setTitle("Historial de muestras de temperatura");
            chartHSD->addAxis(axisX, Qt::AlignBottom);
            chartHSD->addAxis(axisY, Qt::AlignLeft);

            seriesHSD->attachAxis(axisX);
            seriesHSD->attachAxis(axisY);

            ui->chartViewHSD->setChart(chartHSD);
            ui->chartViewHSD->setRenderHint(QPainter::Antialiasing);

            seriesHSD->replace(seriesData);
        }
    }
}

void MainWindow::on_btnBorrarTarjetaSD_clicked()
{
    QFile mFile;
    QTextStream io;

    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString nombreUsuario = homePath.first().split(QDir::separator()).last();

    mFile.setFileName("/media/" + nombreUsuario + "/1GB SANDISK/data.txt");
    if(!mFile.open(QFile::WriteOnly | QFile::Text)){
       qDebug () << "No se encuentra la ruta de la SD" ;
    }
    else
    {
        io.setDevice(&mFile);
        QString text = "";
        io << text;
        mFile.flush();
        mFile.close();
    }
}

void MainWindow::on_ClientDisconnected()
{
    mSocket->close();
    ui->lblServerConn->setText("<html><head/><body><p align='center'><span style='font-size:24pt;'>Desconectado</span></p></body></html>");
    ui->lblServerConn->setStyleSheet("border: 2px solid black; border-radius: 10px; color: red; background-color: white;");
}

void MainWindow::on_ClientConnected()
{
    ui->lblServerConn->setText("<html><head/><body><p align='center'><span style='font-size:24pt;'>Conectado</span></p></body></html>");
    ui->lblServerConn->setStyleSheet("border: 2px solid black; border-radius: 10px; color: green; background-color: white;");
}

void MainWindow::on_ClientStateChange(QAbstractSocket::SocketState state)
{
    if(state == QAbstractSocket::UnconnectedState)
    {
        this->on_ClientDisconnected();
    }
}

void MainWindow::on_ClientError(QAbstractSocket::SocketError error)
{
    if(error)
        this->on_ClientDisconnected();
}

void MainWindow::on_btnConfigurarRTCStick_clicked()
{
    if(mSocket != nullptr)
    {
        if(mSocket->state() == QAbstractSocket::ConnectedState)
        {
            QByteArray data = "$$$unixtime\r";
            mSocket->write(data);
            mSocket->waitForBytesWritten(500);
        }
    }
}

void MainWindow::on_btnDisconnectFromServer_clicked()
{
    if(mSocket != nullptr)
    {
        if(mSocket->state() == QAbstractSocket::ConnectedState)
        {
            mSocket->disconnectFromHost();
        }
    }
}
