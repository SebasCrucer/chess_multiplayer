#include "src/ChessServer.h"

int main() {
    int const port = 8080; // Puerto donde escuchará el servidor
    ChessServer server(port);

    server.start();

    // Mantener el servidor en ejecución
    // Puedes manejar señales para detener el servidor de manera ordenada
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}