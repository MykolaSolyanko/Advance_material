#include <array>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <cstring>
#include <tuple>
#include <numeric>

void work_with_c_string_array() {
  std::array<char, 21> str{'\0'};

//   strcpy(&str[0], "Hello World");

// //   std::cout << &str[0] << std::endl;
   strcpy(str.data(), "Hello World");
   std::cout << str.data() << std::endl;

   std::transform(str.begin(), str.end(), str.begin(), toupper);
   std::cout << str.data() << std::endl;
}

void work_with_array_using_tuple_interface() {
  std::array<int, 3> arr{1, 2, 3};

  std::cout << std::tuple_size<decltype(arr)>::value << std::endl;
  std::cout << std::get<10>(arr) << std::endl;

  auto res = std::accumulate(arr.begin(), arr.end(), 0);
  std::cout << res << std::endl;
}


int main(int argc, const char** argv) {
  std::array<int, 5> arr{1, 2, 3, 4, 5};
  std::copy(arr.begin(), arr.end(), std::ostream_iterator<decltype(arr[0])>(std::cout, " "));

  std::array<int, 0> arr_zero;
  std::copy(arr_zero.begin(), arr_zero.end(), std::ostream_iterator<decltype(arr_zero[0])>(std::cout, " "));


  std::cout << std::endl;
  std::array<int, 5> arr1 = std::move(arr);
  std::copy(arr1.begin(), arr1.end(), std::ostream_iterator<decltype(arr1[0])>(std::cout, " "));
  std::cout << std::endl;
  std::copy(arr.begin(), arr.end(), std::ostream_iterator<decltype(arr[0])>(std::cout, " "));



  std::cout << std::endl;
  std::array<std::string, 5> arr_str {"C++", "Python", "Go", "C", "Java"};
  std::copy(arr_str.begin(), arr_str.end(), std::ostream_iterator<decltype(arr_str[0])>(std::cout, " "));
  std::cout << std::endl;
  std::array<std::string, 5> arr_str_copy;

  std::move(arr_str.begin(), arr_str.end(), arr_str_copy.begin());
  std::copy(arr_str_copy.begin(), arr_str_copy.end(), std::ostream_iterator<decltype(arr_str_copy[0])>(std::cout, " "));
  std::cout << std::endl;
  std::copy(arr_str.begin(), arr_str.end(), std::ostream_iterator<decltype(arr_str[0])>(std::cout, " "));
  std::cout << std::endl;
  work_with_c_string_array();
  work_with_array_using_tuple_interface();
  return 0;
}