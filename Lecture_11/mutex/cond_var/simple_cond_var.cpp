#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>

std::mutex mutex_;
std::condition_variable cv;

bool when_data_ready{false};

void some_work_with_data() {
  std::cout << __PRETTY_FUNCTION__ << std::endl; 
}

void wait_data_ready() {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  std::unique_lock<std::mutex> l{mutex_};
  cv.wait(l, []() { return when_data_ready; });
  some_work_with_data();
}

void notify_when_data_ready() {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  when_data_ready = true;
  cv.notify_one();
}

int main(int argc, char const *argv[]) {


    t1.join();
    t2.join();
    return 0;
}
