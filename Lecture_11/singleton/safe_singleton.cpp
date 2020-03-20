#include <thread>
#include <mutex>
#include <iostream>

// std::once_flag flag1;

class Singleton {
  public:
   static Singleton& getInstance() {
    //  if (instance_) {
    //     return instance_;
    //  }

    //  instance_ = new Singleton{};
    // std::call_once(flag1, Singleton::init_instance);
    static Singleton instance_;

    return instance_;
   }
  private:
    // static void init_instance() {
    //   std::cout << "call create instance" << std::endl; 
    //   instance_ = new Singleton{};
    // }

    Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    // static Singleton* instance_;
};

// Singleton* Singleton::instance_ {nullptr};

void first_thread() {
  auto& instance = Singleton::getInstance();
}

void second_thread() {
  auto& instance = Singleton::getInstance();
}

int main(int argc, char const *argv[]) {
    std::thread t1{first_thread};
    std::thread t2{second_thread};
    t1.join();
    t2.join();
    return 0;
}
