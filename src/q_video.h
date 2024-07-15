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
    QStringList getVideoDevices();
    QString getVideoFormats(const QString &device);
    QString getVideoDeviceDetails(const QString &device);

private:
    QStringList parseVideoOutput(const QString &output);
};

#endif // Q_VIDEO_H
