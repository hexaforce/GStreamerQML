#include <gst/gst.h>

void setupVideoSendPipeline(GstElement *pipeline, const gchar *device, const gchar *host, gint port) {
  GstElement *src = gst_element_factory_make("v4l2src", NULL);
  GstElement *parse = gst_element_factory_make("h264parse", NULL);
  GstElement *rtppay = gst_element_factory_make("rtph264pay", NULL);
  GstElement *udpsink = gst_element_factory_make("udpsink", NULL);

  g_object_set(src, "device", device, NULL);
  g_object_set(udpsink, "host", host, "port", port, NULL);

  gst_bin_add_many(GST_BIN(pipeline), src, parse, rtppay, udpsink, NULL);
  if (!gst_element_link_many(src, parse, rtppay, udpsink, NULL)) {
    g_printerr("Failed to link video elements.\n");
    return;
  }
}

void setupVideoReceivePipeline(GstElement *pipeline, GstElement *sink, gint port) {
  GstElement *src = gst_element_factory_make("udpsrc", NULL);
  GstElement *rtpdepay = gst_element_factory_make("rtph264depay", NULL);
  GstElement *parse = gst_element_factory_make("h264parse", NULL);
  GstElement *decoder = gst_element_factory_make("nvv4l2decoder", NULL);
  GstElement *videoconvert = gst_element_factory_make("nvvideoconvert", NULL);

  g_object_set(src, "port", port, NULL);

  GstCaps *caps = gst_caps_new_simple("application/x-rtp", "media", G_TYPE_STRING, "video", "encoding-name", G_TYPE_STRING, "H264", "payload", G_TYPE_INT, 96, NULL);
  g_object_set(src, "caps", caps, NULL);
  gst_caps_unref(caps);

  gst_bin_add_many(GST_BIN(pipeline), src, rtpdepay, parse, decoder, videoconvert, sink, NULL);
  if (!gst_element_link_many(src, rtpdepay, parse, decoder, videoconvert, sink, NULL)) {
    g_printerr("Failed to link video elements.\n");
    return;
  }
}

void setupAudioSendPipeline(GstElement *pipeline, const gchar *host, gint port) {
  GstElement *src = gst_element_factory_make("alsasrc", NULL);
  GstElement *audioconvert = gst_element_factory_make("audioconvert", NULL);
  GstElement *audioresample = gst_element_factory_make("audioresample", NULL);
  GstElement *encoder = gst_element_factory_make("opusenc", NULL);
  GstElement *rtppay = gst_element_factory_make("rtpopuspay", NULL);
  GstElement *udpsink = gst_element_factory_make("udpsink", NULL);

  g_object_set(udpsink, "host", host, "port", port, NULL);

  gst_bin_add_many(GST_BIN(pipeline), src, audioconvert, audioresample, encoder, rtppay, udpsink, NULL);
  if (!gst_element_link_many(src, audioconvert, audioresample, encoder, rtppay, udpsink, NULL)) {
    g_printerr("Failed to link audio elements.\n");
    return;
  }
}

void setupAudioReceivePipeline(GstElement *pipeline, GstElement *sink, gint port) {
  GstElement *src = gst_element_factory_make("udpsrc", NULL);
  GstElement *rtpdepay = gst_element_factory_make("rtpopusdepay", NULL);
  GstElement *decoder = gst_element_factory_make("opusdec", NULL);
  GstElement *audioconvert = gst_element_factory_make("audioconvert", NULL);

  g_object_set(src, "port", port, NULL);

  GstCaps *caps = gst_caps_new_simple("application/x-rtp", "media", G_TYPE_STRING, "audio", "encoding-name", G_TYPE_STRING, "OPUS", "payload", G_TYPE_INT, 96, NULL);
  g_object_set(src, "caps", caps, NULL);
  gst_caps_unref(caps);

  gst_bin_add_many(GST_BIN(pipeline), src, rtpdepay, decoder, audioconvert, sink, NULL);
  if (!gst_element_link_many(src, rtpdepay, decoder, audioconvert, sink, NULL)) {
    g_printerr("Failed to link audio elements.\n");
    return;
  }
}

int main(int argc, char *argv[]) {
  gst_init(&argc, &argv);

  const gchar *video_device = "/dev/video0";
  const gchar *host = "127.0.0.1";
  gint video_port = 5000;
  gint audio_port = 5002;

  // Video send pipeline
  GstElement *videoSendPipeline = gst_pipeline_new("video-send-pipeline");
  setupVideoSendPipeline(videoSendPipeline, video_device, host, video_port);

  // Video receive pipeline
  GstElement *videoSink = gst_element_factory_make("autovideosink", NULL);
  GstElement *videoReceivePipeline = gst_pipeline_new("video-receive-pipeline");
  setupVideoReceivePipeline(videoReceivePipeline, videoSink, video_port);

  // Audio send pipeline
  GstElement *audioSendPipeline = gst_pipeline_new("audio-send-pipeline");
  setupAudioSendPipeline(audioSendPipeline, host, audio_port);

  // Audio receive pipeline
  GstElement *audioSink = gst_element_factory_make("autoaudiosink", NULL);
  GstElement *audioReceivePipeline = gst_pipeline_new("audio-receive-pipeline");
  setupAudioReceivePipeline(audioReceivePipeline, audioSink, audio_port);

  gst_element_set_state(videoSendPipeline, GST_STATE_PLAYING);
  gst_element_set_state(videoReceivePipeline, GST_STATE_PLAYING);
  gst_element_set_state(audioSendPipeline, GST_STATE_PLAYING);
  gst_element_set_state(audioReceivePipeline, GST_STATE_PLAYING);

  g_print("Audio and Video streaming started. Press Ctrl+C to stop.\n");
  g_main_loop_run(g_main_loop_new(NULL, FALSE));

  gst_element_set_state(videoSendPipeline, GST_STATE_NULL);
  gst_element_set_state(videoReceivePipeline, GST_STATE_NULL);
  gst_element_set_state(audioSendPipeline, GST_STATE_NULL);
  gst_element_set_state(audioReceivePipeline, GST_STATE_NULL);

  gst_object_unref(videoSendPipeline);
  gst_object_unref(videoReceivePipeline);
  gst_object_unref(audioSendPipeline);
  gst_object_unref(audioReceivePipeline);

  return 0;
}
