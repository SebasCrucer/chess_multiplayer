//
// Created by crucer on 22/11/24.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "Connection.h"
#include <memory>

struct Player {
    std::shared_ptr<Connection> connection;
    explicit Player(std::shared_ptr<Connection> conn);
};

#endif // PLAYER_H

