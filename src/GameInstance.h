//
// Created by crucer on 22/11/24.
//

#ifndef GAME_INSTANCE_H
#define GAME_INSTANCE_H

#include "Player.h"
#include <thread>
#include <mutex>
#include <atomic>

class GameInstance {
private:
    Player player1;
    Player player2;
    std::atomic<bool> active;
    std::thread gameThread;

    void runGame();

public:
    GameInstance(Player p1, Player p2);
    ~GameInstance();

    void start();
};

#endif // GAME_INSTANCE_H
