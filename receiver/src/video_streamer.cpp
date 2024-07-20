// g++ -o video_streamer video_streamer.cpp `pkg-config --cflags --libs gstreamer-1.0`

// if (argc != 4) {
//   g_printerr("Usage: %s <device> <host> <port>\n", argv[0]);
//   return -1;
// }

// const char *device = argv[1];
// const char *host   = argv[2];
// int         port   = atoi(argv[3]);

#include <gst/gst.h>
#include "transmit_pipelines.h"

int main(int argc, char *argv[]) {

  gst_init(&argc, &argv);

  {
    const char *device = "/dev/video0";
    const char *host   = "127.0.0.1"; // destinationAddress
    int         port   = 5000;        // videoPort

    // Create elements
    GstElement *pipeline = gst_pipeline_new(NULL);

    // GstStateChangeReturn ret = setupJPEGTransmitPipeline(pipeline, device, host, port);
    GstStateChangeReturn ret = setupH26xTransmitPipeline(pipeline, device, host, port, CodecType::H264);
    // GstStateChangeReturn ret_h264 = setupH26xTransmitPipeline_JpegToH26x_SoftwareEncoding(pipeline, device, host, port, CodecType::H264);
    // GstStateChangeReturn ret_h264 = setupH26xTransmitPipeline_RawToH26x_SoftwareEncoding(pipeline, device, host, port, CodecType::H264);

    // GstStateChangeReturn ret = setupH265TransmitPipeline(pipeline, device, host, port);
    if (ret == GST_STATE_CHANGE_FAILURE) {
      g_printerr("Failed to start pipeline.\n");
      return -1;
    }

    // Wait until error or EOS
    GstBus     *bus = gst_element_get_bus(pipeline);
    GstMessage *msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_EOS);

    // Parse message
    if (msg != NULL) {
      GError *err;
      gchar  *debug_info;
      switch (GST_MESSAGE_TYPE(msg)) {
      case GST_MESSAGE_ERROR:
        gst_message_parse_error(msg, &err, &debug_info);
        g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(msg->src), err->message);
        g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
        g_clear_error(&err);
        g_free(debug_info);
        break;
      case GST_MESSAGE_EOS:
        g_print("End-Of-Stream reached.\n");
        break;
      default:
        // Should not be reached
        g_printerr("Unexpected message received.\n");
        break;
      }
      gst_message_unref(msg);
    }

    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
  }

  return 0;
}
