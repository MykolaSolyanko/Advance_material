
#include "boost_tcp_server.hpp"
#include "thread"

void Run() {
  boost::asio::io_service io_service;
  TCPServer server(io_service);
  io_service.run();
}

int main(int argc, const char** argv) {
    std::thread proc_request{Run};
    proc_request.join();
    return 0;
}