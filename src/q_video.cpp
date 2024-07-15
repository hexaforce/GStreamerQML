#include "q_video.h"
#include <QProcess>
#include <QDebug>

Q_Video::Q_Video(QObject *parent) : QObject(parent) {}

QString Q_Video::getVideoStreamOutput()
{
    QProcess process;
    process.start("v4l2-ctl", QStringList() << "--list-devices");

    // タイムアウトを設定してプロセスが終了するのを待つ
    if (!process.waitForFinished(5000))
    { // 5000ミリ秒 = 5秒
        qWarning() << "Process timed out!";
        return QString();
    }

    QString result = QString::fromLocal8Bit(process.readAllStandardOutput());
    return result;
}

QStringList Q_Video::getVideoDevices()
{
    QString output = getVideoStreamOutput();
    return parseVideoOutput(output);
}

QStringList Q_Video::parseVideoOutput(const QString &output)
{
    QStringList videos;
    QStringList lines = output.split('\n');

    for (const QString &line : lines)
    {
        if (line.contains("/dev/"))
        {
            QString device = line.trimmed();
            // デバイスパスだけを追加
            if (!device.contains("media"))
            { // /dev/media* デバイスを除外
                videos.append(device);
            }
        }
    }
    return videos;
}

QString Q_Video::getVideoFormats(const QString &device)
{
    QProcess process;
    process.start("v4l2-ctl", QStringList() << "--list-formats-ext" << "-d" << device);

    if (!process.waitForFinished(5000))
    { // 5000ミリ秒 = 5秒
        qWarning() << "Process timed out!";
        return QString();
    }

    QString result = QString::fromLocal8Bit(process.readAllStandardOutput());
    return result;
}

QString Q_Video::getVideoDeviceDetails(const QString &device)
{
    QProcess process;
    process.start("v4l2-ctl", QStringList() << "-d" << device << "--all");

    if (!process.waitForFinished(5000))
    { // 5000ミリ秒 = 5秒
        qWarning() << "Process timed out!";
        return QString();
    }

    QString result = QString::fromLocal8Bit(process.readAllStandardOutput());
    return result;
}

// QJsonArray Q_Video::getDeviceList2()
// {
//     QProcess process;
//     QStringList arguments;
//     arguments << "gst-device-monitor-1.0" << "Video/Source";

//     process.start("gst-device-monitor-1.0", arguments);
//     if (!process.waitForStarted())
//     {
//         qWarning() << "Failed to start gst-device-monitor-1.0";
//         return QJsonArray();
//     }

//     if (!process.waitForFinished())
//     {
//         qWarning() << "Process timed out or failed to finish";
//         return QJsonArray();
//     }

//     QByteArray result = process.readAllStandardOutput();
//     return parseDeviceOutput2(result);
// }

// QJsonArray Q_Video::parseDeviceOutput2(const QByteArray &output)
// {
//     QJsonArray devices;

//     QString outputStr = QString::fromLocal8Bit(output);
//     QStringList lines = outputStr.split("\n", QString::SkipEmptyParts);

//     QString currentDeviceName;
//     QJsonObject currentDevice;

//     for (const QString &line : lines)
//     {
//         if (line.startsWith("Device found:"))
//         {
//             if (!currentDevice.isEmpty())
//             {
//                 devices.append(currentDevice);
//                 currentDevice = QJsonObject();
//             }
//             currentDeviceName = line.mid(13).trimmed();
//             currentDevice["name"] = currentDeviceName;
//         }
//         else if (line.startsWith("class :"))
//         {
//             currentDevice["class"] = line.mid(7).trimmed();
//         }
//         else if (line.startsWith("caps  :"))
//         {
//             QStringList capsList = line.mid(7).trimmed().split("\t", QString::SkipEmptyParts);
//             QJsonArray capsArray;
//             for (const QString &caps : capsList)
//             {
//                 QJsonObject capsObj;
//                 QStringList capsLines = caps.split(", ");
//                 for (const QString &capLine : capsLines)
//                 {
//                     QStringList capParts = capLine.split("=");
//                     if (capParts.size() == 2)
//                     {
//                         capsObj[capParts[0].trimmed()] = capParts[1].trimmed();
//                     }
//                     else
//                     {
//                         capsObj[capLine.trimmed()] = true;
//                     }
//                 }
//                 capsArray.append(capsObj);
//             }
//             currentDevice["caps"] = capsArray;
//         }
//     }

//     if (!currentDevice.isEmpty())
//     {
//         devices.append(currentDevice);
//     }

//     return devices;
// }