# GStreamerQML Data
```bash
g++ -o SerialToUdpStreamer.o -c SerialToUdpStreamer.cpp
g++ -o UdpReceiver.o -c UdpReceiver.cpp
g++ -o main.o -c main.cpp
g++ -o UdpReceiver SerialToUdpStreamer.o UdpReceiver.o main.o

