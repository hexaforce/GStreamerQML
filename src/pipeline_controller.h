// pipeline_controller.h
#ifndef PIPELINECONTROLLER_H
#define PIPELINECONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QStringList>
#include "pipeline_manager.h"
#include <gst/gst.h>

class PipelineController : public QObject
{
    Q_OBJECT
public:
    explicit PipelineController(QObject *parent = nullptr);

    Q_INVOKABLE void startPipeline(int port);
    Q_INVOKABLE QString get_pipeline_state();
    Q_INVOKABLE QString get_pipeline_parameters();

    PipelineManager *pipelineManager() const;
    void setPipelineManager(PipelineManager *pipelineManager);

public slots:

private:
    PipelineManager *m_pipelineManager;
};

#endif // PIPELINECONTROLLER_H
