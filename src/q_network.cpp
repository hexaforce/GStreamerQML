// q_network.cpp
#include "q_network.h"
#include <QProcess>
#include <QRegularExpression>

Q_Network::Q_Network(QObject *parent) : QObject(parent) {}
#include <QProcess>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QProcessEnvironment>

QString Q_Network::getNetworkInfoAsJson()
{
    QProcess process;
    process.setProcessEnvironment(QProcessEnvironment::systemEnvironment());

    QProcessEnvironment env = process.processEnvironment();
    env.insert("LANG", "C");
    process.setProcessEnvironment(env);

    process.start("sudo", QStringList() << "lshw" << "-C" << "network");
    process.waitForFinished(-1);

    QByteArray output = process.readAllStandardOutput();
    QString result = QString::fromLocal8Bit(output);

    // Parse lshw output and construct JSON
    QJsonArray networkInterfaces;

    QStringList lines = result.split("\n");
    QJsonObject interfaceObject;
    QString logicalName;

    for (const QString &line : lines) {
        if (line.startsWith("  *-network")) {
            if (!interfaceObject.isEmpty()) {
                if (!logicalName.isEmpty()) {
                    // Add nmcli info
                    QProcess nmcliProcess;
                    nmcliProcess.setProcessEnvironment(env);
                    nmcliProcess.start("nmcli", QStringList() << "device" << "show" << logicalName);
                    nmcliProcess.waitForFinished(-1);
                    QByteArray nmcliOutput = nmcliProcess.readAllStandardOutput();
                    QString nmcliResult = QString::fromLocal8Bit(nmcliOutput);
                    QStringList nmcliLines = nmcliResult.split("\n");
                    QJsonObject nmcliObject;
                    for (const QString &nmcliLine : nmcliLines) {
                        QStringList nmcliParts = nmcliLine.split(':', Qt::SkipEmptyParts);
                        if (nmcliParts.size() < 2) continue;
                        QString nmcliKey = nmcliParts[0].trimmed();
                        QString nmcliValue = nmcliParts[1].trimmed();
                        nmcliObject[nmcliKey] = nmcliValue;
                    }
                    interfaceObject["nmcli_info"] = nmcliObject;

                    // Add udevadm info
                    QProcess udevadmProcess;
                    udevadmProcess.start("udevadm", QStringList() << "info" << "--query=all" << "--path=/sys/class/net/" + logicalName);
                    udevadmProcess.waitForFinished(-1);
                    QByteArray udevadmOutput = udevadmProcess.readAllStandardOutput();
                    QString udevadmResult = QString::fromLocal8Bit(udevadmOutput);
                    QStringList udevadmLines = udevadmResult.split("\n");
                    QJsonObject udevadmObject;
                    for (const QString &udevadmLine : udevadmLines) {
                        if (udevadmLine.startsWith("E: ")) {
                            QStringList udevadmParts = udevadmLine.mid(3).split('=');
                            if (udevadmParts.size() == 2) {
                                QString udevadmKey = udevadmParts[0].trimmed();
                                QString udevadmValue = udevadmParts[1].trimmed();
                                udevadmObject[udevadmKey] = udevadmValue;
                            }
                        }
                    }
                    interfaceObject["udevadm_info"] = udevadmObject;
                }
                networkInterfaces.append(interfaceObject);
            }
            interfaceObject = QJsonObject();
        }

        QStringList parts = line.split(':', Qt::SkipEmptyParts);
        if (parts.size() < 2) continue;

        QString key = parts[0].trimmed();
        QString value = parts[1].trimmed();

        if (key == "logical name") {
            logicalName = value;
        }

        if (key == "capabilities" || key == "configuration") {
            QJsonObject subObject;
            QStringList subParts = value.split(' ');
            for (const QString &subPart : subParts) {
                QStringList subKeyValue = subPart.split('=');
                if (subKeyValue.size() == 2) {
                    QString subKey = subKeyValue[0].trimmed();
                    QString subValue = subKeyValue[1].trimmed();
                    subObject[subKey] = subValue;
                }
            }
            interfaceObject[key] = subObject;
        } else {
            interfaceObject[key] = value;
        }
    }
    if (!interfaceObject.isEmpty()) {
        if (!logicalName.isEmpty()) {
            // Add nmcli info
            QProcess nmcliProcess;
            nmcliProcess.start("nmcli", QStringList() << "device" << "show" << logicalName);
            nmcliProcess.waitForFinished(-1);
            QByteArray nmcliOutput = nmcliProcess.readAllStandardOutput();
            QString nmcliResult = QString::fromLocal8Bit(nmcliOutput);
            QStringList nmcliLines = nmcliResult.split("\n");
            QJsonObject nmcliObject;
            for (const QString &nmcliLine : nmcliLines) {
                QStringList nmcliParts = nmcliLine.split(':', Qt::SkipEmptyParts);
                if (nmcliParts.size() < 2) continue;
                QString nmcliKey = nmcliParts[0].trimmed();
                QString nmcliValue = nmcliParts[1].trimmed();
                nmcliObject[nmcliKey] = nmcliValue;
            }
            interfaceObject["nmcli_info"] = nmcliObject;

            // Add udevadm info
            QProcess udevadmProcess;
            udevadmProcess.start("udevadm", QStringList() << "info" << "--query=all" << "--path=/sys/class/net/" + logicalName);
            udevadmProcess.waitForFinished(-1);
            QByteArray udevadmOutput = udevadmProcess.readAllStandardOutput();
            QString udevadmResult = QString::fromLocal8Bit(udevadmOutput);
            QStringList udevadmLines = udevadmResult.split("\n");
            QJsonObject udevadmObject;
            for (const QString &udevadmLine : udevadmLines) {
                if (udevadmLine.startsWith("E: ")) {
                    QStringList udevadmParts = udevadmLine.mid(3).split('=');
                    if (udevadmParts.size() == 2) {
                        QString udevadmKey = udevadmParts[0].trimmed();
                        QString udevadmValue = udevadmParts[1].trimmed();
                        udevadmObject[udevadmKey] = udevadmValue;
                    }
                }
            }
            interfaceObject["udevadm_info"] = udevadmObject;
        }
        networkInterfaces.append(interfaceObject);
    }

    QJsonObject jsonResult;
    jsonResult["network_interfaces"] = networkInterfaces;

    // Convert JSON object to a string with indented formatting
    QJsonDocument doc(jsonResult);
    return QString::fromUtf8(doc.toJson(QJsonDocument::Indented));
}

