//
// Created by crucer on 22/11/24.
//

#include "Player.h"

Player::Player(std::shared_ptr<Connection> conn) : connection(std::move(conn)) {}
