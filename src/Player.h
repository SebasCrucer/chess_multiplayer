#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <boost/beast.hpp>

class Session; // Forward declaration

class Player {
public:
    explicit Player(std::shared_ptr<Session> session);

    int id() const;

    void send(const std::string& message);
    void close();

    std::shared_ptr<Session> getSession();

private:
    std::shared_ptr<Session> session_;
    int id_;
    static std::atomic<int> idCounter_;
};

#endif // PLAYER_H
