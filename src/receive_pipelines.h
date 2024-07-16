#ifndef RECEIVE_PIPELINES_H
#define RECEIVE_PIPELINES_H

#include <gst/gst.h>

enum class CodecType { H264, H265 };
enum class VendorType { Libav, Nvidia };

// gst-launch-1.0 -e udpsrc port=5000 ! application/x-rtp, encoding-name=H264 ! rtph264depay ! h264parse ! nvh264sldec ! nvvideoconvert ! autovideosink sync=false
// gst-launch-1.0 -e udpsrc port=5000 ! application/x-rtp, encoding-name=H265 ! rtph265depay ! h265parse ! nvh265sldec ! nvvideoconvert ! autovideosink sync=false
void setupLocalCapturePipeline(GstElement *pipeline, GstElement *sink);

// gst-launch-1.0 -e udpsrc port=5000 caps="application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)JPEG, payload=(int)26" ! rtpjpegdepay ! avdec_mjpeg ! videoconvert ! autovideosink
// gst-launch-1.0 -e udpsrc port=5000 caps="application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)JPEG, payload=(int)26" ! rtpjpegdepay ! jpegdec ! videoconvert ! autovideosink
void setupJpegReceivePipeline_SoftwareDecoding(GstElement *pipeline, GstElement *sink, gint port, VendorType vendorType);

// gst-launch-1.0 -e udpsrc port=5000 ! application/x-rtp, encoding-name=H264 ! rtph264depay ! h264parse ! nvv4l2decoder ! nvvideoconvert ! autovideosink sync=false
// gst-launch-1.0 -e udpsrc port=5000 ! application/x-rtp, encoding-name=H265 ! rtph265depay ! h265parse ! nvv4l2decoder ! nvvideoconvert ! autovideosink sync=false
void setupH26xReceivePipeline_HardwareDecoding(GstElement *pipeline, GstElement *sink, gint port, CodecType codecType);

// gst-launch-1.0 -e udpsrc port=5000 ! application/x-rtp, encoding-name=H264 ! rtph264depay ! h264parse ! avdec_h264 ! videoconvert ! autovideosink
// gst-launch-1.0 -e udpsrc port=5000 ! application/x-rtp, encoding-name=H265 ! rtph265depay ! h265parse ! avdec_h265 ! videoconvert ! autovideosink
// gst-launch-1.0 -e udpsrc port=5000 ! application/x-rtp, encoding-name=H264 ! rtph264depay ! h264parse ! nvh264dec ! videoconvert ! autovideosink
// gst-launch-1.0 -e udpsrc port=5000 ! application/x-rtp, encoding-name=H265 ! rtph265depay ! h265parse ! nvh265dec ! videoconvert ! autovideosink
void setupH26xReceivePipeline_SoftwareDecoding(GstElement *pipeline, GstElement *sink, gint port, CodecType codecType, VendorType vendorType);

#endif // RECEIVE_PIPELINES_H