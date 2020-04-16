
#include <iostream>
#include <set>
#include <algorithm>
#include <iterator>

class Cmp {
public:
  enum cmp_mode {normal, reverse};
  Cmp (cmp_mode m=normal) : mode(m) {
  }
  template <typename T>
  bool operator() (const T& t1, const T& t2) const {
    // std::cout << "cmp -> " << t1 << " " << t2 << std::endl;
    return mode==normal ? t1<t2
                        : t2<t1;
  }
  bool operator== (const Cmp& rc) const {
    return mode == rc.mode;
  }
private:
  cmp_mode mode;
};

void with_user_defined_cmp() {
  std::set<int, Cmp> s{4, 7, 5, 1, 6, 2, 5};
  std::copy(s.begin(), s.end(), std::ostream_iterator<int>(std::cout, ":"));
  std::cout << std::endl;

  std::set<int, Cmp> s2({4, 7, 5, 1, 6, 2, 5}, Cmp(Cmp::cmp_mode::reverse));
  std::copy(s2.begin(), s2.end(), std::ostream_iterator<int>(std::cout, ":"));
  std::cout << std::endl;

  if (s.key_comp() == s2.key_comp()) {
    std::cout << "key comparator is the same\n"; 
  } else {
    std::cout << "key comparator not the same\n"; 
  }

  s = s2;
  std::copy(s.begin(), s.end(), std::ostream_iterator<int>(std::cout, ":"));
  std::cout << std::endl;


  if (s.key_comp() == s2.key_comp()) {
    std::cout << "key comparator is the same\n"; 
  }
  
  auto key = s.extract(5);
  key.value() = 100;
  s.insert(std::move(key));
  
  // auto pos = s.find(5);

  // if (pos != s.end()) {
  //   auto value = *pos;
  //   value = 100;
  //   s.erase(pos);
  //   s.emplace(value);
  // }

  std::copy(s.begin(), s.end(), std::ostream_iterator<int>(std::cout, ":"));
  std::cout << std::endl;
}

class data {
  public:
    constexpr data(int value) noexcept : payload{value} {}
    void set_senssative_data(int new_sen_data) {user_sensative_info = new_sen_data;}
    int get_payload() const {return payload;}
  private:
    int payload;
    int user_sensative_info{0};
};

void set_for_user_data() {
  auto comp = [](const data& lhs, const data& rhs) {return lhs.get_payload() < rhs.get_payload();};
  std::set<data, decltype(comp)> s{comp};

  s.emplace(10);
  s.emplace(20);
  s.emplace(1);
  s.emplace(15);

  std::cout << "print set with payload: \n";
  std::for_each(s.begin(), s.end(), [](const data& d) {
    std::cout << d.get_payload() << " : ";
  });

  data find_data{20};
  auto finded_payload = s.find(find_data);
  if (finded_payload != s.end()) {
    // finded_payload->set_senssative_data(10); // compiler error
    auto key = s.extract(finded_payload);
    key.value().set_senssative_data(10);
    s.insert(std::move(key));
  }

  std::cout << "\nprint set with payload: \n";
  std::for_each(s.begin(), s.end(), [](const data& d) {
    std::cout << d.get_payload() << " : ";
  });

}

void with_set() {
  std::set<int> s; //
  s.emplace(10);
  s.insert(20);
  s.insert(5);
  s.insert(6);
  // if (auto [it, succ] = s.insert(5); !succ) {
  //   std::cout << "fail" << std::endl;
  // }

  if (s.find(5) != s.end()) {
     std::cout << "fail" << std::endl;
  }  

  auto it_l = s.lower_bound(4);
  auto it_u = s.upper_bound(4);

  std::cout << *it_l  << " " << *it_u  << std::endl;

  std::copy(s.begin(), s.end(), std::ostream_iterator<int>(std::cout, " : "));

  
}
int main(int argc, char const *argv[]) {
  // with_set();
    with_user_defined_cmp();
    // set_for_user_data();



    return 0;
}
