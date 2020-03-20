#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <thread>
#include <signal.h> 
#include <mutex>

bool is_running {true};
std::vector<int> data{};
std::mutex m;

void Log(std::string str) {
  std::cout << str << std::endl;
}

void GetInputFromUser(int sig) {
  Log("signal handler");
  is_running = false;
}

void print() {
  while(is_running) {
     std::unique_lock<std::mutex> l(m);
     l.lock();
     std::copy(data.begin(), data.end(), std::ostream_iterator<decltype(*data.begin())>(std::cout, " "));
     l.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
  }
}

void add() {
  static int value{};
  while(is_running) {
    std::lock_guard<std::mutex> l(m);
    data.push_back(++value);
    Log("Add");
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
  }
}

void del() {
  while(is_running) {
    std::lock_guard<std::mutex> l(m);
    if (!data.empty()) {
      data.pop_back();
      Log("Remove");
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
  }
}


std::vector<std::string> vec;
std::recursive_mutex m_rec;
void recursion(int deps) {
  if (deps <= 0) {
    return;
  }
  std::unique_lock<std::recursive_mutex> l{m_rec};
  std::cout << vec.back() << std::endl;
  vec.pop_back();
  recursion(--deps);
}

int main(int argc, const char** argv) {
  vec = {"1", "2", "3", "4", "5"};
  std::thread t1{recursion, vec.size()};
  t1.join();
  // std::cout << getpid() << std::endl;
  // std::thread t1{print};
  // std::thread t2{add};
  // std::thread t3{del};
  // signal(SIGUSR1, GetInputFromUser);
  // t1.join();
  // t2.join();
  // t3.join();
  return 0;
}