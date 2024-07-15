// q_telemetry.h
#ifndef Q_TELEMETRY_H
#define Q_TELEMETRY_H

#include <QObject>
#include <QString>
#include <QStringList>

class Q_Telemetry : public QObject
{
    Q_OBJECT
public:
    explicit Q_Telemetry(QObject *parent = nullptr);

public slots:

private:

};

#endif // Q_TELEMETRY_H
