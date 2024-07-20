#ifndef TRANSMIT_PIPELINES_H
#define TRANSMIT_PIPELINES_H

#include <gst/gst.h>

enum class CodecType { H264, H265 };
GstStateChangeReturn setupVideoTransmitPipeline(GstElement *pipeline, const char *device, const char *host, int port, CodecType codecType);
GstStateChangeReturn setupAudioTransmitPipeline(GstElement *pipeline, const char *device, const char *host, int port);

// gst-launch-1.0 -e v4l2src device=/dev/video0 ! 'image/jpeg,width=1920,height=1080,framerate=60/1' ! jpegparse ! rtpjpegpay ! udpsink host=127.0.0.1 port=5000
GstStateChangeReturn setupJpegTransmitPipeline(GstElement *pipeline, const char *device, const char *host, int port);

// gst-launch-1.0 -e v4l2src device=/dev/video0 ! 'video/x-h264,width=1920,height=1080,framerate=30/1' ! h264parse ! rtph264pay ! udpsink host=127.0.0.1 port=5000
// gst-launch-1.0 -e v4l2src device=/dev/video0 ! 'video/x-h265,width=1920,height=1080,framerate=30/1' ! h265parse ! rtph265pay ! udpsink host=127.0.0.1 port=5000
GstStateChangeReturn setupH26xTransmitPipeline(GstElement *pipeline, const char *device, const char *host, int port, CodecType codecType);

// gst-launch-1.0 -e v4l2src device=/dev/video0 ! 'video/x-raw,format=YUY2,width=1920,height=1080,framerate=30/1' ! videoconvert ! x264enc ! h264parse ! rtph264pay ! udpsink host=127.0.0.1 port=5000
// gst-launch-1.0 -e v4l2src device=/dev/video0 ! 'video/x-raw,format=YUY2,width=1920,height=1080,framerate=30/1' ! videoconvert ! x265enc ! h265parse ! rtph265pay ! udpsink host=127.0.0.1 port=5000
GstStateChangeReturn setupH26xTransmitPipeline_RawToH26x_SoftwareEncoding(GstElement *pipeline, const char *device, const char *host, int port, CodecType codecType);

// gst-launch-1.0 -e v4l2src device=/dev/video0 ! 'image/jpeg,width=1920,height=1080,framerate=60/1' ! jpegparse ! jpegdec ! videoconvert ! x264enc ! h264parse ! rtph264pay ! udpsink host=127.0.0.1 port=5000
// gst-launch-1.0 -e v4l2src device=/dev/video0 ! 'image/jpeg,width=1920,height=1080,framerate=60/1' ! jpegparse ! jpegdec ! videoconvert ! x265enc ! h265parse ! rtph265pay ! udpsink host=127.0.0.1 port=5000
GstStateChangeReturn setupH26xTransmitPipeline_JpegToH26x_SoftwareEncoding(GstElement *pipeline, const char *device, const char *host, int port, CodecType codecType) ;

// gst-launch-1.0 -e v4l2src device=/dev/video0 ! 'video/x-raw,format=YUY2,width=1920,height=1080,framerate=30/1' ! videoconvert ! 'video/x-raw,format=NV12' ! nvvideoconvert ! 'video/x-raw(memory:NVMM),format=NV12' ! nvv4l2h264enc ! h264parse ! rtph264pay ! udpsink host=127.0.0.1 port=5000
// gst-launch-1.0 -e v4l2src device=/dev/video0 ! 'video/x-raw,format=YUY2,width=1920,height=1080,framerate=30/1' ! videoconvert ! 'video/x-raw,format=NV12' ! nvvideoconvert ! 'video/x-raw(memory:NVMM),format=NV12' ! nvv4l2h265enc ! h265parse ! rtph265pay ! udpsink host=127.0.0.1 port=5000
GstStateChangeReturn setupH26xTransmitPipeline_RawToH26x_HardwareEncoding(GstElement *pipeline, const char *device, const char *host, int port, CodecType codecType);

// gst-launch-1.0 -e v4l2src device=/dev/video0 ! 'image/jpeg,width=1920,height=1080,framerate=60/1' ! jpegparse ! jpegdec ! videoconvert ! 'video/x-raw,format=NV12' ! nvvideoconvert ! 'video/x-raw(memory:NVMM),format=NV12' ! nvv4l2h264enc ! h264parse ! rtph264pay ! udpsink host=127.0.0.1 port=5000
// gst-launch-1.0 -e v4l2src device=/dev/video0 ! 'image/jpeg,width=1920,height=1080,framerate=60/1' ! jpegparse ! jpegdec ! videoconvert ! 'video/x-raw,format=NV12' ! nvvideoconvert ! 'video/x-raw(memory:NVMM),format=NV12' ! nvv4l2h265enc ! h265parse ! rtph265pay ! udpsink host=127.0.0.1 port=5000
GstStateChangeReturn setupH26xTransmitPipeline_JpegToH26x_HardwareEncoding(GstElement *pipeline, const char *device, const char *host, int port, CodecType codecType) ;

#endif // TRANSMIT_PIPELINES_H
