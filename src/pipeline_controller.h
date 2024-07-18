#ifndef PIPELINECONTROLLER_H
#define PIPELINECONTROLLER_H

#include <QRunnable>
#include <QObject>
#include <gst/gst.h>

class PipelineController : public QRunnable
{
public:
    PipelineController(GstElement *pipeline);
    ~PipelineController();

    void run() override;
    Q_INVOKABLE void startPipeline(int port);
    Q_INVOKABLE void stopPipeline();
    GstElement *pipeline() const;
    void setPipeline(GstElement *pipeline);

private:
    GstElement *m_pipeline;
};

#endif // PIPELINECONTROLLER_H
