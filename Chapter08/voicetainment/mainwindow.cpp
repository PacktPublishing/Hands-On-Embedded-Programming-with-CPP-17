#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "voiceinput.h"

#include <QThread>
#include <QMessageBox>

#include <cmath>


#define MSG_RECORD_MAX_SILENCE_US 5000000



// --- CONSTRUCTOR ---
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), 
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    
    // Set up menu connections.
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(quit()));
    
    // Set up UI connections.
    connect(ui->playBluetoothButton, SIGNAL(pressed), this, SLOT(playBluetooth));
    connect(ui->stopBluetoothButton, SIGNAL(pressed), this, SLOT(stopBluetooth));
    connect(ui->playLocalAudioButton, SIGNAL(pressed), this, SLOT(playLocalFile));
    connect(ui->stopLocalAudioButton, SIGNAL(pressed), this, SLOT(stopLocalFile));
    connect(ui->playOnlineStreamButton, SIGNAL(pressed), this, SLOT(playOnlineStream));
    connect(ui->stopOnlineStreamButton, SIGNAL(pressed), this, SLOT(stopOnlineStream));
    connect(ui->recordMessageButton, SIGNAL(pressed), this, SLOT(recordMessage));
    connect(ui->playBackMessage, SIGNAL(pressed), this, SLOT(playMessage));
    
    // Defaults
    silence = 0;
    
    // Check that we have a DBus session available to us.
//    if (!QDBusConnection::sessionBus().isConnected()) {
//        fprintf(stderr, "Cannot connect to the D-Bus session bus.\n"
//                "To start it, run:\n"
//                "\teval `dbus-launch --auto-syntax`\n");
//        return;
//    }
        
    // Create the audio interface instances.
    player = new QMediaPlayer(this);
    audioRecorder = new QAudioRecorder(this);
    audioProbe = new QAudioProbe(this);
    
    // Configure the audio recorder.
    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/amr");
    audioSettings.setQuality(QMultimedia::HighQuality);    
    audioRecorder->setEncodingSettings(audioSettings);    
    audioRecorder->setOutputLocation(QUrl::fromLocalFile("message/last_message.amr"));
    
    // Configure audio probe.
    connect(audioProbe, SIGNAL(audioBufferProbed(QAudioBuffer)), this, SLOT(processBuffer(QAudioBuffer)));
    audioProbe->setSource(audioRecorder);
    
    // Start the voice interface in its own thread and set up the connections.
    QThread* thread = new QThread;
    VoiceInput* vi = new VoiceInput();
    vi->moveToThread(thread);
    connect(thread, SIGNAL(started()), vi, SLOT(run()));
    connect(vi, SIGNAL(finished()), thread, SLOT(quit()));
    connect(vi, SIGNAL(finished()), vi, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    
    connect(vi, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(vi, SIGNAL(playBluetooth), this, SLOT(playBluetooth));
    connect(vi, SIGNAL(stopBluetooth), this, SLOT(stopBluetooth));
    connect(vi, SIGNAL(playLocal), this, SLOT(playLocalFile));
    connect(vi, SIGNAL(stopLocal), this, SLOT(stopLocalFile));
    connect(vi, SIGNAL(playRemote), this, SLOT(playOnlineStream));
    connect(vi, SIGNAL(stopRemote), this, SLOT(stopOnlineStream));
    connect(vi, SIGNAL(recordMessage), this, SLOT(recordMessage));
    connect(vi, SIGNAL(playMessage), this, SLOT(playMessage));
    
    thread->start();
}


// --- DESTRUCTOR ---
MainWindow::~MainWindow() {
    delete ui;
}


// --- PLAY BLUETOOTH ---
void MainWindow::playBluetooth() {
    // Use the link with the BlueZ Bluetooth stack in the Linux kernel to 
    // configure it to act as an A2DP sink for smartphones to connect to.
//    QDBusInterface iface("org.bluez", "/", "", QDBusConnection::sessionBus());
//    if (iface.isValid()) {
//        QDBusReply<QString> reply = iface.call("ping", argc > 1 ? argv[1] : "");
//        if (reply.isValid()) {
//            printf("Reply was: %s\n", qPrintable(reply.value()));
//            return 0;
//        }

//        fprintf(stderr, "Call failed: %s\n", qPrintable(reply.error().message()));
//        return 1;
//    }
}


// --- STOP BLUETOOTH ---
void MainWindow::stopBluetooth() {
    // 
    
    
}


// --- PLAY ONLINE STREAM ---
void MainWindow::playOnlineStream() {
    // Connect to remote streaming service's API and start streaming.
    // TODO: will not be implemented, just add dummy.
}


// --- STOP ONLINE STREAM ---
void MainWindow::stopOnlineStream() {
    // Stop streaming from remote service.
    // TODO: will not be implemented, just add dummy.
}


// --- PLAY LOCAL FILE ---
void MainWindow::playLocalFile() {
    // Load local audio file if found, then play it back.
    // We assume that these files are either found in a sub-folder called 
    // 'music' or on a USB stick's root directory.
    player->setMedia(QUrl::fromLocalFile("music/coolsong.mp3"));
    player->setVolume(50);
    player->play();
}


// --- STOP LOCAL FILE ---
void MainWindow::stopLocalFile() {
    player->stop();
}


// --- RECORD MESSAGE ---
void MainWindow::recordMessage() {
    // Open a recorder instance, record until stopped. A previous message is
    // overwritten.
    // Write the message to 'message/' sub-folder.
    audioRecorder->record();
}


// --- PLAY MESSAGE ---
void MainWindow::playMessage() {
    // Play back the last recorded message.
    player->setMedia(QUrl::fromLocalFile("message/last_message.arm"));
    player->setVolume(50);
    player->play();
}


// --- PROCESS BUFFER ---
void MainWindow::processBuffer(QAudioBuffer buffer) {
    // Detect silence in the audio samples. Stop the recording if at least
    // five seconds of silence are detected.
    const quint16 *data = buffer.constData<quint16>();
    
    // Get RMS of buffer, if silence, add its duration to the counter.
    int samples = buffer.sampleCount();
    double sumsquared = 0;
    for (int i = 0; i < samples; i++) {
        sumsquared += data[i] * data[i];
    }
    
    double rms = sqrt((double(1) / samples)*(sumsquared));
    
    if (rms <= 100) {
        silence += buffer.duration();
    }
    
    // If five seconds or more of silence, stop the recording and reset the
    // counter.
    if (silence >= MSG_RECORD_MAX_SILENCE_US) {
        silence = 0;
        audioRecorder->stop();
    }
}


// --- ERROR STRING ---
void MainWindow::errorString(QString err) {
    QMessageBox::critical(this, tr("Error"), err);
}


// --- QUIT ---
void MainWindow::quit() {
    exit(0);
}
