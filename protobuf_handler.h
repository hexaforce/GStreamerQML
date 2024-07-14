#ifndef PROTOBUF_HANDLER_H
#define PROTOBUF_HANDLER_H

#include <QObject>
#include <QString>
#include <QByteArray>

// Forward declaration of Data class (assuming it's defined elsewhere)
class Data;

class ProtobufHandler : public QObject
{
    Q_OBJECT
public:
    explicit ProtobufHandler(QObject *parent = nullptr);
    virtual ~ProtobufHandler();

    // Example function to receive and process Protobuf data
    Q_INVOKABLE void receiveData(const QByteArray &data);

signals:
    // Signal to notify about received data
    void dataReceived(const QString &message);

private:
    // Private members and methods
};

#endif // PROTOBUF_HANDLER_H
