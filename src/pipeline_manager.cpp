#include "pipeline_manager.h"
#include "receive_pipelines.h"

PipelineManager::PipelineManager()
{
    GstElement *pipeline = gst_pipeline_new(NULL);
    this->m_pipeline = static_cast<GstElement *>(gst_object_ref(pipeline));
    // this->m_pipeline = gst_pipeline_new(NULL);

    GstElement *sink = gst_element_factory_make("qmlglsink", NULL);
    this->m_sink = static_cast<GstElement *>(gst_object_ref(sink));
    // this->m_sink = gst_element_factory_make("qmlglsink", NULL);

    gint port = 5000;
    // setupLocalCapturePipeline(pipeline, sink);
    setupH26xReceivePipeline_SoftwareDecoding(this->m_pipeline, this->m_sink, port, CodecType::H264, VendorType::Libav);
    // setupH265ReceivePipeline(pipeline, sink, port);
    // setupJpegReceivePipeline(pipeline, sink, port);

    GstState state, pending;
    GstStateChangeReturn ret = gst_element_get_state(pipeline, &state, &pending, GST_CLOCK_TIME_NONE);

    /* Print the state change return value */
    g_print("State Change Return: %d\n", ret);

    /* Print the current and pending states */
    g_print("Current state: %s\n", gst_element_state_get_name(state));
    g_print("Pending state: %s\n", gst_element_state_get_name(pending));
}

PipelineManager::~PipelineManager()
{
    if (this->m_pipeline)
    {
        gst_object_unref(this->m_pipeline);
    }
    if (this->m_sink)
    {
        gst_object_unref(this->m_sink);
    }
}

void PipelineManager::run()
{
    if (this->m_pipeline)
        gst_element_set_state(this->m_pipeline, GST_STATE_PLAYING);
}

void PipelineManager::startPipeline(int port)
{
    // パイプラインの開始処理
    g_print("PipelineManager::startPipeline\n");
    // g_print(this->m_pipeline);
}

void PipelineManager::stopPipeline()
{
    // パイプラインの停止処理
}

GstElement *PipelineManager::pipeline() const
{
    return this->m_pipeline;
}

void PipelineManager::setPipeline(GstElement *pipeline)
{
    if (this->m_pipeline)
    {
        gst_object_unref(this->m_pipeline);
    }
    this->m_pipeline = pipeline ? static_cast<GstElement *>(gst_object_ref(pipeline)) : nullptr;
}

GstElement *PipelineManager::sink() const
{
    return this->m_sink;
}

void PipelineManager::setSink(GstElement *sink)
{
    if (this->m_sink)
    {
        gst_object_unref(this->m_sink);
    }
    this->m_sink = sink ? static_cast<GstElement *>(gst_object_ref(sink)) : nullptr;
}
