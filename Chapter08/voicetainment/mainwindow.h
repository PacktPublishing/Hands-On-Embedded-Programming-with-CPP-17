#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QAudioRecorder>
#include <QAudioProbe>
#include <QMediaPlayer>
//#include <QtDBus/QtDBus>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
public slots:
    void playBluetooth();
    void stopBluetooth();
    void playOnlineStream();
    void stopOnlineStream();
    void playLocalFile();
    void stopLocalFile();
    void recordMessage();
    void playMessage();
    
    void errorString(QString err);
    
    void quit();
    
private:
    Ui::MainWindow *ui;
    
    QMediaPlayer* player;
    QAudioRecorder* audioRecorder;
    QAudioProbe* audioProbe;
    
    qint64 silence; // Microseconds of silence recorded so far.
    
private slots:
    void processBuffer(QAudioBuffer);
};

#endif // MAINWINDOW_H
