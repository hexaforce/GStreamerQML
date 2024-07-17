// q_network.cpp
#include "q_network.h"
#include <QProcess>
#include <QRegularExpression>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QProcessEnvironment>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QProcessEnvironment>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QFile>
#include <QTextStream>

Q_Network::Q_Network(QObject *parent) : QObject(parent) {}

QString Q_Network::getSystemctlStatus(const QString &serviceName) {
    QProcess process;
    process.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    process.start("sudo", QStringList() << "systemctl" << "status" << serviceName);
    process.waitForFinished(-1);

    QString result = QString::fromLocal8Bit(process.readAllStandardOutput());
    QJsonObject jsonObject;
    QStringList lines = result.split("\n");
    for (const QString &line : lines) {
        QString trimmedLine = line.trimmed();
        if (trimmedLine.isEmpty()) continue;

        int colonIndex = trimmedLine.indexOf(':');
        if (colonIndex != -1) {
            QString key = trimmedLine.left(colonIndex).trimmed();
            QString value = trimmedLine.mid(colonIndex + 1).trimmed();
            jsonObject[key] = value;
        } else {
            jsonObject[trimmedLine] = QString();
        }
    }
    return QString::fromUtf8(QJsonDocument(jsonObject).toJson(QJsonDocument::Indented));
}

QJsonArray Q_Network::parseIptablesChain(const QString &chainName, const QStringList &lines) {
    QJsonArray chainArray;
    QRegularExpression re(R"(\s*(\d+)\s+(\d+)\s+(\d+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+))");

    for (const QString &line : lines) {
        QRegularExpressionMatch match = re.match(line);
        if (match.hasMatch()) {
            QJsonObject rule;
            rule["num"] = match.captured(1);
            rule["pkts"] = match.captured(2);
            rule["bytes"] = match.captured(3);
            rule["target"] = match.captured(4);
            rule["prot"] = match.captured(5);
            rule["opt"] = match.captured(6);
            rule["in"] = match.captured(7);
            rule["out"] = match.captured(8);
            rule["source"] = match.captured(9);
            rule["destination"] = match.captured(10);
            chainArray.append(rule);
        }
    }

    return chainArray;
}

QJsonObject Q_Network::getIptablesStatus() {
    QProcess process;
    process.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    process.start("sudo", QStringList() << "iptables" << "-L" << "-n" << "-v" << "--line-numbers");
    process.waitForFinished(-1);

    QString result = QString::fromLocal8Bit(process.readAllStandardOutput());
    QStringList lines = result.split("\n");

    QJsonObject iptablesJson;
    QJsonArray inputChain, forwardChain, outputChain;
    QString currentChain;

    for (const QString &line : lines) {
        if (line.startsWith("Chain INPUT")) {
            currentChain = "INPUT";
            continue;
        } else if (line.startsWith("Chain FORWARD")) {
            currentChain = "FORWARD";
            continue;
        } else if (line.startsWith("Chain OUTPUT")) {
            currentChain = "OUTPUT";
            continue;
        } else if (line.startsWith("Chain DOCKER")) {
            currentChain.clear();
            continue;
        }

        if (currentChain == "INPUT") {
            inputChain.append(parseIptablesChain(currentChain, { line }));
        } else if (currentChain == "FORWARD") {
            forwardChain.append(parseIptablesChain(currentChain, { line }));
        } else if (currentChain == "OUTPUT") {
            outputChain.append(parseIptablesChain(currentChain, { line }));
        }
    }

    iptablesJson["INPUT"] = inputChain;
    iptablesJson["FORWARD"] = forwardChain;
    iptablesJson["OUTPUT"] = outputChain;

    return iptablesJson;
}

QString Q_Network::getCombinedStatus() {
    QJsonObject combinedJson;
    combinedJson["hostapd_status"] = QJsonDocument::fromJson(getSystemctlStatus("hostapd").toUtf8()).object();
    combinedJson["hostapd_conf"] = readConfFile("/etc/hostapd/hostapd.conf");
    combinedJson["dnsmasq_status"] = QJsonDocument::fromJson(getSystemctlStatus("dnsmasq").toUtf8()).object();
    combinedJson["dnsmasq_conf"] = readConfFile("/etc/dnsmasq.conf");
    combinedJson["ufw_status"] = QJsonDocument::fromJson(getSystemctlStatus("ufw").toUtf8()).object();
    combinedJson["iptables_status"] = getIptablesStatus();

    QJsonDocument doc(combinedJson);
    return QString::fromUtf8(doc.toJson(QJsonDocument::Indented));
}

    QString Q_Network::readConfFile(const QString &filePath) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Failed to open file:" << filePath;
            return QString();
        }

        QTextStream in(&file);
        QString content = in.readAll();
        file.close();

        return content;
    }

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

                    // Add iw info if Wireless interface
                    if (interfaceObject["description"].toString() == "Wireless interface") {
                        QProcess iwProcess;
                        iwProcess.start("iw", QStringList() << logicalName << "info");
                        iwProcess.waitForFinished(-1);
                        QByteArray iwOutput = iwProcess.readAllStandardOutput();
                        QString iwResult = QString::fromLocal8Bit(iwOutput);
                        QStringList iwLines = iwResult.split("\n");
                        QJsonObject iwObject;
                        for (const QString &iwLine : iwLines) {
                            QString trimmedLine = iwLine.trimmed();
                            if (trimmedLine.isEmpty()) continue;
                            int firstSpaceIndex = trimmedLine.indexOf(' ');
                            if (firstSpaceIndex != -1) {
                                QString iwKey = trimmedLine.left(firstSpaceIndex).trimmed();
                                QString iwValue = trimmedLine.mid(firstSpaceIndex + 1).trimmed();
                                iwObject[iwKey] = iwValue;
                            } else {
                                iwObject[trimmedLine] = QString();
                            }
                        }
                        interfaceObject["iw_info"] = iwObject;
                    }
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

            // Add iw info if Wireless interface
            if (interfaceObject["description"].toString() == "Wireless interface") {
                QProcess iwProcess;
                iwProcess.start("iw", QStringList() << logicalName << "info");
                iwProcess.waitForFinished(-1);
                QByteArray iwOutput = iwProcess.readAllStandardOutput();
                QString iwResult = QString::fromLocal8Bit(iwOutput);
                QStringList iwLines = iwResult.split("\n");
                QJsonObject iwObject;
                for (const QString &iwLine : iwLines) {
                    QString trimmedLine = iwLine.trimmed();
                    if (trimmedLine.isEmpty()) continue;
                    int firstSpaceIndex = trimmedLine.indexOf(' ');
                    if (firstSpaceIndex != -1) {
                        QString iwKey = trimmedLine.left(firstSpaceIndex).trimmed();
                        QString iwValue = trimmedLine.mid(firstSpaceIndex + 1).trimmed();
                        iwObject[iwKey] = iwValue;
                    } else {
                        iwObject[trimmedLine] = QString();
                    }
                }
                interfaceObject["iw_info"] = iwObject;
            }
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