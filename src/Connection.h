//
// Created by crucer on 22/11/24.
//

#ifndef CONNECTION_H
#define CONNECTION_H

struct Connection {
    int socket_fd; // Descriptor de socket (ejemplo)
    // Puedes agregar más información relevante, como dirección IP, estado, etc.

    explicit Connection(int fd);
    // Otros constructores o métodos si son necesarios
};

#endif // CONNECTION_H

