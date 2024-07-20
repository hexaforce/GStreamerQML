#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

void sendDummyData(const std::string &udp_ip, unsigned short udp_port) {
  int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sock == -1) {
    std::cerr << "Failed to create socket" << std::endl;
    return;
  }

  struct sockaddr_in addr;
  std::memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(udp_port);

  if (inet_pton(AF_INET, udp_ip.c_str(), &addr.sin_addr) != 1) {
    std::cerr << "Invalid IP address" << std::endl;
    close(sock);
    return;
  }

  const char *message = "Hello, UDP!";
  ssize_t sent = sendto(sock, message, std::strlen(message), 0, (struct sockaddr *)&addr, sizeof(addr));
  if (sent == -1) {
    std::cerr << "Failed to send data" << std::endl;
  } else {
    std::cout << "Sent data to " << udp_ip << ":" << udp_port << std::endl;
  }

  close(sock);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <udp_ip> <udp_port>" << std::endl;
    return 1;
  }

  std::string udp_ip = argv[1];
  unsigned short udp_port = std::stoi(argv[2]);

  sendDummyData(udp_ip, udp_port);

  return 0;
}
