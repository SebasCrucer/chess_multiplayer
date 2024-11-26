#include "GameInstance.h"
#include <iostream>
#include <thread>

#include "Session.h"

GameInstance::GameInstance(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2)
    : player1_(player1), player2_(player2), running_(true) {
}

void GameInstance::start() {
    // Notify players that they are paired
    player1_->send("PAIR");
    player2_->send("PAIR");

    std::shared_ptr player1_session = player1_->getSession();

    std::shared_ptr player2_session = player2_->getSession();

    player1_session->setMessageHandler([self = shared_from_this()](const std::string& message) {
            self->handleMessage(self->player1_, message);
        });

    player2_session->setMessageHandler(
        [self = shared_from_this()](const std::string& message) {
            self->handleMessage(self->player2_, message);
        });

    player1_session->setCloseHandler(
        [self = shared_from_this()]() {
            self->playerDisconnected(self->player1_);
        });

    player2_session->setCloseHandler(
        [self = shared_from_this()]() {
            self->playerDisconnected(self->player2_);
        });

    // Keep the game running
    while (running_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void GameInstance::handleMessage(std::shared_ptr<Player> sender, const std::string& message) {
    // Echo the message to the other player
    if (sender == player1_) {
        player2_->send(message);
    } else {
        player1_->send(message);
    }
}

void GameInstance::playerDisconnected(std::shared_ptr<Player> player) {
    running_ = false;
    std::string disconnectMessage = "DISCONNECT";
    if (player == player1_) {
        player2_->send(disconnectMessage);
        player2_->close();
    } else {
        player1_->send(disconnectMessage);
        player1_->close();
    }
    std::cout << "Player " << player->id() << " disconnected." << std::endl;
}
