#include "process_runner.h"

ProcessRunner::ProcessRunner(QObject *parent) : QObject(parent)
{
    connect(&m_process, &QProcess::readyReadStandardOutput, this, &ProcessRunner::readProcessOutput);
}

void ProcessRunner::runCommand(const QString &command, const QStringList &arguments)
{
    qDebug() << "runCommand";
    m_process.start(command, arguments);
    m_process.waitForFinished();
}

QString ProcessRunner::result() const
{
    qDebug() << "result";
    return m_result;
}

void ProcessRunner::readProcessOutput()
{
    qDebug() << "readProcessOutput";
    m_result = QString::fromUtf8(m_process.readAllStandardOutput());
    emit resultChanged();
}
