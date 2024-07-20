#ifndef COMMON_COMMON_PIPELINE_MANAGER_H
#define COMMON_COMMON_PIPELINE_MANAGER_H

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
#include <QRunnable>

class CommonPipelineManager : public QRunnable
{
    Q_OBJECT
public:
    CommonPipelineManager();
    ~CommonPipelineManager();

    GstElement *pipeline() const;
    void setPipeline(GstElement *pipeline);
    GstElement *sink() const;
    void setSink(GstElement *sink);

    void run() override;
    // void startPipeline(int port);
    // void stopPipeline();


     Q_INVOKABLE QString getPipelineInfo();
    // void getPipelineInfo();

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

#endif // COMMON_COMMON_PIPELINE_MANAGER_H
