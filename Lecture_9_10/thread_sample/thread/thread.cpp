#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <mutex>

namespace {
const size_t kRun {100}; // const variable is thread safe
std::mutex m;
}

void run_parallel_task() {
  for (size_t i = 1; i <= kRun; ++i) {
    m.lock(); // mutex is used for synchronous access to the cout object
    std::cout << "Run thread id: " << std::this_thread::get_id() << " " << i << " iteration\n";
    m.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));  
  }
}

int main(int argc, char const *argv[]) {
  auto count_par_run_thread = std::thread::hardware_concurrency(); // Returns the number of concurrent threads supported by the implementation
  std::vector<std::thread> v;
  v.resize(count_par_run_thread);
  std::generate_n(v.begin(), count_par_run_thread, []{return std::thread(run_parallel_task);}); // generate count_par_run_thread task
  std::cout << v.size() << std::endl;
  // without join or detach program will terminate in thread object destructor
  std::for_each(v.begin(), v.end(), std::mem_fn(&std::thread::join)); // block until thread is finished
  return 0;
}
