#include "SerialToUdpStreamer.h"
#include "common_telemetry_socket.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <send|receive> <serial_port> <baud_rate> <udp_ip> <udp_port>\n";
        return 1;
    }

    std::string mode = argv[1];
    if (mode == "send" && argc == 6)
    {
        std::string serial_port_name = argv[2];
        unsigned int baud_rate = std::stoi(argv[3]);
        std::string udp_ip = argv[4];
        unsigned short udp_port = std::stoi(argv[5]);

        try
        {
            SerialToUdpStreamer streamer(serial_port_name, baud_rate, udp_ip, udp_port);
            streamer.startStreaming();
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    }
    else if (mode == "receive" && argc == 4)
    {
        std::string udp_ip = argv[2];
        unsigned short udp_port = std::stoi(argv[3]);

        try
        {
            CommonTelemetrySocket receiver(udp_ip, udp_port);
            receiver.startListening();
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    }
    else
    {
        std::cerr << "Invalid usage or incorrect number of arguments.\n";
        std::cerr << "Usage: " << argv[0] << " <send|receive> <serial_port> <baud_rate> <udp_ip> <udp_port>\n";
        return 1;
    }

    return 0;
}
