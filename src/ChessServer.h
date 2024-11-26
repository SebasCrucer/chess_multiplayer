#ifndef CHESS_SERVER_H
#define CHESS_SERVER_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <thread>
#include <atomic>
#include <memory>
#include <vector>
#include <unordered_map>
#include <mutex>
#include "Player.h"
#include "GameInstance.h"
#include "PlayersQueue.h"

class ChessServer {
public:
    explicit ChessServer(unsigned short port);
    ~ChessServer();

    void run();

private:
    void doAccept();
    void onAccept(boost::system::error_code ec, boost::asio::ip::tcp::socket socket);

    boost::asio::io_context ioContext_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::atomic<bool> running_;

    PlayersQueue playersQueue_;
    std::vector<std::thread> gameThreads_;

    // mapa de juegos activos
    std::unordered_map<int, std::shared_ptr<GameInstance>> activeGames_;
    std::mutex gamesMutex_;
};

#endif // CHESS_SERVER_H
