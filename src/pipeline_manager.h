#ifndef PIPELINEMANAGER_H
#define PIPELINEMANAGER_H

#include <QRunnable>
#include <QString>
#include <gst/gst.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QObject>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QObject>
#include <QDebug>
#include <QString>
#include <QStringList>

class PipelineManager : public QObject
{
    Q_OBJECT
public:
    explicit PipelineManager(QObject *parent = nullptr);

    // class PipelineManager : public QRunnable
    // {
    // public:
    //     PipelineManager();
    ~PipelineManager();

    // void run() override;
    // void startPipeline(int port);
    // void stopPipeline();

    GstElement *pipeline() const;
    void setPipeline(GstElement *pipeline);
    GstElement *sink() const;
    void setSink(GstElement *sink);
    //  Q_INVOKABLE void getPipelineInfo();
    void getPipelineInfo();

    // Q_INVOKABLE void getPipelineInfo();
    // Q_INVOKABLE void getState();

signals:
    // void pipelineInfoChanged(const QString &pipelineInfo);
    // void stateInfoChanged(const QString &stateInfo);

public slots:

private:
    GstElement *m_pipeline;
    GstElement *m_sink;
    QJsonObject getElementInfo(GstElement *element);
};

#endif // PIPELINEMANAGER_H
