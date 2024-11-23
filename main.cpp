#include "src/ChessServer.h"

int main() {
    int constexpr port = 8080;
    ChessServer server(port);

    server.start();

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}