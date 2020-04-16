#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <iterator>
#include <random>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <thread>
#include <deque>
#include <type_traits>

class debug {
  public:
   debug(int value1, double value2) {}
   debug(int value1) {}
  //  debug() {std::cout << __PRETTY_FUNCTION__ << std::endl;}
   ~debug() {}
   debug(const debug&) {}
   debug(debug&& ) noexcept {}
   debug& operator =(const debug&) {return *this;}
   debug& operator =(debug&&) {return *this;}
};

void work_with_debug_class() {
  auto start = std::chrono::system_clock::now();
  std::vector<debug> v;
  // v.reserve(10);
  // std::cout << v.capacity() << " "  << v.size() << std::endl;
  for (int i = 0; i < 10'000'000; i++) {
    // v.push_back({(i + 1), (i + 1) + 0.1});
    v.emplace_back(i + 1, (i + 1) + 0.1);
  }
  auto end = std::chrono::system_clock::now() - start;
  auto duration = std::chrono::duration<double>(end).count();


  std::cout << "end - > " << duration << std::endl;
  int value;
  std::cin >> value;
  // std::array<const char*, 5> arr_string {"first", "second", "third", "fourth", "five"};
  // std::vector<std::string> v_str{arr_string.begin(), arr_string.end()};

  // std::copy(v_str.begin(), v_str.end(), std::ostream_iterator<std::string>(std::cout, " "));
}

void work_with_deque_debug_class() {
  auto start = std::chrono::system_clock::now();
  std::deque<debug> v;
  for (int i = 0; i < 10'000'000; i++) {
    // v.push_back({(i + 1), (i + 1) + 0.1});
    v.emplace_back(i + 1, (i + 1) + 0.1);
  }
  auto end = std::chrono::system_clock::now() - start;
  auto duration = std::chrono::duration<double>(end).count();
  std::cout << "end"  << duration << std::endl;
  int value;
  std::cin >> value;
  // std::array<const char*, 5> arr_string {"first", "second", "third", "fourth", "five"};
  // std::vector<std::string> v_str{arr_string.begin(), arr_string.end()};
  // std::copy(v_str.begin(), v_str.end(), std::ostream_iterator<std::string>(std::cout, " "));
}

void fast_delete() {
  std::random_device rd;
  std::mt19937 g(rd());
  std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};
  std::shuffle(v.begin(), v.end(), g);
  std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " : "));
  std::cout << std::endl;
  auto find_iter = std::find(v.begin(), v.end(), 5);
  if (find_iter != v.end()) {
    *find_iter =  std::move(v.back());
    v.pop_back();
  }
  std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " : "));
}

void clear_capacity() {
  {
    std::vector<debug> v;
    v.reserve(10);
    // v.at(0) = {1, 10.5}; // UB

    std::cout << v.size() << " : " << v.capacity() << std::endl;
  }
  {
    std::vector<int> v;
    std::fill_n(std::back_inserter(v), 10, 10);
    std::cout << v.size() << " : " << v.capacity() << std::endl;
    v.clear(); // call destructor
    std::cout << v.size() << " : " << v.capacity() << std::endl;
    v.shrink_to_fit(); // C++11
    // std::vector<int>(v).swap(v); // before C++11
    std::cout << v.size() << " : " << v.capacity() << std::endl;
  }
}

void work_with_c_string() {
  char buff[] {"Hello World"};
  std::vector<char> v{std::begin(buff), std::end(buff)};
  std::cout << v.data() << std::endl;
  std::transform(v.begin(), v.end(), v.begin(), toupper);
  std::cout << v.data() << std::endl;
  strcpy(v.data(), "MyString");
  std::cout << v.data() << std::endl;
//   std::copy(v.begin(), v.end(), std::ostream_iterator<char>(std::cout, " : "));
}

void remove_erase() {
  std::random_device rd;
  std::mt19937 g(rd());
  std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 10, 11};
  std::shuffle(v.begin(), v.end(), g);

  std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " : "));
  std::cout << std::endl;
  std::cout << v.size() << " : " << v.capacity() << std::endl;

  v.erase(std::remove_if(v.begin(), v.end(), [](int value) { return value % 2 == 1;}), v.end());

  std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " : "));
  std::cout << std::endl;
  std::cout << v.size() << " : " << v.capacity() << std::endl;
}


int main(int argc, char const *argv[]) {
  // remove_erase();
  // fast_delete();
  // std::cout << std::is_trivial<MyClass>::value << std::endl;

  std::cout << getpid() << std::endl;
  int value;
  std::cin >> value; 
  work_with_deque_debug_class();
  // work_with_debug_class();

  // std::cin.get();
  // clear_capacity();
  // work_with_debug_class();
  // work_with_deque_debug_class();
  // std::thread([]{
  //   while(true) {
  //     std::this_thread::sleep_for(std::chrono::seconds(1));
  //   }
  // }).join();
  return 0;
}
