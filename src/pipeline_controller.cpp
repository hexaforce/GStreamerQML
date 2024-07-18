// pipeline_controller.cpp
#include "pipeline_controller.h"
#include <QProcess>
#include <QRegularExpression>

PipelineController::PipelineController(QObject *parent) : QObject(parent) {}

PipelineManager *PipelineController::pipelineManager() const
{
    return this->m_pipelineManager;
}

void PipelineController::setPipelineManager(PipelineManager *pipelineManager)
{
    this->m_pipelineManager = pipelineManager;
}

void PipelineController::startPipeline(int port)
{
    g_print("PipelineController::startPipeline\n");
    this->m_pipelineManager->startPipeline(port);
}
