#ifndef PIPELINEMANAGER_H
#define PIPELINEMANAGER_H

#include <QRunnable>
#include <QString>
#include <gst/gst.h>

class PipelineManager : public QRunnable
{
public:
    PipelineManager();
    ~PipelineManager();

    void run() override;
    void startPipeline(int port);
    void stopPipeline();

    GstElement *pipeline() const;
    void setPipeline(GstElement *pipeline);
    GstElement *sink() const;
    void setSink(GstElement *sink);

private:
    GstElement *m_pipeline;
    GstElement *m_sink;
};

#endif // PIPELINEMANAGER_H
