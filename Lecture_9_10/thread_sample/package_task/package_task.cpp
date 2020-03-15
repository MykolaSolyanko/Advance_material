#include <future>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <random>
#include <future>
#include <iterator>

namespace {
  const size_t kVectorSize {10}; 
  const size_t kMin{1};
  const size_t kMax{1000};
}

auto generate_vector() {
  std::mt19937 generator;
  std::uniform_int_distribution<int> distribution(kMin, kMax);
  std::vector<int> v;
  v.reserve(kVectorSize);
  std::generate_n(std::back_inserter(v), kVectorSize, [&distribution, &generator] { return distribution(generator); });
  return v;
}

template <typename T>
long long get_block_data(T begin_, T end_) {
  using ValueType = typename T::value_type;
  return std::accumulate(begin_, end_, ValueType{});
}

auto run_parrallel_task() {
  auto thr = std::thread::hardware_concurrency();

  auto v = generate_vector();
  std::vector<std::future<long long>> v_fut;
  v_fut.reserve(thr);
  auto block = kVectorSize / thr;
  auto it = v.begin();
  for (size_t i{0}; i < thr; ++i) {
    std::packaged_task<decltype(get_block_data<decltype(v)::iterator>)> task{get_block_data<decltype(v)::iterator>};
    v_fut.push_back(task.get_future());
    std::thread(std::move(task), it, it + block).detach();
    it += block;
  }

  auto ret_value = get_block_data(it, v.end());

  ret_value = std::accumulate(v_fut.begin(), v_fut.end(), ret_value, [](auto acc, auto& fut) { return acc + fut.get(); });
  // size_t ret_value{};
  // for (auto it = v.begin(); it != v.end(); it++) {
  //   ret_value += *it;
  // }
  return ret_value;
}

int main(int argc, const char** argv) {
    std::cout << run_parrallel_task() << std::endl;
    return 0;
}