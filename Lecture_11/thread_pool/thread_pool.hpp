#pragma once
#include "queue.hpp"
#include <functional>
#include <thread>
#include <vector>
#include <iterator>
#include <iostream>

class ThreadPool {
 public:
  using Func = std::function<void()>;
  ThreadPool() {
    auto thread_count = std::thread::hardware_concurrency();
    thread_.reserve(thread_count);
    std::generate_n(std::back_inserter(thread_), thread_count, [this](){
        return std::thread(&ThreadPool::process_task, this);
    });
  }
  ~ThreadPool() { done_ = true; };
  template<typename F>
  void add_task(F f) {
    safe_queue_.push(static_cast<Func>(f));

  }
 private:
  // method
  void process_task() {
    while (!done_) {
      Func task;
      if (safe_queue_.try_pop(task)) {
         std::cout << std::this_thread::get_id() << std::endl;
         task();
      } else {
        std::this_thread::yield();
      }
    }
  }
  // data
  bool done_{false};
  ConcurentQueue<Func> safe_queue_;
  std::vector<std::thread> thread_;  
};