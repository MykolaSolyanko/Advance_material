#include <string>
#include <iostream>


// using derictive means exporting all names from the std namespace
// this is not very good as it leads to the concept of name collisions
// using namespace std;

// using declaration makes the symbol string from the namespace std accessible
// don't do it in header files
using std::string;

namespace my {
  class string {
    public:
     void print_somthing() {
         std::cout << __PRETTY_FUNCTION__ << std::endl;
     }
  };
};

// namespace for support version
namespace library {
  namespace v_1 {
   class debug {
     public:
       bool debug_info() {std::cout << __PRETTY_FUNCTION__ << std::endl;}
   };
  }
  // this namespace will become part of the library namespace
  inline namespace v_1_2 {
   class debug {
     public:
       bool debug_info() {std::cout << __PRETTY_FUNCTION__ << std::endl;}
   };
  }
}

// start from c++17
namespace a::b::c {
  void func_a_b_c() {std::cout << __PRETTY_FUNCTION__ << std::endl;}
}
/* before c++17
namespace a {
  namespace b {
    namespace c {
     void func_a_b_c() {std::cout << __PRETTY_FUNCTION__ << std::endl;}
    }
  }
}
*/

// namespaces can be combined by code
namespace a::b {
  void func_a_b() {std::cout << __PRETTY_FUNCTION__ << std::endl;}
}

// anonymous namespace, implicit uses using derective
// it's better to use this type of namespace instead of static or const variable
namespace {
  // makes this variable with internal linkage
  int value {10};
}

// below example of ADL 
void use (int value) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}

template<typename T>
void call(T data) {
  use(data); // call function use from ADL namespace 
//   ::use(data); // call function use from global namespace 
}

namespace ADL {
  class test {
    public:
      operator int() {
        return 10;
      }
  };
  void use(test t) {std::cout << __PRETTY_FUNCTION__ << std::endl;}
}

int main(int argc, char const *argv[]) {
  string str; // string from std namespace
  my::string str1; // string from my namespace
  // using my::string;
  // compiler error it is not known from what namespace to use string
  // string str2;

  // imlicit library::v_1_2::debug
  library::debug d_v1_2;
  library::v_1::debug d_v1;
  a::b::c::func_a_b_c();
  a::b::func_a_b();

  call(ADL::test{});
  return 0;
}
