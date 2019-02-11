#ifndef VOICEINPUT_H
#define VOICEINPUT_H

#include <QObject>
#include <QAudioInput>

extern "C" {
#include "pocketsphinx.h"
}

class VoiceInput : public QObject {
    Q_OBJECT
    
    QAudioInput* audioInput;
    QIODevice* audioDevice;
    bool state;
    
public:
    explicit VoiceInput(QObject *parent = nullptr);
    bool checkState() { return state; }
    
signals:
    void playBluetooth();
    void stopBluetooth();
    void playLocal();
    void stopLocal();
    void playRemote();
    void stopRemote();
    void recordMessage();
    void playMessage();
    
    void error(QString err);
    void finished();
    
public slots:
    void run();
};

#endif // VOICEINPUT_H
