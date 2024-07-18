// pipeline_controller.h
#ifndef PIPELINECONTROLLER_H
#define PIPELINECONTROLLER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include "pipeline_manager.h"

class PipelineController : public QObject
{
    Q_OBJECT
public:
    explicit PipelineController(QObject *parent = nullptr);

    Q_INVOKABLE void startPipeline(int port);

    PipelineManager *pipelineManager() const;
    void setPipelineManager(PipelineManager *pipelineManager);

public slots:

private:
    PipelineManager *m_pipelineManager;
};

#endif // PIPELINECONTROLLER_H
