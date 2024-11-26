#include "Player.h"
#include "Session.h"

std::atomic<int> Player::idCounter_{1};

Player::Player(std::shared_ptr<Session> session)
    : session_(session), id_(idCounter_++) {
}

int Player::id() const {
    return id_;
}

void Player::send(const std::string& message) {
    if (session_) {
        session_->send(message);
    }
}

void Player::close() {
    if (session_) {
        session_->close();
    }
}

std::shared_ptr<Session> Player::getSession() {
    return session_;
}
