#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <functional>
#include <memory>

boost::asio::io_service service;
// Structure represents a context of a single request.
struct Session
{
    Session(boost::asio::io_service &ios,
            unsigned short port_num,
            const std::string &request) : m_sock(ios),
                                 m_ep(boost::asio::ip::tcp::v4(),
                                      port_num),
                                 m_request(request),
                                 m_was_cancelled(false) {}

    boost::asio::ip::tcp::socket m_sock; // Socket used for communication
    boost::asio::ip::tcp::endpoint m_ep; // Remote endpoint.
    std::string m_request;
    // Request string.
    // streambuf where the response will be stored.
    boost::asio::streambuf m_response_buf;
    std::string m_response;
    bool m_was_cancelled;
};

void connect_to_server(unsigned short port_num)
{

  // Preparing the request string.
  std::string request = "Hello From Client\n";

  std::shared_ptr<Session> session =
      std::shared_ptr<Session>(new Session(service,
                                           port_num,
                                           request));

  session->m_sock.open(session->m_ep.protocol());

  session->m_sock.async_connect(session->m_ep,
                                [session](const boost::system::error_code &ec) {
                                  if (ec)
                                  {
                                    std::cout << "Error when connected to server " << ec.message() << std::endl;
                                    return;
                                  }

                                  boost::asio::async_write(session->m_sock, boost::asio::buffer(session->m_request),
                                                           [session](const boost::system::error_code &ec,
                                                                     std::size_t bytes_transferred) {
                                                             if (ec)
                                                             {
                                                               std::cout << "Error when send request to server " << ec.message() << std::endl;
                                                               return;
                                                             }

                                                             boost::asio::async_read_until(session->m_sock, session->m_response_buf,
                                                                                           '\n',
                                                                                           [session](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                                                                                             if (ec)
                                                                                             {
                                                                                               std::cout << "Error when send request to server " << ec.message() << std::endl;
                                                                                               return;
                                                                                             }
                                                                                             std::istream strm(&session->m_response_buf);
                                                                                             std::getline(strm, session->m_response);
                                                                                             std::cout << "responce from server " << session->m_response << std::endl;
                                                                                           });
                                                           });
                                });
}

int main(int argc, char const *argv[]) {
    connect_to_server(3333);
    service.run();
    return 0;
}
