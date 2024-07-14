# GStreamerQML
```bash
g++ -o video_streamer transmitter/video_streamer.cpp `pkg-config --cflags --libs gstreamer-1.0`
./video_streamer


// sudo tcpdump -i any port 5000 -X
g++ -g -o message_receiver receiver/message_receiver.cpp message/video_data.pb.cc `pkg-config --cflags --libs gstreamer-1.0 gstreamer-app-1.0` -lprotobuf
./message_receiver

g++ -g -o message_streamer transmitter/message_streamer.cpp message/video_data.pb.cc `pkg-config --cflags --libs gstreamer-1.0 gstreamer-app-1.0` -lprotobuf
./message_streamer
