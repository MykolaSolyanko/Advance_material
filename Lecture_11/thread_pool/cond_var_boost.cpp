
#include "thread_pool.hpp"

#include <boost/asio.hpp>
#include "boost_tcp_server.hpp"

using boost::asio::ip::tcp;

ThreadPool poll_;

void add_to_queue(std::string& str) {
  poll_.add_task([str](){std::cout << "process the next request : " << str;});
}

void Run() {
  boost::asio::io_service io_service;
  tcp_server server(io_service);
  io_service.run();
}

int main(int argc, const char** argv) {
    std::cout << getpid() << std::endl;
    std::thread proc_request{Run};
    proc_request.join();
    return 0;
}