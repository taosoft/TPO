#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_btnTempMas_pressed();

    void doIncrement();

    void on_btnTempMas_released();

    void on_btnTempMenos_pressed();

    void doDecrement();

    void on_btnTempMenos_released();

private:
    Ui::MainWindow *ui;
    int isFloat(double temp);
    void checkLcdFormat();

    QTimer *timerIncrement;
    QTimer *timerDecrement;
    int timerTimeout;
};

#endif // MAINWINDOW_H
