#include "ChessServer.h"
#include "Session.h"
#include <iostream>

ChessServer::ChessServer(unsigned short port)
    : acceptor_(ioContext_, {boost::asio::ip::tcp::v4(), port}),
      running_(true) {
    std::cout << "Chess Server listening on port " << port << std::endl;
}

ChessServer::~ChessServer() {
    running_ = false;
    ioContext_.stop();
    if (acceptor_.is_open()) {
        acceptor_.close();
    }
}

void ChessServer::run() {
    // Start accepting connections
    doAccept();

    // Start player matching thread
    std::thread([this]() {
        while (running_) {
            auto player1 = playersQueue_.dequeue();
            auto player2 = playersQueue_.dequeue();

            std::cout << "Pairing players: " << player1->id() << " and " << player2->id() << std::endl;

            auto gameInstance = std::make_shared<GameInstance>(player1, player2);

            {
                std::lock_guard<std::mutex> lock(gamesMutex_);
                activeGames_[player1->id()] = gameInstance;
                activeGames_[player2->id()] = gameInstance;
            }

            // Start the game in a new thread
            gameThreads_.emplace_back([gameInstance]() {
                gameInstance->start();
            });
        }
    }).detach();

    // Run the IO context
    ioContext_.run();
}

void ChessServer::doAccept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
            onAccept(ec, std::move(socket));
        });
}

void ChessServer::onAccept(boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
    if (!ec) {
        std::cout << "New connection from " << socket.remote_endpoint() << std::endl;
        // Create a new session
        std::make_shared<Session>(std::move(socket), playersQueue_, ioContext_)->start();
    } else {
        std::cerr << "Accept error: " << ec.message() << std::endl;
    }

    // Continue accepting connections
    if (running_) {
        doAccept();
    }
}
