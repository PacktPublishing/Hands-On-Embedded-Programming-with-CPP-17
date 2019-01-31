/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef INTERFACE_H
#define INTERFACE_H

#include <QtCore/QObject>
#include <QMediaPlayer>
#include <QByteArray>


class QmlInterface : public QObject {
    Q_OBJECT    
    Q_PROPERTY(QString durationTotal READ getDurationTotal NOTIFY durationTotalChanged)
    Q_PROPERTY(QString durationLeft READ getDurationLeft NOTIFY durationLeftChanged)
    
    QString formatDuration(qint64 milliseconds);
    
    QMediaPlayer mediaPlayer;
    QByteArray magnitudeArray;
    const int millisecondsPerBar = 68;
    QString durationTotal;
    QString durationLeft;
    qint64 trackDuration;
    
public:
    explicit QmlInterface(QObject *parent = nullptr);

    Q_INVOKABLE bool isHoverEnabled() const;
    Q_INVOKABLE void setPlaying();
	Q_INVOKABLE void setStopped();
	Q_INVOKABLE void setPaused();
    Q_INVOKABLE qint64 duration();
    Q_INVOKABLE qint64 position();
    Q_INVOKABLE double getNextAudioLevel(int offsetMs);
    
    QString getDurationTotal() { return durationTotal; }
    QString getDurationLeft() { return durationLeft; }

public slots:
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);
    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);
    
signals:
    void start();
    void stopped();
    void paused();
    void playing();
    void durationTotalChanged();
    void durationLeftChanged();
};

#endif
