#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    checkLcdFormat();

    timerTimeout = 0;
    timerIncrement = new QTimer(this);
    timerDecrement = new QTimer(this);
    connect(timerIncrement, SIGNAL(timeout()), this, SLOT(doIncrement()));
    connect(timerDecrement, SIGNAL(timeout()), this, SLOT(doDecrement()));
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
        if(temperatura[i] == '.') return 1;
    }
    return 0;
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
}

void MainWindow::doIncrement()
{
    if(ui->lcdTemp->value() >= 0  && ui->lcdTemp->value() < 100)
    {
        ui->lcdTemp->display(ui->lcdTemp->value() + 0.5);
        checkLcdFormat();
        if(timerTimeout > 100) timerTimeout = timerTimeout / 2;
        else timerTimeout = 50;
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
}

void MainWindow::doDecrement()
{
    if(ui->lcdTemp->value() > 0  && ui->lcdTemp->value() <= 100)
    {
        ui->lcdTemp->display(ui->lcdTemp->value() - 0.5);
        checkLcdFormat();
        if(timerTimeout > 100) timerTimeout = timerTimeout / 2;
        else timerTimeout = 50;
        timerDecrement->start(timerTimeout);
    }
}
