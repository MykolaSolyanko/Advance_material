
#include <iostream>
#include <utility> // for pair
#include <numeric> // iota
#include <tuple>

static constexpr size_t kContainerSize{6}; 
class container{
  public:
    int* begin() {return data_;}    
    int* end() {return data_ + kContainerSize;}
    auto find(int* first, int* last, int value) { // find only one element
      while (first != last) {
        if (*first == value) {
          return first;
        }
        ++first;
      }
      return last;
    }
    auto get_range(int* first, int* last, int value) { // return pair finded value in range[first, last]
      auto first_range = find(first, last, value);
      if (first_range == last) {
        return std::pair<int*, int*>{nullptr, nullptr};
      }
      auto last_range = find(first_range + 1, last, value);
      return std::make_pair(first_range, last_range);
    }    
  private:
   int* data_ = new int[5]{kContainerSize};
};

using UserRequest = std::tuple<int, double, std::string>;

UserRequest makeUserRequest() {
  return {1, 10, "Hello World"};
}

int main(int argc, const char** argv) {
    container c;
    std::iota(c.begin(), c.begin() + 3, 1); // fill container with values 1, 2, 3

    std::iota(c.begin() + 3, c.end(), 2); // fill container with values 2, 3, 4

    for(auto var : c) { // print caontainer data onto display
      std::cout << var << ' '; 
    }
    std::cout << std::endl;

    auto[first, last] = c.get_range(c.begin(), c.end(), 2); // find pair pointers where occur value 2 

    while(first != last) {
        std::cout << *first++ << " ";
    }
    std::cout << std::endl;

    // tuple
    auto [i, d, str] = makeUserRequest();
    std::cout << i << " " << d << " " << str <<  std::endl;
    UserRequest data = makeUserRequest();
    std::cout << std::get<0>(data) << " " << std::get<1>(data) << " " << std::get<2>(data) <<  std::endl;

    int i_{};
    double d_{};
    std::tie(i_, d_, std::ignore) = makeUserRequest();
    std::cout << i_ << " " << d_ <<  std::endl;

    // example of how to print a tuple of any length on the screen
    // https://en.cppreference.com/w/cpp/utility/integer_sequence
    return 0;
}