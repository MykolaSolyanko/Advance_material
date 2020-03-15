#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

void add_to_queue(std::string &str);

class Coonnection
    : public boost::enable_shared_from_this<Coonnection>
{
public:
  typedef boost::shared_ptr<Coonnection> pointer;

  static pointer create(boost::asio::io_service &io_service)
  {
    return pointer(new Coonnection(io_service));
  }

  tcp::socket &socket()
  {
    return socket_;
  }

  void start()
  {
    socket_.async_read_some(
        boost::asio::buffer(message_.data(), 2048),
        boost::bind(&Coonnection::handle_read,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }

private:
  Coonnection(boost::asio::io_service &io_service)
      : socket_(io_service)
  {
    message_.resize(2048);
  }

  void handle_read(const boost::system::error_code &err,
                   size_t bytes_transferred)
  {
    add_to_queue(message_);

    socket_.async_read_some(
        boost::asio::buffer(message_.data(), 2048),
        boost::bind(&Coonnection::handle_read,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }

  tcp::socket socket_;
  std::string message_;
};

class tcp_server
{
public:
  tcp_server(boost::asio::io_service &io_service)
      : acceptor_(io_service, tcp::endpoint(tcp::v4(), 13))
  {
    start_accept();
  }

private:
  void start_accept()
  {
    Coonnection::pointer new_connection =
        Coonnection::create(acceptor_.get_io_service());

    acceptor_.async_accept(new_connection->socket(),
                           boost::bind(&tcp_server::handle_accept, this, new_connection,
                                       boost::asio::placeholders::error));
  }

  void handle_accept(Coonnection::pointer new_connection,
                     const boost::system::error_code &error)
  {
    if (!error)
    {
      new_connection->start();
    }

    start_accept();
  }

  tcp::acceptor acceptor_;
};