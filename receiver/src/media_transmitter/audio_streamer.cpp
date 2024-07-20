#include <gst/gst.h>

// gst-launch-1.0 -e alsasrc ! audioconvert ! audioresample ! opusenc ! rtpopuspay ! udpsink host=127.0.0.1 port=5002
void setupAudioSendPipeline(GstElement *pipeline, gint port) {
    GstElement *src           = gst_element_factory_make("alsasrc", NULL);
    GstElement *audioconvert  = gst_element_factory_make("audioconvert", NULL);
    GstElement *audioresample = gst_element_factory_make("audioresample", NULL);
    GstElement *encoder       = gst_element_factory_make("opusenc", NULL);
    GstElement *rtppay        = gst_element_factory_make("rtpopuspay", NULL);
    GstElement *udpsink       = gst_element_factory_make("udpsink", NULL);

    g_object_set(udpsink, "host", "127.0.0.1", "port", port, NULL);

    gst_bin_add_many(GST_BIN(pipeline), src, audioconvert, audioresample, encoder, rtppay, udpsink, NULL);
    if (!gst_element_link_many(src, audioconvert, audioresample, encoder, rtppay, udpsink, NULL)) {
        g_printerr("Failed to link elements.\n");
        return;
    }
}

// gst-launch-1.0 -e udpsrc port=5002 caps="application/x-rtp, media=audio, encoding-name=OPUS, payload=96" ! rtpopusdepay ! opusdec ! audioconvert ! autoaudiosink
void setupAudioReceivePipeline(GstElement *pipeline, GstElement *sink, gint port) {
    GstElement *src           = gst_element_factory_make("udpsrc", NULL);
    GstElement *rtpdepay      = gst_element_factory_make("rtpopusdepay", NULL);
    GstElement *decoder       = gst_element_factory_make("opusdec", NULL);
    GstElement *audioconvert  = gst_element_factory_make("audioconvert", NULL);

    g_object_set(src, "port", port, NULL);

    GstCaps *caps = gst_caps_new_simple("application/x-rtp", "media", G_TYPE_STRING, "audio", "encoding-name", G_TYPE_STRING, "OPUS", "payload", G_TYPE_INT, 96, NULL);
    g_object_set(src, "caps", caps, NULL);
    gst_caps_unref(caps);

    gst_bin_add_many(GST_BIN(pipeline), src, rtpdepay, decoder, audioconvert, sink, NULL);
    if (!gst_element_link_many(src, rtpdepay, decoder, audioconvert, sink, NULL)) {
        g_printerr("Failed to link elements.\n");
        return;
    }
}

int main(int argc, char *argv[]) {
    gst_init(&argc, &argv);

    GstElement *audioSendPipeline = gst_pipeline_new("audio-send-pipeline");
    setupAudioSendPipeline(audioSendPipeline, 5002);

    GstElement *audioSink = gst_element_factory_make("autoaudiosink", NULL);
    GstElement *audioReceivePipeline = gst_pipeline_new("audio-receive-pipeline");
    setupAudioReceivePipeline(audioReceivePipeline, audioSink, 5002);

    gst_element_set_state(audioSendPipeline, GST_STATE_PLAYING);
    gst_element_set_state(audioReceivePipeline, GST_STATE_PLAYING);

    g_print("Audio streaming started. Press Ctrl+C to stop.\n");
    g_main_loop_run(g_main_loop_new(NULL, FALSE));

    gst_element_set_state(audioSendPipeline, GST_STATE_NULL);
    gst_element_set_state(audioReceivePipeline, GST_STATE_NULL);

    gst_object_unref(audioSendPipeline);
    gst_object_unref(audioReceivePipeline);

    return 0;
}
