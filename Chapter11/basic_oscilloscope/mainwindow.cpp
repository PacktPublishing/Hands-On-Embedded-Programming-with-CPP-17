#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPortInfo>
#include <QInputDialog>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), 
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    
    // Menu connections.
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(quit()));
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(connectUart()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(disconnectUart()));
    connect(ui->actionInfo, SIGNAL(triggered()), this, SLOT(about()));
    
    // Other connections
    connect(&serialPort, SIGNAL(readyRead()), this, SLOT(uartReady()));
    
    // Configure the chart view.
    QChart* chart = ui->chartView->chart();
    chart->setTheme(QChart::ChartThemeBlueIcy);
    chart->createDefaultAxes();
    series = new QtCharts::QLineSeries(chart);
    chart->setAnimationOptions(QChart::NoAnimation);        
    chart->addSeries(series);
}


MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::connectUart() {
    // Get the list of available COM ports on the system, then display it to the
    // user for selecting. 
    QList<QSerialPortInfo> comInfo = QSerialPortInfo::availablePorts();
    QStringList comNames;
    for (QSerialPortInfo com: comInfo) {
        comNames.append(com.portName());
    }
    
    if (comNames.size() < 1) {
        QMessageBox::warning(this, tr("No serial port found"), tr("No serial port was found on the system. Please check all connections and try again."));
        return;
    }
    
    QString comPort = QInputDialog::getItem(this, tr("Select serial port"), tr("Available ports:"), comNames, 0, false);
    
    if (comPort.isEmpty()) { return; }
    
    serialPort.setPortName(comPort);
    if (!serialPort.open(QSerialPort::ReadOnly)) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to open the serial port."));
        return;
    }
    
    // Baud rate: 19,200, 8N1
    serialPort.setBaudRate(19200);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setDataBits(QSerialPort::Data8);
}


void MainWindow::disconnectUart() {
    serialPort.close();
}


void MainWindow::uartReady() {
    QByteArray data = serialPort.readAll();
    
    // Each byte is a value. Add it to the series, keeping it below the max
    // size. X: time base, Y: amplitude.
    for (qint8 value: data) {
        series->append(counter++, value);
    }
}


void MainWindow::about() {
    QMessageBox::aboutQt(this, tr("About"));
}


void MainWindow::quit() {
    exit(0);
}
