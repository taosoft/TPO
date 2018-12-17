#ifndef MAINWINDOW_H
    #define MAINWINDOW_H

    #include <QMainWindow>
    #include <QtCharts/QCategoryAxis>
    #include <QtCharts/QtCharts>
    #include <QtCharts/QLineSeries>
    #include <QTimer>
    #include <QVector>

    #include <QTcpSocket>

    using namespace QtCharts;

    namespace Ui {
    class MainWindow;
    }

    class QTcpSocket;

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:

        //Botonera de temperatura deseada
        void on_btnTempMas_pressed();
        void doIncrement();
        void on_btnTempMas_released();
        void on_btnTempMenos_pressed();
        void doDecrement();
        void on_btnTempMenos_released();

        //Chart real time
        void realtime(double);

        //Tcp Client
        void on_btnConnectToServer_clicked();
        void on_ClientConnected();
        void on_ClientDisconnected();
        void on_ClientStateChange(QAbstractSocket::SocketState);
        void on_ClientError(QAbstractSocket::SocketError);        
        void on_btnConfigurarRTCStick_clicked();
        void on_btnDisconnectFromServer_clicked();

        //Chart historial en SD
        void on_btnLeerTarjetaSD_clicked();
        void on_btnBorrarTarjetaSD_clicked();

    private:
        Ui::MainWindow *ui;

        //Temperatura
        int isFloat(double temp);
        bool sendNewConfigTemp(void);
        void checkLcdFormat();

        //Botonera temperatura
        QTimer *timerIncrement;
        QTimer *timerDecrement;
        int timerTimeout;

        //Grafico en tiempo real
        QChart *chart;
        QLineSeries *series;

        //Gráfico historial SD
        QChart *chartHSD;
        QLineSeries *seriesHSD;
        QVector<QPointF> seriesData;
        QTimer timer;

        //Cliente TCP
        QTcpSocket *mSocket;
        void analyzeRcvData(QString data);
    };

#endif // MAINWINDOW_H
