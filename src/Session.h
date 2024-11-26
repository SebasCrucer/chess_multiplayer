#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>
#include <functional>
#include "PlayersQueue.h"

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(boost::asio::ip::tcp::socket socket, PlayersQueue& playersQueue, boost::asio::io_context& ioContext);

    void start();

    void send(const std::string& message);
    void close();

    void setMessageHandler(std::function<void(const std::string&)> handler);
    void setCloseHandler(std::function<void()> handler);

private:
    void doRead();
    void onRead(boost::system::error_code ec, std::size_t bytesTransferred);
    void onWrite(boost::system::error_code ec, std::size_t bytesTransferred);

    void doClose();

    boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws_;
    boost::beast::flat_buffer buffer_;

    PlayersQueue& playersQueue_;
    boost::asio::io_context& ioContext_;

    std::function<void(const std::string&)> messageHandler_;
    std::function<void()> closeHandler_;
};

#endif // SESSION_H
