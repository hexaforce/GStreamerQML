#ifndef UDP_RECEIVER_H
#define UDP_RECEIVER_H

class UdpReceiver {
public:
    UdpReceiver(unsigned short udpPort);
    ~UdpReceiver();
    void startReceiving();

private:
    int udp_socket;
    unsigned short udp_port;

    void setupUdpSocket();
};

#endif // UDP_RECEIVER_H
