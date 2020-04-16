#include <iostream>
#include <unordered_set>

#include <iterator>
#include <type_traits>
#include <algorithm>

void print_hashtable_data(const std::unordered_multiset<int>& cont) {
  std::cout << "common becket info\n";
  std::cout << "size:" << cont.size() << "\n";
  std::cout << "buckets:" << cont.bucket_count() << "\n";
  std::cout << "load factor:" << cont.load_factor() << "\n";
  std::cout << "max load factor:" << cont.max_load_factor() << "\n";

  // iterator category:
  if (typeid(std::iterator_traits<std::unordered_multiset<int>::iterator>::iterator_category)
          == typeid(std::bidirectional_iterator_tag)) {
    std::cout << "chaining style: doubly-linked" << "\n";
  }
  else {
    std::cout << "chaining style: singly-linked" << "\n";
  }

  // elements per bucket:
  std::cout << "data: " << "\n";
  for (auto idx{0}; idx != cont.bucket_count(); ++idx) {
    std::cout << " b["<< idx << "]: ";
    for (auto pos{cont.begin(idx)}; pos != cont.end(idx); ++pos) {
      std::cout << *pos << " ";
    }
    std::cout << "\n";
  }
  std::cout << std::endl;
}


void using_bucket_interface() {
  {
    std::unordered_multiset<int> s{1, 2, 3, 4, 20, 6, 35};
    s.max_load_factor(0.7);
    print_hashtable_data(s);

    s.insert({33, 19, -17});
    print_hashtable_data(s);
  }
}

void rehash_functionality() {
  std::unordered_set<int> s{1, 2, 3, 4, 20, 6, 35};
  std::copy(s.begin(), s.end(), std::ostream_iterator<int>(std::cout, " : "));     
  std::cout << std::endl;
  s.erase(4);
  std::copy(s.begin(), s.end(), std::ostream_iterator<int>(std::cout, " : "));     
  std::cout << std::endl;
  s.erase(35);
  std::copy(s.begin(), s.end(), std::ostream_iterator<int>(std::cout, " : "));     
  std::cout << std::endl;
  s.insert(33);
  std::copy(s.begin(), s.end(), std::ostream_iterator<int>(std::cout, " : "));     
  std::cout << std::endl;
}

int main(int argc, char const *argv[]) {
  rehash_functionality();
  using_bucket_interface();
  return 0;
}
