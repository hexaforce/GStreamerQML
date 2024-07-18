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


QString PipelineController::get_pipeline_state()
{
    qInfo() << "3get_pipeline_state!";
    if (!this->m_pipelineManager) {
        qWarning() << "Pipeline is not initialized!";
        return "UNINITIALIZED";
    }
    qInfo() << "2get_pipeline_state!";
    if (!this->m_pipelineManager->pipeline()) {
        qWarning() << "Pipeline is not initialized!";
        return "UNINITIALIZED";
    }

    qInfo() << "1get_pipeline_state!";
    try {
        GstState state;
        GstState pending;
        GstStateChangeReturn ret = gst_element_get_state(this->m_pipelineManager->pipeline(), &state, &pending, GST_CLOCK_TIME_NONE);

        qInfo() << "0get_pipeline_state!";
        if (ret == GST_STATE_CHANGE_FAILURE) {
            throw std::runtime_error("Failed to get pipeline state");
        }

        QString stateString;
        switch (ret) {
        case GST_STATE_CHANGE_SUCCESS:
            stateString = "SUCCESS";
            break;
        case GST_STATE_CHANGE_ASYNC:
            stateString = "ASYNC";
            break;
        case GST_STATE_CHANGE_NO_PREROLL:
            stateString = "NO_PREROLL";
            break;
        default:
            stateString = "UNKNOWN";
            break;
        }

        return stateString;
    } catch (const std::exception &e) {
        qWarning() << "Exception occurred: " << e.what();
        return "EXCEPTION";
    }
}
QString PipelineController::get_pipeline_parameters()
{
    return "aaa";
}
