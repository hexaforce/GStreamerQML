#ifndef PIPELINETHREAD_H
#define PIPELINETHREAD_H

#include <QRunnable>
#include <gst/gst.h>

class PipelineThread : public QRunnable {
public:
  PipelineThread(GstElement *pipeline);
  ~PipelineThread();

  void run() override;

private:
  GstElement *m_pipeline;
};

#endif // PIPELINETHREAD_H
