#include <iostream>
#include <memory>

// full template
template<typename T, typename R>
struct is_same
{
    enum{value = false};
};

// partial template where the same type T is parameterized
template<typename T>
struct is_same<T, T>
{
    enum{value = true};
};
 

class A{

};

class B : public A{

};

class C{

};

/*
type traits manual implementation based on
overloading the return value of the function, and the result is calculated in compile time
*/
template <typename From, typename To>
struct is_convert {
    private:
    typedef char yes;

    typedef struct {char _[2];} no;

    // overloading function.
    /*
      In the compile time, we don’t need an implementation of the function,
      because when you call in the sizeof operator, you use type information.
    */
    static yes check(To);
    static no check(...);

    static From make();
    public:

    // Compile time checking function return type based on function argument
    enum {value = sizeof(check(make())) == sizeof(yes)};
};

int main(int argc, char const *argv[]) {
    std::cout << is_convert<B, C>::value << std::endl; // print 0
    std::cout << is_convert<B, A>::value << std::endl; // print 1
    return 0;
}
