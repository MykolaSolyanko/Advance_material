#include <iostream>
#include <algorithm>
#include <list>
#include <vector>
#include <chrono>
#include <iomanip>
#include <random>
#include <functional>
#include <iterator>

template<typename Cont>
void test_container(Cont& c) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  auto begin = std::chrono::system_clock::now();
  std::fill_n(std::back_inserter(c), 100, 300);
  auto end = std::chrono::system_clock::now() - begin;
  std::cout << std::setprecision(15);
  auto res = std::chrono::duration<double>(end).count();
  std::cout << res << std::endl;
}

void slice_list() {
  std::random_device r;
  std::mt19937 rng;
  std::uniform_int_distribution<int> dist(0, 100);

 {
  std::list<int> l1;
  std::generate_n(std::back_inserter(l1), 10, [&dist, &r]() {return dist(r);});
  std::vector<std::reference_wrapper<int>> v1{l1.begin(), l1.end()};
  std::shuffle(v1.begin(), v1.end(), rng);
  std::copy(l1.begin(), l1.end(), std::ostream_iterator<int>(std::cout, " "));

  std::cout << std::endl;
  std::list<int> l2;
  std::generate_n(std::back_inserter(l2), 10, [&dist, &r]() {return dist(r);});
  std::vector<std::reference_wrapper<int>> v2{l2.begin(), l2.end()};
  std::shuffle(v2.begin(), v2.end(), rng);
  std::copy(l2.begin(), l2.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;

//   l1.splice(l1.begin(), l2);

//   std::cout << "l2: " << std::endl;
//   std::copy(l2.begin(), l2.end(), std::ostream_iterator<int>(std::cout, " "));

//   std::cout << "l1: " << std::endl;
//   std::copy(l1.begin(), l1.end(), std::ostream_iterator<int>(std::cout, " "));
//  }

//  {
//      std::list<int> l1 {1 ,2 , 3, 4, 5};
//      std::list<int> l2 {5 , 5, 6, 7, 8};

//      l1.splice(std::find(l1.begin(), l1.end(), 5), l2, std::find(l2.begin(), l2.end(), 5), std::find(l2.begin(), l2.end(), 6));

//     std::cout << "\n l2: " << std::endl;
//     std::copy(l2.begin(), l2.end(), std::ostream_iterator<int>(std::cout, " "));

//     std::cout << "\n l1: " << std::endl;
//     std::copy(l1.begin(), l1.end(), std::ostream_iterator<int>(std::cout, " "));

//     l1.unique();
//     std::cout << "\n l1: " << std::endl;
//     std::copy(l1.begin(), l1.end(), std::ostream_iterator<int>(std::cout, " "));
//  }

 {
    std::list<int> l1 {3 , 8 , 2, 1, 11};
    std::list<int> l2 {9 , 1, 3, 8, 10};
    l1.sort();
    l2.sort();
    l1.merge(l2);
    
    std::cout << "\n l2: " << std::endl;
    std::copy(l2.begin(), l2.end(), std::ostream_iterator<int>(std::cout, " "));

    std::cout << "\n l1: " << std::endl;
    std::copy(l1.begin(), l1.end(), std::ostream_iterator<int>(std::cout, " "));
 }


}

class debug {
  public:
   debug(int value1, double value2) {std::cout << __PRETTY_FUNCTION__ << std::endl;}
   debug(int value1) {std::cout << __PRETTY_FUNCTION__ << std::endl;}
  //  debug() {std::cout << __PRETTY_FUNCTION__ << std::endl;}
   ~debug() {std::cout << __PRETTY_FUNCTION__ << std::endl;}
   debug(const debug&) {std::cout << __PRETTY_FUNCTION__ << std::endl;}
   debug(debug&& ) noexcept {std::cout << __PRETTY_FUNCTION__ << std::endl;}
   debug& operator =(const debug&) {std::cout << __PRETTY_FUNCTION__ << std::endl; return *this;}
   debug& operator =(debug&&) {std::cout << __PRETTY_FUNCTION__ << std::endl; return *this;}
};

void vector_use_spilce_list() {
  std::vector<int> v1{3 , 4 , 1 , 6 , 5 , 7 , 8 , 2};
  std::vector<int> v2{2 , 7 , 3 , 6 , 8 , 4 , 1 , 5};

  auto it_v2 = std::find(v2.begin(), v2.end(), 6);

  if (it_v2 != v2.end()) {
    // auto it_v1 = std::find(v1.begin(), v1.end(), 6);

    // std::vector<int> v_tmp {v1.begin(), it_v1};
    // std::cout << "v_tmp = " << std::endl;
    // std::copy(v_tmp.begin(), v_tmp.end(), std::ostream_iterator<int>(std::cout, " : "));
    // std::cout << std::endl;

    // std::copy(it_v2, it_v2 + 3, std::back_inserter(v_tmp));
    // std::cout << "v_tmp = " << std::endl;
    // std::copy(v_tmp.begin(), v_tmp.end(), std::ostream_iterator<int>(std::cout, " : "));
    // std::cout << std::endl;

    // std::copy(it_v1, v1.end(), std::back_inserter(v_tmp));
    // std::cout << "v_tmp = " << std::endl;
    // std::copy(v_tmp.begin(), v_tmp.end(), std::ostream_iterator<int>(std::cout, " : "));
    // std::cout << std::endl;
    // v_tmp.swap(v1);


    std::list<std::reference_wrapper<int>> v_tmp {v1.begin(), v1.end()};
    std::list<std::reference_wrapper<int>> v_tmp2 {it_v2, it_v2 + 3};

    auto it_v1 = std::find(v_tmp.begin(), v_tmp.end(), 6);
    v_tmp.splice(it_v1, v_tmp2);
    v1.assign(v_tmp.begin(), v_tmp.end());

    std::cout << "v_tmp = " << std::endl;
    std::copy(v1.begin(), v1.end(), std::ostream_iterator<int>(std::cout, " : "));
    std::cout << std::endl;
  }
}

int main(int argc, char const *argv[]) {
    std::list<int> l;
    test_container(l);

    std::vector<int> v;
    test_container(v);

    // slice_list();
    // vector_use_spilce_list();

    return 0;
}
