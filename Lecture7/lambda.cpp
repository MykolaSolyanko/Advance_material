#include <iostream>
#include <functional>

using FuncP = long long(*)(int a, int b); // declare function pointer
using Func = std::function<long long(int, int)> // declare function object

struct Callable {
  long long operator()(int a, int b) {return a + b;}
};

void call_function_use_pointer(FuncP f) {
  f(10 , 100); // call function through function pointer
}

void call_function_use_function(Func f) {
  f(300 , 100); // call function through function object
}

void call_function_use_calleble_object(Callable f) {
  f(300 , 100); // call function through Callable object
}

long long some_callable(int a, int b) {
    return a + b;
}
class A {
  public:
   void change_element_array_by_index(size_t index, int value) {array[index] = value;}
   int get_element_array_by_index(size_t index) {return array[index];}
  private:
  int array[1000];
};

int main(int argc, char const *argv[]) {
    call_function_use_pointer(some_callable);
    call_function_use_function(some_callable);
    // pass lambda how function object
    call_function_use_function([](int a, int b){return a + b});

    auto l = [](int a, int b){return a + b};
    call_function_use_pointer(l); // implicit convert function object to function pointer

    call_function_use_calleble_object(Callable{}); // use function object
    A a;
    // capture a on copy
    size_t index{};
    auto l_new = [a, &index](int a, int b) mutable {
        long long res = a + b;
        a.change_element_array_by_index(index++, res); // compiler error without , inside lambda object capture how copy is const
        return res;
    }

    call_function_use_function(l_new); // could allacate memory for function object. Implicit convertfrom function object to std::function calss
    // call_function_use_pointer(l_new); // compiler error. If lambda capture object in capture list, cannot be converted to a function pointer
    return 0;
}
