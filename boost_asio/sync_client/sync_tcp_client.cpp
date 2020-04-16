#include <boost/asio.hpp>
#include <string>
#include <iostream>

int main(int argc, const char** argv) {
  std::string ip_server_address{"172.17.0.2"};
  unsigned short port_number{4444};

  boost::asio::io_service service;
  boost::asio::ip::tcp::endpoint end_point{/* boost::asio::ip::address::from_string(ip_server_address) */boost::asio::ip::tcp::v4(), port_number};

  boost::asio::ip::tcp::socket client_socket{service};

  boost::system::error_code ec;
  client_socket.open(end_point.protocol(), ec);
  if (ec) {
    std::cout << ec.category().name() << ":" << ec.value() << std::endl;
    return -1;
  }

  client_socket.connect(end_point, ec);
  if (ec) {
    std::cout << ec.category().name() << ":" << ec.value() << std::endl;
    return -1;
  }

  std::string payload{"Hello From TCP client\n"};
  auto write_size_byte = boost::asio::write(client_socket, boost::asio::buffer(payload), ec);

  if (ec) {
    std::cout << ec.category().name() << ":" << ec.value() << std::endl;
    return -1;
  }

  std::cout << "success send to server: " << write_size_byte << " bytes\n"; 
  boost::asio::streambuf buff;
  auto read_size_byte = boost::asio::read_until(client_socket, buff, '\n', ec);

  std::cout << "success read from server: " << read_size_byte << " bytes\n";
  if (ec) {
    std::cout << ec.category().name() << ":" << ec.value() << std::endl;
    return -1;
  }

  std::istream in(&buff);
  std::string response;
  std::getline(in, response);
  std::cout << "response from server: " << response;
  client_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);

  return 0;
}