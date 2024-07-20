// common_telemetry_socket.h
#ifndef CommonTelemetrySocket_H
#define CommonTelemetrySocket_H

#include <string>

class CommonTelemetrySocket
{
public:
    CommonTelemetrySocket(const std::string &ip, unsigned short port);
    ~CommonTelemetrySocket();

    void startListening();

private:
    int udp_socket;
    std::string udp_ip;
    unsigned short udp_port;

    void setupUdpSocket(); // 関数の宣言を追加
};

#endif // CommonTelemetrySocket_H
