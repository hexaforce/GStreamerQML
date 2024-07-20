#include "receive_pipelines.h"

void setupLocalCapturePipeline(GstElement *pipeline, GstElement *sink) {
  GstElement *src = gst_element_factory_make("v4l2src", NULL);
  GstElement *videoconvert = gst_element_factory_make("videoconvert", NULL);
  GstElement *glupload = gst_element_factory_make("glupload", NULL);

  gst_bin_add_many(GST_BIN(pipeline), src, videoconvert, glupload, sink, NULL);
  gst_element_link_many(src, videoconvert, glupload, sink, NULL);
}

void setupJpegReceivePipeline_SoftwareDecoding(GstElement *pipeline, GstElement *sink, gint port, VendorType vendorType) {
  GstElement *src = gst_element_factory_make("udpsrc", NULL);
  GstElement *rtpdepay = gst_element_factory_make("rtpjpegdepay", NULL);
  GstElement *jpegparse = gst_element_factory_make("jpegparse", NULL);
  GstElement *decoder = gst_element_factory_make(vendorType == VendorType::Libav ? "avdec_mjpeg" : "jpegdec", NULL);
  GstElement *videoconvert = gst_element_factory_make("videoconvert", NULL);
  GstElement *glupload = gst_element_factory_make("glupload", NULL);

  g_object_set(src, "port", port, NULL);
  GstCaps *caps = gst_caps_new_simple("application/x-rtp", "media", G_TYPE_STRING, "video", "encoding-name", G_TYPE_STRING, "JPEG", "payload", G_TYPE_INT, 26, NULL);
  g_object_set(src, "caps", caps, NULL);
  gst_caps_unref(caps);

  gst_bin_add_many(GST_BIN(pipeline), src, rtpdepay, jpegparse, decoder, videoconvert, glupload, sink, NULL);
  gst_element_link_many(src, rtpdepay, jpegparse, decoder, videoconvert, glupload, sink, NULL);
}

void setupH26xReceivePipeline_HardwareDecoding(GstElement *pipeline, GstElement *sink, gint port, CodecType codecType) {
  GstElement *src = gst_element_factory_make("udpsrc", NULL);
  GstElement *rtpdepay = nullptr;
  GstElement *parse = nullptr;
  GstElement *decoder = gst_element_factory_make("nvv4l2decoder", NULL);
  GstElement *videoconvert = gst_element_factory_make("nvvideoconvert", NULL);

  g_object_set(src, "port", port, NULL);

  if (codecType == CodecType::H264) {
    rtpdepay = gst_element_factory_make("rtph264depay", NULL);
    parse = gst_element_factory_make("h264parse", NULL);
  } else if (codecType == CodecType::H265) {
    rtpdepay = gst_element_factory_make("rtph265depay", NULL);
    parse = gst_element_factory_make("h265parse", NULL);
  } else {
    g_printerr("Unsupported codec type.\n");
    return;
  }

  GstCaps *caps = gst_caps_new_simple("application/x-rtp", "media", G_TYPE_STRING, "video", "encoding-name", G_TYPE_STRING, (codecType == CodecType::H264) ? "H264" : "H265", "payload", G_TYPE_INT, 96, NULL);
  g_object_set(src, "caps", caps, NULL);
  gst_caps_unref(caps);

  gst_bin_add_many(GST_BIN(pipeline), src, rtpdepay, parse, decoder, videoconvert, sink, NULL);

  if (!gst_element_link_many(src, rtpdepay, parse, decoder, videoconvert, sink, NULL)) {
    g_printerr("Failed to link elements.\n");
    return;
  }
}

void setupH26xReceivePipeline_SoftwareDecoding(GstElement *pipeline, GstElement *sink, gint port, CodecType codecType, VendorType vendorType) {
  GstElement *src = gst_element_factory_make("udpsrc", NULL);
  GstElement *rtpdepay = nullptr;
  GstElement *parse = nullptr;
  GstElement *decoder = nullptr;
  GstElement *videoconvert = gst_element_factory_make("videoconvert", NULL);
  GstElement *glupload = gst_element_factory_make("glupload", NULL);

  g_object_set(src, "port", port, NULL);

  if (codecType == CodecType::H264) {
    rtpdepay = gst_element_factory_make("rtph264depay", NULL);
    parse = gst_element_factory_make("h264parse", NULL);
    decoder = gst_element_factory_make(vendorType == VendorType::Libav ? "avdec_h264" : "nvh264dec", NULL);
  } else if (codecType == CodecType::H265) {
    rtpdepay = gst_element_factory_make("rtph265depay", NULL);
    parse = gst_element_factory_make("h265parse", NULL);
    decoder = gst_element_factory_make(vendorType == VendorType::Libav ? "avdec_h265" : "nvh265dec", NULL);
  } else {
    g_printerr("Unsupported codec type.\n");
    return;
  }

  GstCaps *caps = gst_caps_new_simple("application/x-rtp", "media", G_TYPE_STRING, "video", "encoding-name", G_TYPE_STRING, (codecType == CodecType::H264) ? "H264" : "H265", "payload", G_TYPE_INT, 96, NULL);
  g_object_set(src, "caps", caps, NULL);
  gst_caps_unref(caps);

  gst_bin_add_many(GST_BIN(pipeline), src, rtpdepay, parse, decoder, videoconvert, glupload, sink, NULL);
  gst_element_link_many(src, rtpdepay, parse, decoder, videoconvert, glupload, sink, NULL);
}