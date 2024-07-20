#include "process_runner.h"

ProcessRunner::ProcessRunner(QObject *parent) : QObject(parent)
{
    connect(&m_process, &QProcess::readyReadStandardOutput, this, &ProcessRunner::readProcessOutput);
    connect(&m_process, &QProcess::readyReadStandardError, this, &ProcessRunner::readProcessError);
    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &ProcessRunner::processFinished);
}

void ProcessRunner::runCommand(const QString &command, const QStringList &arguments)
{
    qDebug() << "runCommand: " << command << arguments;
    m_result.clear();
    m_error.clear();
    m_process.start(command, arguments);
    if (!m_process.waitForStarted()) {
        qDebug() << "Failed to start process";
    }
}

QString ProcessRunner::result() const
{
    return m_result;
}

QString ProcessRunner::error() const
{
    return m_error;
}

void ProcessRunner::readProcessOutput()
{
    // qDebug() << "readProcessOutput";
    m_result += QString::fromUtf8(m_process.readAllStandardOutput());
    emit resultChanged();
}

void ProcessRunner::readProcessError()
{
    // qDebug() << "readProcessError";
    m_error += QString::fromUtf8(m_process.readAllStandardError());
    emit errorChanged();
}

void ProcessRunner::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    // qDebug() << "Process finished with exit code" << exitCode;
    if (exitStatus == QProcess::CrashExit) {
        m_error += "\nProcess crashed";
        emit errorChanged();
    } else if (exitCode != 0) {
        m_error += "\nProcess exited with code " + QString::number(exitCode);
        emit errorChanged();
    }
    // emit resultChanged();
}