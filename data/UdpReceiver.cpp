#include "UdpReceiver.h"
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

UdpReceiver::UdpReceiver(unsigned short udpPort)
    : udp_port(udpPort), udp_socket(-1) {}

UdpReceiver::~UdpReceiver() {
    if (udp_socket >= 0) {
        close(udp_socket);
    }
}

void UdpReceiver::setupUdpSocket() {
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket < 0) {
        throw std::runtime_error("UDPソケットを作成できませんでした。");
    }

    sockaddr_in local_addr;
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(udp_port);
    local_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(udp_socket, (sockaddr*)&local_addr, sizeof(local_addr)) < 0) {
        close(udp_socket);
        throw std::runtime_error("UDPソケットにバインドできませんでした。");
    }
}

void UdpReceiver::startReceiving() {
    setupUdpSocket();

    char buffer[1024];
    while (true) {
        sockaddr_in remote_addr;
        socklen_t addr_len = sizeof(remote_addr);
        int num_bytes = recvfrom(udp_socket, buffer, sizeof(buffer), 0, (sockaddr*)&remote_addr, &addr_len);
        if (num_bytes < 0) {
            std::cerr << "データを受信できませんでした。" << std::endl;
            break;
        }
        std::cout.write(buffer, num_bytes);
        std::cout << std::endl;
    }
}
