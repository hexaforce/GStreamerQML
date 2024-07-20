#include "common_telemetry_socket.h"

CommonTelemetrySocket::CommonTelemetrySocket(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
}

CommonTelemetrySocket::~CommonTelemetrySocket()
{
    udpSocket->close();
    delete udpSocket;
}

void CommonTelemetrySocket::startListening(int port)
{
    udpSocket->bind(QHostAddress::Any, port);
    connect(udpSocket, &QUdpSocket::readyRead, this, &CommonTelemetrySocket::processPendingDatagrams);
}

void CommonTelemetrySocket::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());

        QString message = QString::fromUtf8(datagram);
        emit messageReceived(message);
    }
}

void CommonTelemetrySocket::messageSend(const QString &message, const QString &host, int port)
{
    udpSocket->writeDatagram(message.toUtf8(), QHostAddress(host), port);
}
