#include <thread>
#include <iostream>
#include <signal.h>

namespace {
  bool is_wait{true};
}

bool get_sync_connection() {
  return true; // implement real connection from user with timeout
}

void wait_connection_from_client() {
  while (true) {
    get_sync_connection();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  
}

void run_and_detach_thread() {
  std::thread t(wait_connection_from_client);
  t.detach();
}

void exit_(int sig) {
  std::cout << "iterrupt from user" << std::endl;
  is_wait = false;
}

int main(int argc, const char** argv) {

    run_and_detach_thread();
    signal(SIGUSR1, exit_);
    while (is_wait) { ; }   
    return 0;
}