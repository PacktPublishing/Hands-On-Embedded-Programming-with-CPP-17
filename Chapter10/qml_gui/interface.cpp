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

#include "interface.h"
#include <QtGui/QTouchDevice>
#include <QDebug>
#include <QFile>
#include <QtMath>

QmlInterface::QmlInterface(QObject *parent) : QObject(parent) {
    // Set track for media player.
    mediaPlayer.setMedia(QUrl("qrc:/music/tiltshifted_lost_neon_sun.mp3"));
    
    // Load magnitude file for the audio track.
    QFile magFile(":/music/visualization.raw", this);
    magFile.open(QFile::ReadOnly);
    magnitudeArray = magFile.readAll();
    
    // Media player connections.
    connect(&mediaPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(mediaStatusChanged(QMediaPlayer::MediaStatus)));
    connect(&mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
    connect(&mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
}


bool QmlInterface::isHoverEnabled() const {
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID) || defined(Q_OS_QNX) || defined(Q_OS_WINRT)
    return false;
#else
    bool isTouch = false;
    foreach (const QTouchDevice *dev, QTouchDevice::devices()) {
        if (dev->type() == QTouchDevice::TouchScreen) {
            isTouch = true;
            break;
        }
    }
    
    bool isMobile = false;
    if (qEnvironmentVariableIsSet("QT_QUICK_CONTROLS_MOBILE")) {
        isMobile = true;
    }
    
    return !isTouch && !isMobile;
#endif
}


void QmlInterface::setPlaying() {
	mediaPlayer.play();
}


void QmlInterface::setStopped() {
	mediaPlayer.stop();
}


void QmlInterface::setPaused() {
	mediaPlayer.pause();
}

void QmlInterface::mediaStatusChanged(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::EndOfMedia) {
        emit stopped();
    }
}

void QmlInterface::durationChanged(qint64 duration) {
    qDebug() << "Duration changed: " << duration;
    
    durationTotal = formatDuration(duration);
    durationLeft = "-" + durationTotal;
    trackDuration = duration;
    emit start();
    emit durationTotalChanged();
    emit durationLeftChanged();
}

void QmlInterface::positionChanged(qint64 position) {
    qDebug() << "Position changed: " << position;
    durationLeft = "-" + formatDuration((trackDuration - position));
    emit durationLeftChanged();
}

qint64 QmlInterface::duration() {
    qDebug() << "Returning duration value: " << mediaPlayer.duration();
    return mediaPlayer.duration();
}

qint64 QmlInterface::position() {
    qDebug() << "Returning position value: " << mediaPlayer.position();
    return mediaPlayer.position();
}

double QmlInterface::getNextAudioLevel(int offsetMs) {
    // Calculate the integer index position in to the magnitude array
    qint64 index = ((mediaPlayer.position() + offsetMs) / millisecondsPerBar) | 0;

    if (index < 0 || index >= (magnitudeArray.length() / 2)) { 
        return 0.0; 
    }

    return (((quint16*) magnitudeArray.data())[index] / 63274.0);
}

QString QmlInterface::formatDuration(qint64 milliseconds) {
    qint64 minutes = floor(milliseconds / 60000);
    milliseconds -= minutes * 60000;
    qint64 seconds = milliseconds / 1000;
    seconds = round(seconds);
    if (seconds < 10) {
        return QString::number(minutes) + ":0" + QString::number(seconds);
    }
    else {
        return QString::number(minutes) + ":" + QString::number(seconds);
    }
}
