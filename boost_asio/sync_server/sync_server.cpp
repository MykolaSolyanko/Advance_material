#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <atomic>
#include <thread>

static std::atomic<bool> stop_server{false};

void HandleClientSocket(boost::asio::ip::tcp::socket& act_socket) {
  std::cout << "accept client request\n";
  boost::asio::streambuf buff;
  boost::asio::read_until(act_socket, buff, '\n');
  std::iostream in(&buff);
  std::string response_from_client;
  std::getline(in, response_from_client);
  std::cout << "response from client " << response_from_client << std::endl;
  response_from_client = "Hello From server\n";
  boost::asio::write(act_socket, boost::asio::buffer(response_from_client));
}

class WaitClientConnectionRequest {
  public:
    WaitClientConnectionRequest(boost::asio::io_service& io_ser, unsigned short port) : service{io_ser},
      acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4()/* boost::asio::ip::address::from_string(ip_addr) */, port)) {
        acceptor.listen();
    }
    void AcceptClient() {
      boost::asio::ip::tcp::socket active_socket{service};
      acceptor.accept(active_socket);
      HandleClientSocket(active_socket);
    }
  private:
    std::string ip_addr{"172.17.0.2"};
    boost::asio::io_service& service;
    boost::asio::ip::tcp::acceptor acceptor;  
};

int main(int argc, const char** argv) {
  boost::asio::io_service service;
  std::thread([&service]() {
    WaitClientConnectionRequest passive_wait(service,4444); 
    while (!stop_server) {
      passive_wait.AcceptClient();
    }
  }).join();
  return 0;
}