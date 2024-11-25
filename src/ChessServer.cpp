//
// Created by crucer on 22/11/24.
//

#include "ChessServer.h"
#include "Connection.h"
#include "GameInstance.h"
#include <iostream>
#include <unistd.h> // Para close()
#include <arpa/inet.h> // Para inet_ntoa
#include <cstring>

ChessServer::ChessServer(int const port = 8080) : running(false) {
    // Crear socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << server_fd <<std::endl;

    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int constexpr opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    sockaddr_in address{};

    std::memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY; // Escuchar en todas las interfaces

    // Vincular el socket
    if (bind(server_fd, reinterpret_cast<sockaddr *>(&address), sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Escuchar conexiones
    if (listen(server_fd, 10) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Chess Server escuchando en el puerto " << port << std::endl;
}

ChessServer::~ChessServer() {
    stop();
    close(server_fd);
}

void ChessServer::start() {
    running = true;

    // Hilo para aceptar conexiones
    std::thread(&ChessServer::acceptConnections, this).detach();

    // Hilo para emparejar jugadores
    std::thread(&ChessServer::matchPlayers, this).detach();
}

void ChessServer::stop() {
    running = false;
}

void ChessServer::acceptConnections() {
    while (running) {
        sockaddr_in client_address{};
        socklen_t client_len = sizeof(client_address);
        int new_socket = accept(server_fd, reinterpret_cast<struct sockaddr *>(&client_address), &client_len);
        if (new_socket < 0) {
            perror("accept");
            continue;
        }

        std::cout << "Nueva conexión desde " << inet_ntoa(client_address.sin_addr)
                  << ":" << ntohs(client_address.sin_port) << ", socket fd: " << new_socket << std::endl;

        // Crear una nueva conexión y jugador
        auto const connection = std::make_shared<Connection>(new_socket);
        Player player(connection);

        // Añadir el jugador a la cola
        playersQueue.enqueue(player);
    }
}

void ChessServer::matchPlayers() {
    while (running) {
        // Espera hasta que al menos dos jugadores estén disponibles
        auto player1 = playersQueue.dequeue();
        auto player2 = playersQueue.dequeue();

        std::cout << "Emparejando jugadores: "
                  << player1.connection->socket_fd << " y "
                  << player2.connection->socket_fd << std::endl;

        auto game = std::make_shared<GameInstance>(player1, player2);

        // Iniciar el juego en un nuevo hilo, pasando el shared_ptr
        gameThreads.emplace_back([game]() {
            game->start();
        });
    }
}