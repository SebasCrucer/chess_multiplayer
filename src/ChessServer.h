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
    int server_fd; // File descriptor del socket del servidor
    PlayersQueue playersQueue;
    std::vector<std::thread> gameThreads;
    std::vector<std::shared_ptr<GameInstance>> activeGames;
    std::atomic<bool> running;

    void acceptConnections();
    void matchPlayers();

public:
    explicit ChessServer(int port);
    ~ChessServer();

    void start();
    void stop();
};

#endif // CHESS_SERVER_H
