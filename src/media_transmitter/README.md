# GStreamerQML
```bash
g++ -o video_streamer src/media_transmitter/video_streamer.cpp `pkg-config --cflags --libs gstreamer-1.0`
./video_streamer

g++ -o audio_streamer src/media_transmitter/audio_streamer.cpp `pkg-config --cflags --libs gstreamer-1.0`
./audio_streamer


960x540 (qHD): quarter HD
1280x720 (HD): High Definition
1920x1080 (FHD): Full High Definition