QString Q_Network::getIwconfigOutput()
{
  QProcess process;
  QString program = "iwconfig";
  QStringList arguments;

  process.start(program, arguments); // プログラムと引数リストを指定してプロセスを開始
  process.waitForFinished(-1);       // プロセスが終了するまで待機

  QString result = QString::fromLocal8Bit(process.readAllStandardOutput());
  return result;
}

QStringList Q_Network::getWifiDevices()
{
  QString output = getIwconfigOutput();
  return parseIwconfigOutput(output);
}

QStringList Q_Network::parseIwconfigOutput(const QString &output)
{
  QStringList devices;
  QStringList lines = output.split('\n');
  QRegularExpression regex("^\\S+\\s+IEEE 802\\.11"); // WIFIデバイスの行を検出する正規表現

  for (const QString &line : lines)
  {
    QRegularExpressionMatch match = regex.match(line);
    if (match.hasMatch())
    {
      QString device = line.split(' ')[0];
      devices.append(device);
    }
  }

  return devices;
}

QString Q_Network::getDeviceStatus(const QString &device)
{
  QProcess process;
  process.start("nmcli", QStringList() << "device" << "show" << device);
  process.waitForFinished(-1); // Wait until the process finishes

  QString result = QString::fromLocal8Bit(process.readAllStandardOutput());
  return result;
}

QString Q_Network::getDeviceSupport(const QString &device)
{
  QProcess process;
  QString result;

  // Execute iw command with the specified device
  process.start("iw", QStringList() << device << "info");
  if (!process.waitForStarted())
  {
    return "Error starting iw process";
  }
  if (!process.waitForFinished(-1))
  {
    return "Error while waiting for iw process to finish";
  }

  // Read the output of iw command
  result = QString::fromLocal8Bit(process.readAllStandardOutput());

  // Extract wiphy index using regular expression
  QRegularExpression regex("wiphy (\\d+)");
  QRegularExpressionMatch match = regex.match(result);
  if (!match.hasMatch())
  {
    return "Failed to extract wiphy index";
  }

  // Get the wiphy index value
  QString wiphyIndexStr = match.captured(1);
  bool ok;
  int wiphyIndex = wiphyIndexStr.toInt(&ok);
  if (!ok)
  {
    return "Failed to convert wiphy index to integer";
  }

  // Execute iw phy info based on wiphy index
  process.start("iw", QStringList() << QString("phy%1").arg(wiphyIndex) << "info");
  if (!process.waitForStarted())
  {
    return "Error starting iw phy process";
  }
  if (!process.waitForFinished(-1))
  {
    return "Error while waiting for iw phy process to finish";
  }

  // Append the output of iw phy info to the result
  result += "\n\n" + QString::fromLocal8Bit(process.readAllStandardOutput());

  return result;
}