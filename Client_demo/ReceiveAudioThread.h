#ifndef RECEIVEAUDIOTHREAD_H
#define RECEIVEAUDIOTHREAD_H

#include <QObject>
#include <QUdpSocket>
#include <QDebug>
#include "DialogLive.h"
#include <QIODevice>
#include <QAudioInput>
#include <QAudioOutput>

class ReceiveAudioThread : public QObject
{
    Q_OBJECT
public:
    explicit ReceiveAudioThread(QUdpSocket *receiverAudio,unsigned short int port,QObject *parent = nullptr);


signals:

public slots:
    void onReadyRead();
private:
    unsigned short int port;
    QUdpSocket *receiverAudio;
    QHostAddress groupAddress;
    QAudioDeviceInfo deviceInfo;
    QIODevice *audioDevice;
    QAudioFormat audioFormat;
    QAudioOutput* audioOutput;


};

#endif // RECEIVEAUDIOTHREAD_H
