
#include <queue>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <unistd.h>
#include <signal.h>
#include <condition_variable>

bool runnnig{true};
std::condition_variable cv;
std::mutex m;
std::queue<int> q;

void Log(std::string str) {
  std::cout << str << std::endl;
}

void consume(int sig) {
  static size_t value{1};
  Log("add task to queue");
  q.push(value++);
  cv.notify_one();
}

void quit(int sig) {
  Log("quit signal");
  runnnig = false;
}

void produce() {
  while(runnnig) {
    std::unique_lock<std::mutex> l{m};
    cv.wait(l);
    auto back = std::move(q.back());
    q.pop();
    Log(std::string("Data :") + std::to_string(back));
  }  
}

int main(int argc, const char** argv) {
    std::cout << getpid() << std::endl;
    std::thread t{produce};
    signal(SIGUSR1, consume);
    signal(SIGUSR2, quit);
    t.join();
    return 0;
}