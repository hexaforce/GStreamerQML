#ifndef PIPELINE_MANAGER_H
#define PIPELINE_MANAGER_H

#include <QRunnable>
#include <gst/gst.h>

class PipelineManager : public QRunnable
{
public:
    PipelineManager();
    ~PipelineManager();

    void run() override;

    GstElement *pipeline() const;
    void setPipeline(GstElement *pipeline);
    GstElement *sink() const;
    void setSink(GstElement *sink);

private:
    GstElement *m_pipeline;
    GstElement *m_sink;
};

#endif // PIPELINE_MANAGER_H
