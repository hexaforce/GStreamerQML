# GStreamerQML Data
```bash
g++ -o UdpStreamer SerialToUdpStreamer.cpp common_telemetry_socket.cpp main.cpp

./UdpStreamer send シリアルポート ボーレート 送信先IPアドレス 受信側ポート
./UdpStreamer send /dev/ttyUSB0 115200 127.0.0.1 5005

./UdpStreamer receive 受信許可IPアドレス 受信ポート
./UdpStreamer receive 0.0.0.0 5005



# テスト
./src/blackbox/UdpSender  127.0.0.1 5005