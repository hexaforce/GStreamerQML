#include "common_process_runner.h"

CommonProcessRunner::CommonProcessRunner(QObject *parent) : QObject(parent) {
  connect(&m_process, &QProcess::readyReadStandardOutput, this, &CommonProcessRunner::readProcessOutput);
  connect(&m_process, &QProcess::readyReadStandardError, this, &CommonProcessRunner::readProcessError);
  connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &CommonProcessRunner::processFinished);
}

void CommonProcessRunner::runCommand(const QString &command, const QStringList &arguments) {
  qDebug() << "runCommand: " << command << arguments;
  m_result.clear();
  m_error.clear();
  m_process.start(command, arguments);
  if (!m_process.waitForStarted()) {
    qDebug() << "Failed to start process";
  }
}

QString CommonProcessRunner::result() const { return m_result; }

QString CommonProcessRunner::error() const { return m_error; }

void CommonProcessRunner::readProcessOutput() {
  // qDebug() << "readProcessOutput";
  m_result += QString::fromUtf8(m_process.readAllStandardOutput());
  emit resultChanged();
}

void CommonProcessRunner::readProcessError() {
  // qDebug() << "readProcessError";
  m_error += QString::fromUtf8(m_process.readAllStandardError());
  emit errorChanged();
}

void CommonProcessRunner::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
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