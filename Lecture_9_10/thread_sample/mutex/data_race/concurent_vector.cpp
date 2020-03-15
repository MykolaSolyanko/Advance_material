
#include <vector>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <unistd.h>
#include <signal.h>

class ConcurrentVector {
  public:
   void add(int value) {
     m.lock();
     data.push_back(value);
     m.unlock();
   }
   void pop_back() {
     m.lock();
     if (!data.empty())
       data.pop_back();
     m.unlock();
   }

   int get_from_back() const {
     std::lock_guard<std::mutex> l(m);
     return data[data.size() - 1];
   }
   bool is_empty() {
     std::lock_guard<std::mutex> l(m);
     return data.empty();
   }

  friend void swap_concurent_vector(ConcurrentVector& vect1, ConcurrentVector& vect2);
  private:
    std::vector<int> data;
    mutable std::mutex m;
};

void swap_concurent_vector(ConcurrentVector& vect1, ConcurrentVector& vect2) {
  vect1.m.lock();
  vect2.m.lock();
  std::swap(vect1.data, vect2.data);
  vect1.m.unlock();
  vect2.m.unlock();
}

ConcurrentVector vect;
bool runnnig{true};
void Log(std::string str) {
  std::cout << str << std::endl;
}


void add_task_to_vector(int sig) {
  static size_t value{1};
  vect.add(value++);
  Log("add to vector");
}

void quit(int sig) {
  Log("quit signal");
  runnnig = false;
}

void ProccesTask() {
  while(runnnig) {
    if (!vect.is_empty()) {
        Log(std::string("Data :") + std::to_string(vect.get_from_back()));
        vect.pop_back();
    }
    std::this_thread::yield();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    
  }  
}

int main(int argc, const char** argv) {
    std::cout << getpid() << std::endl;
    std::thread t{ProccesTask};
    std::thread t1(add_task_to_vector, 100);
    signal(SIGUSR2, quit);
    t.join();
    t1.join();
    ConcurrentVector v1;
    ConcurrentVector v2;
    swap_concurent_vector(v1, v2);
    swap_concurent_vector(v2, v1);
    return 0;
}