#include "ReceiveAudioThread.h"

ReceiveAudioThread::ReceiveAudioThread(QUdpSocket *receiverAudio,unsigned short int port,QObject *parent)
    : QObject(parent),receiverAudio(receiverAudio),port(port)
{    
    groupAddress.setAddress("239.88.88.88");
    receiverAudio->bind(QHostAddress::AnyIPv4,port,QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress);
    receiverAudio->joinMulticastGroup(groupAddress);


    audioFormat.setSampleRate(8000);
    audioFormat.setChannelCount(1);
    audioFormat.setSampleSize(16);
    audioFormat.setCodec("audio/pcm");
    audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    audioFormat.setSampleType(QAudioFormat::SignedInt);
    audioOutput = new QAudioOutput(audioFormat);
    audioDevice = audioOutput->start();

    connect(receiverAudio,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
}

void ReceiveAudioThread::onReadyRead()
{
    qDebug()<<"do";
    UPacket pack;
    memset(&pack, 0, sizeof(UPacket));
    receiverAudio->readDatagram((char*)&pack,sizeof(UPacket));
    audioDevice->write(pack.data,pack.datalen);
}
