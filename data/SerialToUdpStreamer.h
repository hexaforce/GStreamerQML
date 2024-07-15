#ifndef SERIAL_TO_UDP_STREAMER_H
#define SERIAL_TO_UDP_STREAMER_H

#include <string>

class SerialToUdpStreamer {
public:
    SerialToUdpStreamer(const std::string& serialPort, unsigned int baudRate, const std::string& udpIp, unsigned short udpPort);
    ~SerialToUdpStreamer();
    void startStreaming();

private:
    int serial_port;
    int udp_socket;
    std::string serial_port_name;
    unsigned int baud_rate;
    std::string udp_ip;
    unsigned short udp_port;

    void setupSerialPort();
    void setupUdpSocket();
};

#endif // SERIAL_TO_UDP_STREAMER_H
