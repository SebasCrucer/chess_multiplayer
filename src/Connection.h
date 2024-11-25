//
// Created by crucer on 22/11/24.
//

#ifndef CONNECTION_H
#define CONNECTION_H

struct Connection {
    int socket_fd; // Descriptor de socket

    explicit Connection(int fd);
};

#endif // CONNECTION_H

