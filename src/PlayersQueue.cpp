#include "PlayersQueue.h"

void PlayersQueue::enqueue(std::shared_ptr<Player> player) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(player);
    }
    condition_.notify_one();
}

std::shared_ptr<Player> PlayersQueue::dequeue() {
    std::unique_lock<std::mutex> lock(mutex_);
    condition_.wait(lock, [this]() { return !queue_.empty(); });
    auto player = queue_.front();
    queue_.pop();
    return player;
}
