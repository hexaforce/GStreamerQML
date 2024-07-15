// Udp_receiver.h
#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <string>

class UdpReceiver
{
public:
    UdpReceiver(const std::string &ip, unsigned short port);
    ~UdpReceiver();

    void startListening();

private:
    int udp_socket;
    std::string udp_ip;
    unsigned short udp_port;

    void setupUdpSocket(); // 関数の宣言を追加
};

#endif // UDPRECEIVER_H
