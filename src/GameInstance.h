#ifndef GAME_INSTANCE_H
#define GAME_INSTANCE_H

#include <memory>
#include <atomic>
#include "Player.h"

class GameInstance : public std::enable_shared_from_this<GameInstance> {
public:
    GameInstance(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2);

    void start();

    void handleMessage(std::shared_ptr<Player> sender, const std::string& message);
    void playerDisconnected(std::shared_ptr<Player> player);

private:
    std::shared_ptr<Player> player1_;
    std::shared_ptr<Player> player2_;
    std::atomic<bool> running_;
};

#endif // GAME_INSTANCE_H
