#include "transmit_pipelines.h"

GstStateChangeReturn setupVideoTransmitPipeline(GstElement *pipeline, const char *device, const char *host, int port, CodecType codecType) {
    // ビデオ送信パイプラインのセットアップコード
    // codecTypeに基づいてH264またはH265を設定
    // device, host, portの使用
    // 例:
    GstElement *src = gst_element_factory_make("v4l2src", "src");
    GstElement *enc = gst_element_factory_make("x264enc", "enc");
    GstElement *pay = gst_element_factory_make("rtph264pay", "pay");
    GstElement *sink = gst_element_factory_make("udpsink", "sink");
    gst_bin_add_many(GST_BIN(pipeline), src, enc, pay, sink, NULL);
    gst_element_link_many(src, enc, pay, sink, NULL);
    return gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // // 簡略化のための例
    // return GST_STATE_CHANGE_SUCCESS;
}

GstStateChangeReturn setupAudioTransmitPipeline(GstElement *pipeline, const char *device, const char *host, int port) {
    // オーディオ送信パイプラインのセットアップコード
    // device, host, portの使用
    // 例:
    GstElement *src = gst_element_factory_make("alsasrc", "src");
    GstElement *enc = gst_element_factory_make("opusenc", "enc");
    GstElement *pay = gst_element_factory_make("rtpopuspay", "pay");
    GstElement *sink = gst_element_factory_make("udpsink", "sink");
    gst_bin_add_many(GST_BIN(pipeline), src, enc, pay, sink, NULL);
    gst_element_link_many(src, enc, pay, sink, NULL);
    return gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // 簡略化のための例

    // return GST_STATE_CHANGE_SUCCESS;
    // GstElement *src           = gst_element_factory_make("alsasrc", NULL);
    // GstElement *audioconvert  = gst_element_factory_make("audioconvert", NULL);
    // GstElement *audioresample = gst_element_factory_make("audioresample", NULL);
    // GstElement *encoder       = gst_element_factory_make("opusenc", NULL);
    // GstElement *rtppay        = gst_element_factory_make("rtpopuspay", NULL);
    // GstElement *udpsink       = gst_element_factory_make("udpsink", NULL);

    // g_object_set(udpsink, "host", "127.0.0.1", "port", 5001, NULL);

    // gst_bin_add_many(GST_BIN(pipeline), src, audioconvert, audioresample, encoder, rtppay, udpsink, NULL);
    // if (!gst_element_link_many(src, audioconvert, audioresample, encoder, rtppay, udpsink, NULL)) {
    //     g_printerr("Failed to link elements.\n");
    //     return;
    // }
}

#include "transmit_pipelines.h"

GstStateChangeReturn setupJpegTransmitPipeline(GstElement *pipeline, const char *device, const char *host, int port) {
  GstElement *source    = gst_element_factory_make("v4l2src", NULL);
  GstElement *filter    = gst_element_factory_make("capsfilter", NULL);
  GstElement *parser    = gst_element_factory_make("jpegparse", NULL);
  GstElement *payloader = gst_element_factory_make("rtpjpegpay", NULL);
  GstElement *sink      = gst_element_factory_make("udpsink", NULL);

  g_object_set(source, "device", device, NULL);

  GstCaps *caps = gst_caps_new_simple("image/jpeg", "width", G_TYPE_INT, 1920, "height", G_TYPE_INT, 1080, "framerate", GST_TYPE_FRACTION, 60, 1, NULL);
  g_object_set(filter, "caps", caps, NULL);
  gst_caps_unref(caps);

  g_object_set(sink, "host", host, "port", port, NULL);

  gst_bin_add_many(GST_BIN(pipeline), source, filter, parser, payloader, sink, NULL);
  gst_element_link_many(source, filter, parser, payloader, sink, NULL);

  GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
  return ret;
}

GstStateChangeReturn setupH26xTransmitPipeline(GstElement *pipeline, const char *device, const char *host, int port, CodecType codecType) {
    GstElement *source    = gst_element_factory_make("v4l2src", NULL);
    GstElement *filter    = gst_element_factory_make("capsfilter", NULL);
    GstElement *parser    = nullptr;
    GstElement *payloader = nullptr;
    GstElement *sink      = gst_element_factory_make("udpsink", NULL);

    g_object_set(source, "device", device, NULL);

    GstCaps *caps = nullptr;
    if (codecType == CodecType::H264) {
        caps = gst_caps_new_simple("video/x-h264", "width", G_TYPE_INT, 1920, "height", G_TYPE_INT, 1080, "framerate", GST_TYPE_FRACTION, 30, 1, NULL);
        parser = gst_element_factory_make("h264parse", NULL);
        payloader = gst_element_factory_make("rtph264pay", NULL);
    } else if (codecType == CodecType::H265) {
        caps = gst_caps_new_simple("video/x-h265", "width", G_TYPE_INT, 1920, "height", G_TYPE_INT, 1080, "framerate", GST_TYPE_FRACTION, 30, 1, NULL);
        parser = gst_element_factory_make("h265parse", NULL);
        payloader = gst_element_factory_make("rtph265pay", NULL);
    }

    if (!caps || !parser || !payloader) {
        g_printerr("Failed to create caps, parser, or payloader element.\n");
        return GST_STATE_CHANGE_FAILURE;
    }

    g_object_set(filter, "caps", caps, NULL);
    gst_caps_unref(caps);

    g_object_set(sink, "host", host, "port", port, NULL);

    gst_bin_add_many(GST_BIN(pipeline), source, filter, parser, payloader, sink, NULL);
    gst_element_link_many(source, filter, parser, payloader, sink, NULL);

    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    return ret;
}

