#ifndef VIDEOSTREAMINFO_H
#define VIDEOSTREAMINFO_H

#include <QObject>
#include <QString>
#include <QStringList>

class VideoStreamInfo : public QObject
{
    Q_OBJECT
public:
    explicit VideoStreamInfo(QObject *parent = nullptr);

public slots:
    QString getVideoStreamOutput();
    QStringList getVideoDevices();
    QString getVideoFormats(const QString &device);
    QString getVideoDeviceDetails(const QString &device);

private:
    QStringList parseVideoOutput(const QString &output);
};

#endif // VIDEOSTREAMINFO_H
