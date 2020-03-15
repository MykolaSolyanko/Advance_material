#include <future>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <random>

namespace {
  const size_t kVectorSize {100'000}; 
  const size_t kMin{1};
  const size_t kMax{1000};
}

auto generate_vector() {
  std::random_device device;
  std::mt19937 generator;
  std::uniform_int_distribution<int> distribution(kMin, kMax);
  std::vector<int> v;
  v.reserve(kVectorSize);
  std::generate_n(std::back_inserter(v), kVectorSize, [&distribution, &generator] { return distribution(generator); });
  return v;
}

template <typename T>
long long get_block_data(T begin, T end) {
  using ValueType = typename T::value_type;
  return std::accumulate(begin, end, ValueType{});
}

auto run_parrallel_task() {
  auto thr = std::thread::hardware_concurrency();

  auto v = generate_vector();
  std::vector<std::future<long long>> v_fut;
  v_fut.reserve(thr);
  auto block = thr / kVectorSize;
  
  auto it = v.begin();
  for (size_t i{0}; i < thr; ++i) {
    v_fut.push_back(std::async(std::launch::async, get_block_data<decltype(v)::iterator>, it, it + block));
    it += block;
  }

  auto ret_value = get_block_data(it, v.end());
  ret_value = std::accumulate(v_fut.begin(), v_fut.end(), ret_value, [](auto acc, auto& fut) { return acc + fut.get(); });
//   size_t ret_value{};
//   for (auto it = v.begin(); it != v.end(); it++) {
//     ret_value += *it;
//   }
  return ret_value;
}

int main(int argc, const char** argv) {
    std::cout << run_parrallel_task() << std::endl;
    return 0;
}