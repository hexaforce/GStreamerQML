#ifndef TRANSMIT_PIPELINES_H
#define TRANSMIT_PIPELINES_H

#include <gst/gst.h>

enum class CodecType { H264, H265 };

GstStateChangeReturn setupVideoTransmitPipeline(GstElement *pipeline, const char *device, const char *host, int port, CodecType codecType);
GstStateChangeReturn setupAudioTransmitPipeline(GstElement *pipeline, const char *device, const char *host, int port);

#endif // TRANSMIT_PIPELINES_H
