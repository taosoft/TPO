#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    checkLcdFormat();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnTempMas_clicked()
{
    if(ui->lcdTemp->value() >= 0  && ui->lcdTemp->value() < 100)
    {
        ui->lcdTemp->display(ui->lcdTemp->value() + 0.5);
        checkLcdFormat();
    }
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

void MainWindow::on_btnTempMenos_clicked()
{
    if(ui->lcdTemp->value() > 0  && ui->lcdTemp->value() <= 100)
    {
        ui->lcdTemp->display(ui->lcdTemp->value() - 0.5);
        checkLcdFormat();
    }
}
