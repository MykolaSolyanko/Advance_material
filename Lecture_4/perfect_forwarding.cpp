#include <cstring>
#include <iostream>
#include <type_traits>
#include <string>
#include <memory>
#include <cstdio>

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
    Vector() : data_{new T[capacity_]{}} {
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

    auto insert(size_t pos, T value) {
      return std::pair{begin(), true};
    }
    /*
      this is a method for optimization,
      i.e. instead of calling copy or moving when push_back is called,
      we take the in-place optimization approach.
    */
    template<typename ...Args> // this is a form starting with c ++ 11 called Variadic templates
    void emplace_back(Args&&... args) { // this is perfect forwarding use references collapsing rules
      new (&data_[count_++]) T{std::forward<Args...>(args...)}; // using placement new
    }

    ~Vector() {
        delete [] data_;
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

class String {
  public:
    String() = default;
    String(int size) { std::cout << __PRETTY_FUNCTION__ << std::endl; }
    String(const char* str) { std::cout << __PRETTY_FUNCTION__ << std::endl; }
    String(int count, char ch) { std::cout << __PRETTY_FUNCTION__ << std::endl; }
    String(const String&) { std::cout << __PRETTY_FUNCTION__ << std::endl; }
    String(String&&) { std::cout << __PRETTY_FUNCTION__ << std::endl; }
    String operator =(const String&) { std::cout << __PRETTY_FUNCTION__ << std::endl; return *this; }
    String operator =(String&&) { std::cout << __PRETTY_FUNCTION__ << std::endl; return *this;}
};

class B {
  public:
    B() = default;
    B(const String& ref) : str{ref} {std::cout << __PRETTY_FUNCTION__ << std::endl;}
    B(String&& ref) : str{std::move(ref)} {std::cout << __PRETTY_FUNCTION__ << std::endl;}
    B(const B&) { std::cout << __PRETTY_FUNCTION__ << std::endl; }
    B(B&&) { std::cout << __PRETTY_FUNCTION__ << std::endl; }
    B& operator =(const B&) { std::cout << __PRETTY_FUNCTION__ << std::endl; return *this; }
    B& operator =(B&&) { std::cout << __PRETTY_FUNCTION__ << std::endl; return *this;}
    ~B() {std::cout << __PRETTY_FUNCTION__ << std::endl;}
  private:
    String str;
};

int main(int argc, const char** argv) {
    Vector<B> v;

    v.emplace_back(String{"Hello Wold"}); // using rvalue references(call String(String&&))
    String s{"Test"};
    v.emplace_back(s); // using lvalue references (call String(const String&))
    return 0;
}