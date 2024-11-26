#include "src/ChessServer.h"

int main() {
    unsigned short port = 8080;
    ChessServer server(port);
    server.run();
    return 0;
}