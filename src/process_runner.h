// process_runner.h
#ifndef PROCESSRUNNER_H
#define PROCESSRUNNER_H

#include <QObject>
#include <QProcess>
#include <QDebug>

class ProcessRunner : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString result READ result NOTIFY resultChanged)

public:
    explicit ProcessRunner(QObject *parent = nullptr);

    Q_INVOKABLE void runCommand(const QString &command, const QStringList &arguments);

    QString result() const;

signals:
    void resultChanged();

private:
    QString m_result;
    QProcess m_process;

private slots:
    void readProcessOutput();
};

#endif // PROCESSRUNNER_H
