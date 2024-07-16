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

// QStringList Q_Video::getVideoDevices()
// {
//     QString output = getVideoStreamOutput();
//     return parseVideoOutput(output);
// }

// QStringList Q_Video::parseVideoOutput(const QString &output)
// {
//     QStringList videos;
//     QStringList lines = output.split('\n');

//     for (const QString &line : lines)
//     {
//         if (line.contains("/dev/"))
//         {
//             QString device = line.trimmed();
//             // デバイスパスだけを追加
//             if (!device.contains("media"))
//             { // /dev/media* デバイスを除外
//                 videos.append(device);
//             }
//         }
//     }
//     return videos;
// }

// QString Q_Video::getVideoFormats(const QString &device)
// {
//     QProcess process;
//     process.start("v4l2-ctl", QStringList() << "--list-formats-ext" << "-d" << device);

//     if (!process.waitForFinished(5000))
//     { // 5000ミリ秒 = 5秒
//         qWarning() << "Process timed out!";
//         return QString();
//     }

//     QString result = QString::fromLocal8Bit(process.readAllStandardOutput());
//     return result;
// }

// QString Q_Video::getVideoDeviceDetails(const QString &device)
// {
//     QProcess process;
//     process.start("v4l2-ctl", QStringList() << "-d" << device << "--all");

//     if (!process.waitForFinished(5000))
//     { // 5000ミリ秒 = 5秒
//         qWarning() << "Process timed out!";
//         return QString();
//     }

//     QString result = QString::fromLocal8Bit(process.readAllStandardOutput());
//     return result;
// }
