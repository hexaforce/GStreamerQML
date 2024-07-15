#include "UdpReceiver.h"

int main() {
    const unsigned short udp_port = 5005; // 受信側のポート

    try {
        UdpReceiver receiver(udp_port);
        receiver.startReceiving();
    } catch (const std::exception& e) {
        std::cerr << "エラー: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
