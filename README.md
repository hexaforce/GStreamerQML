# GStreamerQML
```bash

gst-device-monitor-1.0 Video/Source
gst-device-monitor-1.0 Audio/Source
gst-device-monitor-1.0 Audio/Sink
gst-inspect-1.0
gst-launch-1.0

░░░░░░
gst-launch-1.0 audiotestsrc ! audioconvert ! audioresample ! autoaudiosink
gst-launch-1.0 videotestsrc ! videoconvert ! autovideosink

gst-launch-1.0 audiotestsrc ! audioconvert ! audioresample ! audio/x-raw,format=S16LE,channels=2,rate=44100 ! filesink location=test.wav

gst-launch-1.0 -v pulsesrc ! audioconvert ! audioresample ! audio/x-raw,format=S16LE,channels=2,rate=44100 ! voaacenc bitrate=128000 ! aacparse ! rtpmp4apay ! udpsink host=127.0.0.1 port=5000
gst-launch-1.0 -v videotestsrc ! videoconvert ! x264enc bitrate=2000 ! h264parse ! rtph264pay ! udpsink host=127.0.0.1 port=5000