GstStateChangeReturn setupH26xTransmitPipeline_RawToH26x_SoftwareEncoding(GstElement *pipeline, const char *device, const char *host, int port, CodecType codecType) {
  GstElement *source       = gst_element_factory_make("v4l2src", NULL);
  GstElement *filter       = gst_element_factory_make("capsfilter", NULL);
  GstElement *videoconvert = gst_element_factory_make("videoconvert", NULL);
  GstElement *encoder      = nullptr;
  GstElement *parser       = nullptr;
  GstElement *payloader    = nullptr;
  GstElement *sink         = gst_element_factory_make("udpsink", NULL);

  g_object_set(source, "device", device, NULL);

  GstCaps *caps = gst_caps_new_simple("video/x-raw", "format", G_TYPE_STRING, "YUY2", "width", G_TYPE_INT, 1920, "height", G_TYPE_INT, 1080, "framerate", GST_TYPE_FRACTION, 30, 1, NULL);
  g_object_set(filter, "caps", caps, NULL);
  gst_caps_unref(caps);

  g_object_set(sink, "host", host, "port", port, NULL);

  if (codecType == CodecType::H264) {
    encoder   = gst_element_factory_make("x264enc", NULL);
    parser    = gst_element_factory_make("h264parse", NULL);
    payloader = gst_element_factory_make("rtph264pay", NULL);
  } else if (codecType == CodecType::H265) {
    encoder   = gst_element_factory_make("x265enc", NULL);
    parser    = gst_element_factory_make("h265parse", NULL);
    payloader = gst_element_factory_make("rtph265pay", NULL);
  }

  if (!encoder || !parser || !payloader) {
    g_printerr("Failed to create encoder, parser, or payloader element.\n");
    return GST_STATE_CHANGE_FAILURE;
  }

  gst_bin_add_many(GST_BIN(pipeline), source, filter, videoconvert, encoder, parser, payloader, sink, NULL);
  gst_element_link_many(source, filter, videoconvert, encoder, parser, payloader, sink, NULL);

  GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
  return ret;
}

GstStateChangeReturn setupH26xTransmitPipeline_JpegToH26x_SoftwareEncoding(GstElement *pipeline, const char *device, const char *host, int port, CodecType codecType) {
  GstElement *source       = gst_element_factory_make("v4l2src", NULL);
  GstElement *filter       = gst_element_factory_make("capsfilter", NULL);
  GstElement *jpegparse    = gst_element_factory_make("jpegparse", NULL);
  GstElement *jpegdec      = gst_element_factory_make("jpegdec", NULL);
  GstElement *videoconvert = gst_element_factory_make("videoconvert", NULL);
  GstElement *encoder      = nullptr;
  GstElement *parser       = nullptr;
  GstElement *payloader    = nullptr;
  GstElement *sink         = gst_element_factory_make("udpsink", NULL);

  g_object_set(source, "device", device, NULL);

  GstCaps *caps = gst_caps_new_simple("image/jpeg", "width", G_TYPE_INT, 1920, "height", G_TYPE_INT, 1080, "framerate", GST_TYPE_FRACTION, 60, 1, NULL);
  g_object_set(filter, "caps", caps, NULL);
  gst_caps_unref(caps);

  g_object_set(sink, "host", host, "port", port, NULL);

  if (codecType == CodecType::H264) {
    encoder   = gst_element_factory_make("x264enc", NULL);
    parser    = gst_element_factory_make("h264parse", NULL);
    payloader = gst_element_factory_make("rtph264pay", NULL);
  } else if (codecType == CodecType::H265) {
    encoder   = gst_element_factory_make("x265enc", NULL);
    parser    = gst_element_factory_make("h265parse", NULL);
    payloader = gst_element_factory_make("rtph265pay", NULL);
  }

  if (!encoder || !parser || !payloader) {
    g_printerr("Failed to create encoder, parser, or payloader element.\n");
    return GST_STATE_CHANGE_FAILURE;
  }

  gst_bin_add_many(GST_BIN(pipeline), source, filter, jpegparse, jpegdec, videoconvert, encoder, parser, payloader, sink, NULL);
  gst_element_link_many(source, filter, jpegparse, jpegdec, videoconvert, encoder, parser, payloader, sink, NULL);

  GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
  return ret;
}

