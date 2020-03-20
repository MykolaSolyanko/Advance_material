#include <iostream>
#include <thread>
#include <assert.h>
#include <atomic>

size_t data;
std::atomic<bool> data_ready {false};

void procc_data(size_t data_) {
  std::cout << data_ << std::endl; 
}

void set_data() {
  data_ready = true;
  data = 42;
}

void wait_data() {
  while (!data_ready) ;
  assert(data == 42);
  procc_data(data);  
}

int main(int argc, char const *argv[]) {
  std::thread t1{wait_data};
  std::thread t2{set_data};
  t1.join();
  t2.join();
  return 0;
}
