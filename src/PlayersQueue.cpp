//
// Created by crucer on 22/11/24.
//

#include "PlayersQueue.h"

#include <utility>

void PlayersQueue::enqueue(const Player& player) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(player);
    cv_.notify_one(); // Notifica a un hilo que espera
}

Player PlayersQueue::dequeue() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this] { return !queue_.empty(); }); // Espera hasta que la cola no esté vacía
    Player player = queue_.front();
    queue_.pop();
    return player;
}

bool PlayersQueue::isEmpty() {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}

size_t PlayersQueue::size() {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
}