GstStateChangeReturn setupH26xTransmitPipeline_RawToH26x_HardwareEncoding(GstElement *pipeline, const char *device, const char *host, int port, CodecType codecType) {
    GstElement *source         = gst_element_factory_make("v4l2src", NULL);
    GstElement *filter         = gst_element_factory_make("capsfilter", NULL);
    GstElement *videoconvert   = gst_element_factory_make("videoconvert", NULL);
    GstElement *nvvideoconvert = gst_element_factory_make("nvvideoconvert", NULL);
    GstElement *encoder        = nullptr;
    GstElement *parser         = nullptr;
    GstElement *payloader      = nullptr;
    GstElement *sink           = gst_element_factory_make("udpsink", NULL);

    g_object_set(source, "device", device, NULL);

    GstCaps *caps = gst_caps_new_simple("video/x-raw", "format", G_TYPE_STRING, "YUY2", "width", G_TYPE_INT, 1920, "height", G_TYPE_INT, 1080, "framerate", GST_TYPE_FRACTION, 30, 1, NULL);
    g_object_set(filter, "caps", caps, NULL);
    gst_caps_unref(caps);

    g_object_set(sink, "host", host, "port", port, NULL);

    if (codecType == CodecType::H264) {
        encoder = gst_element_factory_make("nvv4l2h264enc", NULL);
        parser = gst_element_factory_make("h264parse", NULL);
        payloader = gst_element_factory_make("rtph264pay", NULL);
    } else if (codecType == CodecType::H265) {
        encoder = gst_element_factory_make("nvv4l2h265enc", NULL);
        parser = gst_element_factory_make("h265parse", NULL);
        payloader = gst_element_factory_make("rtph265pay", NULL);
    }

    if (!encoder || !parser || !payloader) {
        g_printerr("Failed to create encoder, parser, or payloader element.\n");
        return GST_STATE_CHANGE_FAILURE;
    }

    gst_bin_add_many(GST_BIN(pipeline), source, filter, videoconvert, nvvideoconvert, encoder, parser, payloader, sink, NULL);
    gst_element_link_many(source, filter, videoconvert, nvvideoconvert, encoder, parser, payloader, sink, NULL);

    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    return ret;
}

GstStateChangeReturn setupH26xTransmitPipeline_JpegToH26x_HardwareEncoding(GstElement *pipeline, const char *device, const char *host, int port, CodecType codecType) {
    GstElement *source          = gst_element_factory_make("v4l2src", NULL);
    GstElement *filter          = gst_element_factory_make("capsfilter", NULL);
    GstElement *jpegparser      = gst_element_factory_make("jpegparse", NULL);
    GstElement *jpegdec         = gst_element_factory_make("jpegdec", NULL);
    GstElement *videoconvert1   = gst_element_factory_make("videoconvert", NULL);
    GstElement *nvvideoconvert1 = gst_element_factory_make("nvvideoconvert", NULL);
    GstElement *nvvideoconvert2 = gst_element_factory_make("nvvideoconvert", NULL);
    GstElement *encoder         = NULL;
    GstElement *parser          = NULL;
    GstElement *payloader       = NULL;
    GstElement *sink           = gst_element_factory_make("udpsink", NULL);

    g_object_set(source, "device", device, NULL);

    GstCaps *caps = gst_caps_new_simple("image/jpeg", "width", G_TYPE_INT, 1920, "height", G_TYPE_INT, 1080, "framerate", GST_TYPE_FRACTION, 60, 1, NULL);
    g_object_set(filter, "caps", caps, NULL);
    gst_caps_unref(caps);

    g_object_set(sink, "host", host, "port", port, NULL);

    if (codecType == CodecType::H264) {
        encoder = gst_element_factory_make("nvv4l2h264enc", NULL);
        parser  = gst_element_factory_make("h264parse", NULL);
        payloader = gst_element_factory_make("rtph264pay", NULL);
    } else if (codecType == CodecType::H265) {
        encoder = gst_element_factory_make("nvv4l2h265enc", NULL);
        parser  = gst_element_factory_make("h265parse", NULL);
        payloader = gst_element_factory_make("rtph265pay", NULL);
    } else {
        g_printerr("Unsupported codec type.\n");
        return GST_STATE_CHANGE_FAILURE;
    }

    gst_bin_add_many(GST_BIN(pipeline), source, filter, jpegparser, jpegdec, videoconvert1, nvvideoconvert1, nvvideoconvert2, encoder, parser, payloader, sink, NULL);
    gst_element_link_many(source, filter, jpegparser, jpegdec, videoconvert1, nvvideoconvert1, nvvideoconvert2, encoder, parser, payloader, sink, NULL);

    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    return ret;
}
