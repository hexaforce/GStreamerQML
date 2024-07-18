#ifndef PIPELINECONTROLLER_H
#define PIPELINECONTROLLER_H

#include <QRunnable>
#include <gst/gst.h>

class PipelineController : public QRunnable {
public:
  PipelineController(GstElement *pipeline);
  ~PipelineController();

  void run() override;

private:
  GstElement *m_pipeline;
};

#endif // PIPELINECONTROLLER_H
