#include "pipeline_controller.h"

PipelineController::PipelineController(GstElement *pipeline)
{
    this->m_pipeline = pipeline ? static_cast<GstElement *>(gst_object_ref(pipeline)) : NULL;
}

PipelineController::~PipelineController()
{
    if (this->m_pipeline)
        gst_object_unref(this->m_pipeline);
}

void PipelineController::run()
{
    if (this->m_pipeline)
        gst_element_set_state(this->m_pipeline, GST_STATE_PLAYING);
}

// void PipelineController::startPipeline(int port)
// {
//     g_print("startPipeline.\n");
// }

// void PipelineController::stopPipeline()
// {
//     g_print("stopPipeline.\n");
// }

// GstElement *PipelineController::pipeline() const
// {
//     return m_pipeline;
// }

// void PipelineController::setPipeline(GstElement *pipeline)
// {
//     m_pipeline = pipeline;
// }
