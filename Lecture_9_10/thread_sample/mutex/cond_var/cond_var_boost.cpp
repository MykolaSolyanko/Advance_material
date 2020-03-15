
#include <queue>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <unistd.h>
#include <signal.h>
#include <condition_variable>

#include <boost/asio.hpp>
#include "boost_tcp_server.hpp"

using boost::asio::ip::tcp;

bool runnnig{true};
std::condition_variable cv;
std::mutex m;
std::queue<std::string> q;

void Log(std::string str) {
  std::cout << str << std::endl;
}

void add_to_queue(std::string& str) {
  q.push(str);
  cv.notify_one();
}

void consume() {
  boost::asio::io_service io_service;
  tcp_server server(io_service);
  io_service.run();
}


void produce() {
  while(runnnig) {
    std::unique_lock<std::mutex> l{m};
    cv.wait(l, [&q]() {return q.empty();});
    auto back = std::move(q.back());
    q.pop();
    Log(std::string("Data :") + back);
  }  
}

int main(int argc, const char** argv) {
    std::cout << getpid() << std::endl;
    std::thread t1{produce};
    std::thread t2{consume};
    t1.join();
    t2.join();
    return 0;
}