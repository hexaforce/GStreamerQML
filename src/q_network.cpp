// q_network.cpp
#include "q_network.h"
#include <QProcess>
#include <QRegularExpression>

Q_Network::Q_Network(QObject *parent) : QObject(parent) {}

QString Q_Network::getIwconfigOutput() {
  QProcess process;
  process.start("iwconfig", QStringList() << "");
  process.waitForFinished(-1); // Wait until the process finishes

  QString result = QString::fromLocal8Bit(process.readAllStandardOutput());
  return result;
}

QStringList Q_Network::getWifiDevices() {
  QString output = getIwconfigOutput();
  return parseIwconfigOutput(output);
}

QStringList Q_Network::parseIwconfigOutput(const QString &output) {
  QStringList        devices;
  QStringList        lines = output.split('\n');
  QRegularExpression regex("^\\S+\\s+IEEE 802\\.11"); // WIFIデバイスの行を検出する正規表現

  for (const QString &line : lines) {
    QRegularExpressionMatch match = regex.match(line);
    if (match.hasMatch()) {
      QString device = line.split(' ')[0];
      devices.append(device);
    }
  }

  return devices;
}

QString Q_Network::getDeviceStatus(const QString &device) {
  QProcess process;
  process.start("nmcli", QStringList() << "device" << "show" << device);
  process.waitForFinished(-1); // Wait until the process finishes

  QString result = QString::fromLocal8Bit(process.readAllStandardOutput());
  return result;
}


QString Q_Network::getDeviceSupport(const QString &device) {
    QProcess process;
    QString result;

    // Execute iw command with the specified device
    process.start("iw", QStringList() << device << "info");
    if (!process.waitForStarted()) {
        return "Error starting iw process";
    }
    if (!process.waitForFinished(-1)) {
        return "Error while waiting for iw process to finish";
    }

    // Read the output of iw command
    result = QString::fromLocal8Bit(process.readAllStandardOutput());

    // Extract wiphy index using regular expression
    QRegularExpression regex("wiphy (\\d+)");
    QRegularExpressionMatch match = regex.match(result);
    if (!match.hasMatch()) {
        return "Failed to extract wiphy index";
    }

    // Get the wiphy index value
    QString wiphyIndexStr = match.captured(1);
    bool ok;
    int wiphyIndex = wiphyIndexStr.toInt(&ok);
    if (!ok) {
        return "Failed to convert wiphy index to integer";
    }

    // Execute iw phy info based on wiphy index
    process.start("iw", QStringList() << QString("phy%1").arg(wiphyIndex) << "info");
    if (!process.waitForStarted()) {
        return "Error starting iw phy process";
    }
    if (!process.waitForFinished(-1)) {
        return "Error while waiting for iw phy process to finish";
    }

    // Append the output of iw phy info to the result
    result += "\n\n" + QString::fromLocal8Bit(process.readAllStandardOutput());

    return result;
}