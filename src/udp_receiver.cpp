#include "udp_receiver.h"

UdpReceiver::UdpReceiver(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
}

UdpReceiver::~UdpReceiver()
{
    udpSocket->close();
    delete udpSocket;
}

void UdpReceiver::startListening(int port)
{
    udpSocket->bind(QHostAddress::Any, port);
    connect(udpSocket, &QUdpSocket::readyRead, this, &UdpReceiver::processPendingDatagrams);
}

void UdpReceiver::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());

        QString message = QString::fromUtf8(datagram);
        emit messageReceived(message);
    }
}
