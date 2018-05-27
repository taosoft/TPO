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
    void on_btnTempMas_clicked();

    void on_btnTempMenos_clicked();

private:
    Ui::MainWindow *ui;
    int isFloat(double temp);
    void checkLcdFormat();
};

#endif // MAINWINDOW_H
