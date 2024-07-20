// common_telemetry_socket.cpp
#include "common_telemetry_socket.h"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>

CommonTelemetrySocket::CommonTelemetrySocket(const std::string &ip, unsigned short port)
    : udp_ip(ip), udp_port(port)
{
    // Initialize socket, bind, etc.
    setupUdpSocket(); // setupUdpSocket 関数を呼び出す
}

CommonTelemetrySocket::~CommonTelemetrySocket()
{
    close(udp_socket);
}

void CommonTelemetrySocket::setupUdpSocket()
{
    // Create UDP socket
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket == -1)
    {
        throw std::runtime_error("Failed to create UDP socket");
    }

    // Prepare sockaddr_in structure
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(udp_ip.c_str());
    server_addr.sin_port = htons(udp_port);

    // Bind
    if (bind(udp_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        throw std::runtime_error("Bind failed");
    }
}

void CommonTelemetrySocket::startListening()
{
    char buffer[1024];
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int recv_len;

    while (true)
    {
        // Receive data
        recv_len = recvfrom(udp_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (recv_len == -1)
        {
            std::cerr << "Error in receiving data" << std::endl;
            continue;
        }

        // Process received data (example)
        std::cout << "Received: " << buffer << std::endl;
        
    }
}
