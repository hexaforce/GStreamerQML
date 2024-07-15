#include <iostream>
#include "SerialToUdpStreamer.h"
#include "UdpReceiver.h"
#include <stdexcept>
#include <termios.h>

int main(int argc, char *argv[])
{
    if (argc < 6)
    {
        std::cerr << "Usage: " << argv[0] << " <send|receive> <serial_port> <baud_rate> <udp_ip> <udp_port>" << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    std::string serial_port_name = argv[2];
    unsigned int baud_rate = std::stoi(argv[3]);
    std::string udp_ip = argv[4];
    unsigned short udp_port = std::stoi(argv[5]);

    try
    {
        if (mode == "send")
        {
            SerialToUdpStreamer streamer(serial_port_name, baud_rate, udp_ip, udp_port);
            streamer.startStreaming();
        }
        else if (mode == "receive")
        {
            UdpReceiver receiver(udp_ip, udp_port);
            receiver.startListening();
        }
        else
        {
            std::cerr << "Unknown mode: " << mode << std::endl;
            return 1;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
