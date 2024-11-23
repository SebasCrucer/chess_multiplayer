//
// Created by crucer on 22/11/24.
//

#ifndef PLAYERSQUEUE_H
#define PLAYERSQUEUE_H

#include "Player.h"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>


// Clase para manejar la cola de jugadores de manera segura en entornos multihilo
class PlayersQueue {
private:
    std::queue<Player> queue_;                 // Cola de jugadores
    std::mutex mutex_;                         // Mutex para proteger la cola
    std::condition_variable cv_;               // Condición para esperar/notificar

public:
    // Agrega un jugador a la cola
    void enqueue(const Player& player);

    // Remueve y retorna el siguiente jugador de la cola
    Player dequeue();

    // Verifica si la cola está vacía
    bool isEmpty();

    // Obtiene el tamaño de la cola
    size_t size();
};

#endif // PLAYERSQUEUE_H