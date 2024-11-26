#include "Session.h"
#include "Player.h"
#include <iostream>

Session::Session(boost::asio::ip::tcp::socket socket, PlayersQueue& playersQueue, boost::asio::io_context& ioContext)
    : ws_(std::move(socket)), playersQueue_(playersQueue), ioContext_(ioContext) {
}

void Session::start() {
    ws_.async_accept([self = shared_from_this()](boost::system::error_code ec) {
        if (!ec) {
            // Create a new player and enqueue
            auto player = std::make_shared<Player>(self);
            self->playersQueue_.enqueue(player);
            // Start reading messages
            self->doRead();
        } else {
            std::cerr << "WebSocket accept error: " << ec.message() << std::endl;
            self->close();
        }
    });
}

void Session::doRead() {
    ws_.async_read(buffer_, [self = shared_from_this()](boost::system::error_code ec, std::size_t bytesTransferred) {
        self->onRead(ec, bytesTransferred);
    });
}

void Session::onRead(boost::system::error_code ec, std::size_t /*bytesTransferred*/) {
    if (!ec) {
        std::string message = boost::beast::buffers_to_string(buffer_.data());
        buffer_.consume(buffer_.size());

        // Call the message handler
        if (messageHandler_) {
            messageHandler_(message);
        }

        // Continue reading
        doRead();
    } else if (ec == boost::beast::websocket::error::closed) {
        // Connection closed cleanly
        if (closeHandler_) {
            closeHandler_();
        }
    } else {
        std::cerr << "WebSocket read error: " << ec.message() << std::endl;
    }
}

void Session::send(const std::string& message) {
    ws_.text(true);
    ws_.async_write(boost::asio::buffer(message), [self = shared_from_this()](boost::system::error_code ec, std::size_t bytesTransferred) {
        self->onWrite(ec, bytesTransferred);
    });
}

void Session::onWrite(boost::system::error_code ec, std::size_t /*bytesTransferred*/) {
    if (ec) {
        std::cerr << "WebSocket write error: " << ec.message() << std::endl;
    }
}

void Session::close() {
    ws_.async_close(boost::beast::websocket::close_code::normal, [self = shared_from_this()](boost::system::error_code ec) {
        if (ec) {
            std::cerr << "WebSocket close error: " << ec.message() << std::endl;
        }
    });
}

void Session::setMessageHandler(std::function<void(const std::string&)> handler) {
    messageHandler_ = std::move(handler);
}

void Session::setCloseHandler(std::function<void()> handler) {
    closeHandler_ = std::move(handler);
}
