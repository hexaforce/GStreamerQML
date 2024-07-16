#ifndef Q_VIDEO_H
#define Q_VIDEO_H

#include <QObject>
#include <QString>
#include <QStringList>

class Q_Video : public QObject
{
    Q_OBJECT
public:
    explicit Q_Video(QObject *parent = nullptr);

public slots:
    QString getVideoStreamOutput();
    // QStringList getVideoDevices();
    // // QStringList getDeviceList2();
    // QString getVideoFormats(const QString &device);
    // QString getVideoDeviceDetails(const QString &device);

private:
    // QStringList parseVideoOutput(const QString &output);
    // // QJsonArray parseDeviceOutput2(const QByteArray &output)
};

#endif // Q_VIDEO_H
