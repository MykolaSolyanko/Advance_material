#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Connection
    : public boost::enable_shared_from_this<Connection>, private boost::noncopyable {
public:
  using ClientConnection = boost::shared_ptr<Connection>;

  static ClientConnection create(boost::asio::io_service &io_service) {
    return ClientConnection(new Connection(io_service));
  }

  tcp::socket &socket() {
    return socket_;
  }

  void start() {
    boost::asio::async_read_until(socket_, request_, '\n',
            boost::bind(&Connection::handle_read,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }

private:
  Connection(boost::asio::io_service &io_service)
      : socket_(io_service) {
  }
  void handle_read(const boost::system::error_code &err,
                   size_t bytes_transferred) {
    if (err) {
      std::cout << "Error when read request from client " << err.message() << std::endl; 
      return;
    }
    std::iostream in(&request_);
    std::string request_from_client;
    std::getline(in, request_from_client);
    std::cout << "response from client " << request_from_client << std::endl;
    std::string response_to_client = "Hello From server\n";
    boost::asio::async_write(socket_, boost::asio::buffer(response_to_client),
      [] (const boost::system::error_code &err, size_t bytes_transferred) {
        if (err) {
          std::cout << "Error when send response to client " << err.message() << std::endl; 
          return;
        }
        std::cout << "Data success send into client" << std::endl;;
      });
  }
  tcp::socket socket_;
  boost::asio::streambuf request_;
};

class TCPServer {
public:
  TCPServer(boost::asio::io_service &io_service)
      : acceptor_(io_service, tcp::endpoint(tcp::v4(), 3333)) {
    start_accept();
  }

private:
  void start_accept() {
    Connection::ClientConnection new_connection =
        Connection::create(acceptor_.get_io_service());

    acceptor_.async_accept(new_connection->socket(),
                           boost::bind(&TCPServer::handle_accept, this, new_connection,
                                       boost::asio::placeholders::error));
  }

  void handle_accept(Connection::ClientConnection new_connection,
                     const boost::system::error_code &error) {
    if (!error) {
      new_connection->start();
    }
    start_accept();
  }
  tcp::acceptor acceptor_;
};