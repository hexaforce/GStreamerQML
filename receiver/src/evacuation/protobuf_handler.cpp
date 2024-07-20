#include "protobuf_handler.h"

ProtobufHandler::ProtobufHandler(QObject *parent) : QObject(parent) {}

QString ProtobufHandler::receivedMessage() const { return m_receivedMessage; }

void ProtobufHandler::receiveData(const QString &data) {
  // Simulate protobuf data processing
  QByteArray byteArray = QByteArray::fromBase64(data.toUtf8());
  m_receivedMessage = QString::fromUtf8(byteArray);

  emit messageReceived();
}
