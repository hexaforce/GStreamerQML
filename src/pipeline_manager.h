#ifndef PIPELINEMANAGER_H
#define PIPELINEMANAGER_H

#include <QRunnable>
#include <QObject>
#include <gst/gst.h>

class PipelineManager : public QRunnable
{
public:
    PipelineManager(GstElement *pipeline);
    ~PipelineManager();

    void run() override;
    // Q_INVOKABLE void startPipeline(int port);
    // Q_INVOKABLE void stopPipeline();
    // GstElement *pipeline() const;
    // void setPipeline(GstElement *pipeline);

private:
    GstElement *m_pipeline;
};

#endif // PIPELINEMANAGER_H
