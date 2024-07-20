#include "SerialToUdpStreamer.h"
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

SerialToUdpStreamer::SerialToUdpStreamer(const std::string &serialPort, unsigned int baudRate, const std::string &udpIp, unsigned short udpPort) : serial_port_name(serialPort), baud_rate(baudRate), udp_ip(udpIp), udp_port(udpPort), serial_port(-1), udp_socket(-1) {}

SerialToUdpStreamer::~SerialToUdpStreamer() {
  if (serial_port >= 0) {
    close(serial_port);
  }
  if (udp_socket >= 0) {
    close(udp_socket);
  }
}

void SerialToUdpStreamer::setupSerialPort() {
  serial_port = open(serial_port_name.c_str(), O_RDWR);
  if (serial_port < 0) {
    throw std::runtime_error("シリアルポートを開けませんでした。");
  }

  termios tty;
  memset(&tty, 0, sizeof(tty));
  if (tcgetattr(serial_port, &tty) != 0) {
    close(serial_port);
    throw std::runtime_error("シリアルポートの属性を取得できませんでした。");
  }

  cfsetospeed(&tty, baud_rate);
  cfsetispeed(&tty, baud_rate);

  tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
  tty.c_iflag &= ~IGNBRK;
  tty.c_lflag = 0;
  tty.c_oflag = 0;
  tty.c_cc[VMIN] = 1;
  tty.c_cc[VTIME] = 5;
  tty.c_iflag &= ~(IXON | IXOFF | IXANY);
  tty.c_cflag |= (CLOCAL | CREAD);
  tty.c_cflag &= ~(PARENB | PARODD);
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CRTSCTS;

  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
    close(serial_port);
    throw std::runtime_error("シリアルポートの属性を設定できませんでした。");
  }
}

void SerialToUdpStreamer::setupUdpSocket() {
  udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (udp_socket < 0) {
    throw std::runtime_error("UDPソケットを作成できませんでした。");
  }
}

void SerialToUdpStreamer::startStreaming() {
  setupSerialPort();
  setupUdpSocket();

  sockaddr_in remote_addr;
  memset(&remote_addr, 0, sizeof(remote_addr));
  remote_addr.sin_family = AF_INET;
  remote_addr.sin_port = htons(udp_port);
  inet_pton(AF_INET, udp_ip.c_str(), &remote_addr.sin_addr);

  char buffer[1024];
  while (true) {
    int num_bytes = read(serial_port, buffer, sizeof(buffer));
    if (num_bytes < 0) {
      std::cerr << "シリアルポートから読み取れませんでした。" << std::endl;
      break;
    }
    sendto(udp_socket, buffer, num_bytes, 0, (sockaddr *)&remote_addr, sizeof(remote_addr));
  }
}
