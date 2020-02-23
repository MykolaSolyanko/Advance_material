#include <iostream>

// before C++14
// last instantiated function
template<typename T>
void Log(T value) {
  std::cout << "End" << std::endl;
}

template<typename T, typename ...Args>
void Log(T head, Args ...args) { // pack values syntactic
  std::cout << "Value is " << head << std::endl;
  Log(args...); // unpack values syntactic
}


// after C++14 Fold Expressions
template <typename ...Args>
void LogFold(Args ...args) {
  (std::cout << ... << args); // but it’s hard to add something between the cout and args
}


// this fold expresion calc summ
template <typename ...Args>
constexpr auto  summ(Args ...args) {
  // return (0 + ... + args); // with init value
  return (args + ...); // without init value
}

struct Point{
  int x{};
  int y{};
};

std::ostream& operator <<(std::ostream& out, const Point& p) {
  out << "Value is " <<  p.x << " " << p.y << std::endl;
  return out; 
};

class A {
  public:
    A() {std::cout << __PRETTY_FUNCTION__ << std::endl;}
};

class B {
  public:
    B() {std::cout << __PRETTY_FUNCTION__ << std::endl;}
};

class C {
  public:
    C() {std::cout << __PRETTY_FUNCTION__ << std::endl;}
};

class D {
  public:
    D() {std::cout << __PRETTY_FUNCTION__ << std::endl;}
};

template <typename ...base>
class E : public base...{
  public:
    E() {std::cout << __PRETTY_FUNCTION__ << std::endl;}
};

int main(int argc, const char** argv) {
    Log(1, 1.23, "Hello World", Point{10, 20}); // output formatted text

    LogFold(1, 1.23, "Hello World", Point{10, 20}); // output in one line

    std::cout << summ(10, 1.23, 2.45) << std::endl;

    E<A, B, C, D> e; // E will be inherited from A() B() C() D()
    return 0;
}