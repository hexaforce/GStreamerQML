// common_process_runner.h
#ifndef COMMON_PROCESS_RUNNER_H
#define COMMON_PROCESS_RUNNER_H

#include <QDebug>
#include <QObject>
#include <QProcess>

class CommonProcessRunner : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString result READ result NOTIFY resultChanged)
  Q_PROPERTY(QString error READ error NOTIFY errorChanged)

public:
  explicit CommonProcessRunner(QObject *parent = nullptr);

  Q_INVOKABLE void runCommand(const QString &command, const QStringList &arguments);

  QString result() const;
  QString error() const;

signals:
  void resultChanged();
  void errorChanged();

private:
  QString m_result;
  QString m_error;
  QProcess m_process;

private slots:
  void readProcessOutput();
  void readProcessError();
  void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif // COMMON_PROCESS_RUNNER_H
