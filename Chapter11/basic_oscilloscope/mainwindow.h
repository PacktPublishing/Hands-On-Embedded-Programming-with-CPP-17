#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSerialPort>
#include <QChartView>
#include <QLineSeries>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
public slots:
    void connectUart();
    void disconnectUart();
    void about();
    void quit();
    
private:
    Ui::MainWindow *ui;
    
    QSerialPort serialPort;
    QtCharts::QLineSeries* series;
    quint64 counter = 0;
    
private slots:
    void uartReady();
};

#endif // MAINWINDOW_H
