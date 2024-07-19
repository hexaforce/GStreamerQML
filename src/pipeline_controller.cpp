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

// bool PipelineController::isPipelineRunning() const {
//   // Check if the pipeline is currently running
//   GstState current, pending;
//   gst_element_get_state(pipeline, &current, &pending, 0);
//   return (current == GST_STATE_PLAYING);
// }

QString PipelineController::get_pipeline_state()
{


    if (!pipelineManager()->pipeline()) {
        qWarning() << "Pipeline is not initialized!";
        return "UNINITIALIZED";
    }

    try {
        GstState state, pending;
        // GstStateChangeReturn ret = gst_element_get_state(pipelineManager(), &state, &pending, GST_CLOCK_TIME_NONE);
        GstStateChangeReturn ret = gst_element_get_state(pipelineManager()->pipeline(), &state, &pending, 0);

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
