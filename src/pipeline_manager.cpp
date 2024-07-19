#include "pipeline_manager.h"
#include "receive_pipelines.h"
#include <QDebug>
#include <QStringBuilder>
#include <QJsonDocument>

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
    qDebug() << "State Change Return:" << ret;
    qDebug() << "Current state:" << gst_element_state_get_name(state);
    qDebug() << "Pending state:" << gst_element_state_get_name(pending);
    // /* Print the state change return value */
    // g_print("State Change Return: %d\n", ret);

    // /* Print the current and pending states */
    // g_print("Current state: %s\n", gst_element_state_get_name(state));
    // g_print("Pending state: %s\n", gst_element_state_get_name(pending));

    //   if (!pipeline) {
    //     qDebug() << "Pipeline is not initialized.";
    //     emit pipelineInfoChanged("Pipeline is not initialized");
    //     return;
    // }

}

void PipelineManager::getPipelineInfo() {
    // if (!pipeline()) {
    //     qDebug() << "Pipeline is not initialized.";
    //     emit pipelineInfoChanged("Pipeline is not initialized");
    //     return;
    // }

    GstIterator *iter = gst_bin_iterate_elements(GST_BIN(pipeline()));
    GValue item = G_VALUE_INIT;
    gboolean done = FALSE;
    QJsonArray elementsArray;

    while (!done) {
        switch (gst_iterator_next(iter, &item)) {
            case GST_ITERATOR_OK: {
                GstElement *element = GST_ELEMENT(g_value_get_object(&item));
                QJsonObject elementInfo = getElementInfo(element);
                elementsArray.append(elementInfo);
                g_value_reset(&item);
                break;
            }
            case GST_ITERATOR_RESYNC:
                gst_iterator_resync(iter);
                break;
            case GST_ITERATOR_ERROR:
            case GST_ITERATOR_DONE:
                done = TRUE;
                break;
        }
    }

    gst_iterator_free(iter);

    QJsonObject pipelineInfo;
    pipelineInfo["elements"] = elementsArray;

    QJsonDocument doc(pipelineInfo);
    QString jsonString = doc.toJson(QJsonDocument::Compact);
    qDebug() << jsonString;
    // emit pipelineInfoChanged(jsonString);
}

QJsonObject PipelineManager::getElementInfo(GstElement *element) {
    QJsonObject elementInfo;
    const gchar *element_name = gst_element_get_name(element);
    const gchar *element_type = gst_element_factory_get_longname(gst_element_get_factory(element));

    elementInfo["name"] = element_name;
    elementInfo["type"] = element_type;

    // プロパティを取得
    GParamSpec **properties;
    guint num_properties;
    properties = g_object_class_list_properties(G_OBJECT_GET_CLASS(element), &num_properties);

    QJsonObject propertiesInfo;
    for (guint i = 0; i < num_properties; i++) {
        GParamSpec *property = properties[i];
        gchar *name = g_strdup(property->name);
        GValue value = G_VALUE_INIT;
        g_value_init(&value, property->value_type);
        g_object_get_property(G_OBJECT(element), name, &value);
        gchar *value_str = g_strdup_value_contents(&value);
        propertiesInfo[name] = value_str;
        g_free(name);
        g_free(value_str);
        g_value_unset(&value);
    }
    g_free(properties);

    elementInfo["properties"] = propertiesInfo;

    return elementInfo;
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
