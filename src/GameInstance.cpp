//
// Created by crucer on 22/11/24.
//

#include "GameInstance.h"
#include <iostream>
#include <unistd.h> // Para read() y write()

GameInstance::GameInstance(Player p1, Player p2)
    : player1(std::move(p1)), player2(std::move(p2)), active(true) {}

GameInstance::~GameInstance() {
    if (gameThread.joinable()) {
        gameThread.join();
    }
}

void GameInstance::start() {
    gameThread = std::thread(&GameInstance::runGame, this);
}

void GameInstance::runGame() {
    // Aquí implementarías la lógica del juego de ajedrez
    // Por simplicidad, enviaremos mensajes básicos entre los jugadores

    // Notificar a los jugadores que han sido emparejados
    std::string const msg = "Has sido emparejado. Comienza el juego.\n";
    write(player1.connection->socket_fd, msg.c_str(), msg.size());
    write(player2.connection->socket_fd, msg.c_str(), msg.size());

    // Comunicación básica: eco de mensaje
    while (active) {
        char buffer[1024];
        // Leer del jugador 1 y enviar al jugador 2
        ssize_t bytes_read = read(player1.connection->socket_fd, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            std::cout << "Jugador 1 desconectado.\n";
            active = false;
            break;
        }
        write(player2.connection->socket_fd, buffer, bytes_read);

        // Leer del jugador 2 y enviar al jugador 1
        bytes_read = read(player2.connection->socket_fd, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            std::cout << "Jugador 2 desconectado.\n";
            active = false;
            break;
        }
        write(player1.connection->socket_fd, buffer, bytes_read);
    }

    // Cerrar las conexiones
    close(player1.connection->socket_fd);
    close(player2.connection->socket_fd);
}
