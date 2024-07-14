// 簡単な例（エラーチェックやエラーハンドリングは省略）
// OpenCV を使用してビデオフレームをキャプチャし、UDP ソケットで送信する例

#include <opencv2/opencv.hpp>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace cv;
using namespace std;

int main() {
    // UDP ソケットの作成
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // 送信先のアドレスとポートの設定
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);  // 送信先ポート番号
    inet_pton(AF_INET, "192.168.1.100", &addr.sin_addr);  // 送信先IPアドレス

    // カメラの初期化
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Error opening video capture" << endl;
        return -1;
    }

    Mat frame;
    while (true) {
        cap >> frame;  // フレームをキャプチャ

        // 画像データを UDP ソケットで送信
        sendto(sockfd, frame.data, frame.total() * frame.elemSize(), 0,
               (struct sockaddr*)&addr, sizeof(addr));

        // キーボード入力（'q' を入力すると終了）
        char key = (char)waitKey(30);
        if (key == 'q') break;
    }

    // ソケットを閉じる
    close(sockfd);

    return 0;
}
