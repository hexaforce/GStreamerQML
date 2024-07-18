#include "pipeline_manager.h"

PipelineManager::PipelineManager(GstElement *pipeline)
{
    this->m_pipeline = pipeline ? static_cast<GstElement *>(gst_object_ref(pipeline)) : NULL;
}

PipelineManager::~PipelineManager()
{
    if (this->m_pipeline)
        gst_object_unref(this->m_pipeline);
}

void PipelineManager::run()
{
    if (this->m_pipeline)
        gst_element_set_state(this->m_pipeline, GST_STATE_PLAYING);
}

void PipelineManager::startPipeline(int port)
{
    g_print("startPipeline.\n");
}

void PipelineManager::stopPipeline()
{
    g_print("stopPipeline.\n");
}

GstElement *PipelineManager::pipeline() const
{
    return m_pipeline;
}

void PipelineManager::setPipeline(GstElement *pipeline)
{
    m_pipeline = pipeline;
}
