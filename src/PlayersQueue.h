#ifndef PLAYERS_QUEUE_H
#define PLAYERS_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include "Player.h"

class PlayersQueue {
public:
    void enqueue(std::shared_ptr<Player> player);
    std::shared_ptr<Player> dequeue();

private:
    std::queue<std::shared_ptr<Player>> queue_;
    std::mutex mutex_;
    std::condition_variable condition_;
};

#endif // PLAYERS_QUEUE_H
