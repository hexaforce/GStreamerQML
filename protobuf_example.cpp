// protobuf_example.cpp
#include "protobuf_example.h"

ProtobufHandler::ProtobufHandler(QObject *parent)
    : QObject(parent)
{
}

void ProtobufHandler::receiveData(const QByteArray &data)
{
    Data message;
    message.ParseFromArray(data.constData(), data.size());

    // Emit signal with received message
    emit dataReceived(QString::fromStdString(message.message()));
}
