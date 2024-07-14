#ifndef PROTOBUF_HANDLER_H
#define PROTOBUF_HANDLER_H

#include <QObject>
#include <QByteArray>
#include <QString>

class ProtobufHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString receivedMessage READ receivedMessage NOTIFY messageReceived)

public:
    explicit ProtobufHandler(QObject *parent = nullptr);

    QString receivedMessage() const;

public slots:
    void receiveData(const QString &data);

signals:
    void messageReceived();

private:
    QString m_receivedMessage;
};

#endif // PROTOBUF_HANDLER_H
