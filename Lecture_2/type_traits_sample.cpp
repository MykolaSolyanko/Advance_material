#include <iostream>

// general template
template <typename T, typename U>
struct is_same {
 constexpr static bool value = false;
};

// partial specialization, which says that if we have type T == U
template <typename T>
struct is_same <T, T> {
 constexpr static bool value = true;
};

/*
// full specialization
template <>
struct is_same <bool, bool> {
 enum{ value = true};
};
*/

template <typename T, typename U>
void check_on_same() {
  std::cout << std::boolalpha;
  std::cout << is_same<T, U>::value << std::endl;
}
 
class A{};
class B{};

template<typename T>
struct remove_const {
  typedef T type;
};

// partial specialization, for const type
template<typename T>
struct remove_const<const T> {
  typedef T type; // define a new type only without constancy
};

int main(int argc, char const *argv[])
{
    check_on_same<int, float>(); // print false
    check_on_same<int, int>(); // print true
    check_on_same<A, A>(); // print true
    check_on_same<B, A>(); // print false

    check_on_same<int, const int>(); // print false
    check_on_same<int, remove_const<const int>::type>(); // print true
    return 0;
}
