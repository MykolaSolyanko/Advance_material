
#include <cstring>
#include <iostream>
#include <type_traits>
#include <string>

template<typename T>
class Vector {
  public:
    // this constructor allocates memory using the new operator, which will call the default constructor for custom types
    Vector(size_t count) :  capacity_{count}, data_{new T[count]{}}, count_{count}
    {
        std::cout << __PRETTY_FUNCTION__ <<std::endl;
    }
    // this constructor allocates memory using the C operator malloc function,
    // which will allocate raw memory and not call the constructor for custom types
    Vector() : data_{static_cast<T*>(malloc(capacity_ * sizeof(T)))} {
        std::cout << __PRETTY_FUNCTION__ <<std::endl;
    }

    Vector(const Vector& rhs) : data_{new T[rhs.count_]{}}, capacity_{rhs.capacity_}, count_{rhs.count_}
    {std::cout << __PRETTY_FUNCTION__ <<std::endl;}

    // move constructor should leave the movable object in a valid indefinite state
    Vector(Vector&& rhs) : capacity_{rhs.capacity_}, data_{rhs.data_}, count_{rhs.count_}
    {
        std::cout << __PRETTY_FUNCTION__ <<std::endl;
        rhs.data_ = nullptr;
        rhs.capacity_ = 0;
        rhs.count_ = 0;
    }

    // this method uses optimization based on the type_traits of the instantiated type
    // but the compiler processes all versions of branches(if is a runtime operator)
    void copy(const Vector& cp) {
      if (std::is_class<T>::value) {
        std::cout << "copies approach" << std::endl;
        auto begin_c = cp.data_;
        auto end_c = cp.data_ + cp.count_;
        auto begin_ = data_;
        while(begin_c != end_c) {
          *begin_++ = *begin_c++;
        }
      } else { // optimization branch 
        std::cout << "build in type" << std::endl;
        memcpy(data_, cp.data_, sizeof(T) * cp.count_);
      }
      count_ = cp.count_;
    }

    Vector& operator=(Vector&& rhs) { // move operator should leave the movable object in a valid indefinite state
        std::cout << __PRETTY_FUNCTION__ <<std::endl;
        data_ = rhs.data_;
        capacity_ = rhs.capacity_;
        count_ = rhs.count_;
        rhs.data_ = nullptr;
        rhs.capacity_ = 0;
        rhs.count_ = 0;   
        return *this;
    }
    Vector& operator=(const Vector& rhs) { // copy operator
        std::cout << __PRETTY_FUNCTION__ <<std::endl;
        if (this == &rhs) {return *this;}
        count_ = rhs.count_;
        capacity_ = rhs.capacity_;
        data_ = new T[capacity_];
       
      return *this;
    }
    void push_back(const T& value) { // will be called when passing lvalue
     std::cout << __PRETTY_FUNCTION__ << count_ <<std::endl;
      data_[count_++] = value;
    }

    void push_back(T&& value) { // will be called when passing rvalue
      std::cout << __PRETTY_FUNCTION__ << count_ <<std::endl;
      // if you do not specify std :: move,
      // then the assignment operator will be called instead of the moving operator
      data_[count_++] = std::move(value);
    }

    /*
      this is a method for optimization,
      i.e. instead of calling copy or moving when push_back is called,
      we take the in-place optimization approach.
    */
    template<typename ...Args> // this is a form starting with c ++ 11 called Variadic templates
    void emplace_back(const Args&... args) {
      new (&data_[count_++]) T{args...}; // using placement new
    }

    ~Vector() {
        delete[] data_;
    }
    /*
      these methods are needed to iterate over the elements of our class through range for
    */
    T* begin() {return data_;}
    T* end() {return data_ + count_;}

  private:
    size_t capacity_{4};
    T* data_{};
    size_t count_{};
};

/*
using this function based on type_traits will not work since the if statement is a run-time operator,
and the compiler will process all branches of the if statement.
for example
auto str = convert(10); //error can not convert int to string, erroneous branch checks if (std::is_same<T, std::string>::value)
*/
// template<typename T>
// std::string convert(T value) {
//   if (std::is_same<T, std::string>::value) {
//     return value; // if T=string, then just return value, because we do not need a conversion
//   } else if (std::is_integral<T>::value) {
//     return std::to_string(value); // if T for example int using a standard function to convert a number to a string
//   }
//   return std::string(value); // for example T == const char*
// }

/*
the convert function uses the SFINAE approach,
i.e. instantiation error is not a compilation error
this used before C++17
*/
// template<typename T>
// typename std::enable_if<std::is_integral<T>::value, std::string>::type convert(T value) {
//   std::cout << "function\n";
//     return std::to_string(value);
//   // } else if (std::is_same<T, std::string>::value) {
//   //   return value;
//   // }
// }

// template<typename T>
// typename std::enable_if<std::is_same<T, std::string>::value, std::string>::type convert(T value) {
//   std::cout << "function2\n";
//     return value;
//   // } else if (std::is_same<T, std::string>::value) {
//   //   return value;
//   // }
// }

/*
 the same as above only uses a more compact form,
 available starting with C ++ 17
*/
template<typename T>
std::string convert(T value) {
  if constexpr (std::is_integral<T>::value) {
    return std::to_string(value);
  } else if constexpr (std::is_same<T, std::string>::value) {
    return value;
  }
}

/*
if you explicitly define a destructor or one of the copying semantics, then move semant will not generate, and on the contrary,
if you define one of the move semantics operators, then copying functionality will not be generated
*/
class A {
  public:
    A() = default;
    A(int size) { std::cout << "default\n"; }
    A(int size, double d) { std::cout << "another\n"; }
    A(const A&) { std::cout << "copy\n"; }
    A(A&&) { std::cout << "move\n"; }
    A operator =(const A&) { std::cout << "oper copy\n"; return *this; }
    A operator =(A&&) { std::cout << "oper move\n"; return *this;}
};

int main(int argc, char const *argv[]) {
    Vector<A> v1{5};
    v1.push_back(A{});
    v1.emplace_back(10);
    v1.emplace_back(10, 123.5);
    return 0;
}
