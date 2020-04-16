
#include <iterator>
#include <forward_list>
#include <algorithm>
#include <iostream>
#include <list>
#include <vector>

namespace my_algo {
template<typename InputIt, typename Distance>
void advance(InputIt& begin, Distance n, std::forward_iterator_tag) {
//   static_assert(false, "advance algorithm support at least bidirection type of iterator");
  std::cout << "forward\n";
  while (n--) {
    ++begin;
  }
}

template<typename InputIt, typename Distance>
void advance(InputIt& begin, Distance n, std::bidirectional_iterator_tag) {
  std::cout << "bidirection\n";
  if (n > 0) {
    while (n--) {
      ++begin;
    }
    return;
  }
  while (n++) {
    --begin;
  }
}

template<typename InputIt, typename Distance>
void advance(InputIt& begin, Distance n, std::random_access_iterator_tag) {
  std::cout << "random\n";
  begin += n;
}

template<typename InputIt, typename Distance>
void advance(InputIt& begin, Distance n) {
  advance(begin, n, typename std::iterator_traits<InputIt>::iterator_category());
}
}

int main(int argc, char const *argv[]) {
  {
    std::forward_list<int> l{1, 2, 3};
    auto it = l.begin();
    my_algo::advance(it, 2);

    std::cout << *it << std::endl;
  }
  {
    std::list<int> l{1, 2, 3};
    auto it = l.begin();
    my_algo::advance(it, 1);

    std::cout << *it << std::endl;
  }

  {
    std::vector<int> l{1, 2, 3};
    auto it = l.begin();
    my_algo::advance(it, 2);

    std::cout << *it << std::endl;
  }

  return 0;
}
