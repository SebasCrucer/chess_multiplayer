//
// Created by crucer on 22/11/24.
//

#ifndef CHESS_SERVER_H
#define CHESS_SERVER_H

#include "PlayersQueue.h"
#include "GameInstance.h"
#include <thread>
#include <vector>
#include <atomic>

class ChessServer {
private:
    int server_fd; // Descriptor del socket del servidor
    PlayersQueue playersQueue;
    std::vector<std::thread> gameThreads; // Hilos para las instancias de juego
    std::vector<std::shared_ptr<GameInstance>> activeGames; // Opcional: Mantener referencias a los juegos activos
    std::atomic<bool> running;

    void acceptConnections();
    void matchPlayers();

public:
    ChessServer(int port);
    ~ChessServer();

    void start();
    void stop();
};

#endif // CHESS_SERVER_H
