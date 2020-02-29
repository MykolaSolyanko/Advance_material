#include <iostream>
#include <exception>
#include <cstring>
#include <type_traits>
#include <vector>
#include <string>

class Vector {
  public:
    // constructor must be invariant see core guidline 
    // see E.5: Let a constructor establish an invariant, and throw if it cannot
    Vector(int size) : size_{size}, capacity_{size} {
      if (size <= 0) {
        throw std::length_error{"Incorect length of vector"};
      }
      data_ = new int [size]; // maybe throw bad_alloc exception
    }

    // WRONG
/*     Vector& operator=(const Vector& rhs) {
      if (this == &rhs) { // check on self assigned
        return *this;
      }
      delete[] data_;
      data_ = new int[rhs.capacity_];
      size_ = rhs.size_;
      capacity_ = rhs.capacity_;
      memcpy(data_, rhs.data_, sizeof(int) * rhs.size_);
    } */

    // CORRECT: invariant
    Vector& operator=(const Vector& rhs) {
      if (this == &rhs) { // check on self assigned
        return *this;
      }
      int* tmp = new int[rhs.capacity_];
      memcpy(data_, rhs.data_, sizeof(int) * rhs.size_);
      size_ = rhs.size_;
      capacity_ = rhs.capacity_;
      delete [] data_;
      data_ = tmp;
      return *this;
    }

   private:
    int* data_{};
    int size_{};
    int capacity_{};
};


class debug {
  public:
    debug(const char* log) {std::cout << log << std::endl;}
    ~debug(){std::cout << __PRETTY_FUNCTION__ << std::endl;}
    void print() {std::cout << __PRETTY_FUNCTION__ << std::endl;}
};

// own class exception MUST be inherit from class std::exception 
class make_f1_f2_exception : public std::exception {
  public:
   make_f1_f2_exception(const char* err) : error_{err} {}
   const char* what() const noexcept {return error_;}
  private:
   const char* error_{};
};

void f2() {
  debug d{__PRETTY_FUNCTION__}; // the destructor is called if an catch block is found
  throw make_f1_f2_exception{"exception when try make f1 and f2 functionality"}; // throw exception use stack unwinding   
}

void f1() {
  try { // used only for logging
    debug d{__PRETTY_FUNCTION__}; // the destructor is called if an catch block is found
    f2();
  } catch (const make_f1_f2_exception& e) { // specific exception
    std::cout << __LINE__ << ":" << e.what() << std::endl;
    // WRONG
    // throw e;
    // CORRECT 
    throw;
  } catch (const std::exception & e) { // generic exception
    std::cout << __LINE__ << ":" << e.what() << std::endl;
    throw;
  }
}


void make_f1_f2() {
  try {
    f1();
  } catch(const make_f1_f2_exception& e) {
    std::cout << __LINE__ << ":" << e.what() << std::endl;
  } catch(const std::exception& e) {
    std::cout << __LINE__ << ":" << e.what() << std::endl;
  } catch(...) {
    std::cout << __LINE__ << std::endl;
    std::terminate();
  }
}

template<typename T>
class HugeCopyClass{
  public:
   HugeCopyClass(T data) : data_{data} {}
   HugeCopyClass(const HugeCopyClass& ) {std::cout << __PRETTY_FUNCTION__ << std::endl;}
   HugeCopyClass(HugeCopyClass&& ) noexcept(std::is_move_constructible<T>::value) {std::cout << __PRETTY_FUNCTION__ << std::endl;}
   HugeCopyClass& operator=(const HugeCopyClass&) {std::cout << __PRETTY_FUNCTION__ << std::endl; return *this;}
   HugeCopyClass& operator=(HugeCopyClass&&) noexcept(std::is_move_assignable<T>::value)  {std::cout << __PRETTY_FUNCTION__ << std::endl; return *this;}
  private:
    T data_;
};

class WillCallExcept {
  public:
   WillCallExcept() = default;
   WillCallExcept(const WillCallExcept& ) {std::cout << __PRETTY_FUNCTION__ << std::endl;}
   WillCallExcept(WillCallExcept&& ) {std::cout << __PRETTY_FUNCTION__ << std::endl;}
   WillCallExcept& operator=(const WillCallExcept&) {std::cout << __PRETTY_FUNCTION__ << std::endl; return *this;}
   WillCallExcept& operator=(WillCallExcept&&)  {std::cout << __PRETTY_FUNCTION__ << std::endl; return *this;}
};

void test_vector() {
  {
    std::vector<HugeCopyClass<std::string>> v; //  HugeCopyClass(HugeCopyClass&& ) noexcept(true), because string move constructor defined how noexcept
    std::string str{"Hello World"};

    // will be called HugeCopyClass move constructor because it is noexcept
    v.emplace_back(str);
    v.emplace_back(str);
    v.emplace_back(str);
    v.emplace_back(str);
    v.emplace_back(str);
    v.emplace_back(str);
  }

  {
    std::vector<HugeCopyClass<WillCallExcept>> v; //  HugeCopyClass(HugeCopyClass&& ) noexcept(false), because WillCallExcept  move constructor defined without noexcept
    WillCallExcept data{};

    // will be called HugeCopyClass copy constructor because it is noexcept(false)
    v.emplace_back(data);
    v.emplace_back(data);
    v.emplace_back(data);
    v.emplace_back(data);
    v.emplace_back(data);
    v.emplace_back(data);
  }
}

int main(int argc, const char** argv) {
    make_f1_f2();
    test_vector();
    try {
        Vector v1{-100};
        Vector v2{100};
        v1 = v2;
    } catch (const std::length_error& e) {
      std::cout << __LINE__ << ":" << e.what() << std::endl;
    } catch (const std::bad_alloc& e) {
      std::cout << __LINE__ << ":" << e.what() << std::endl;
    } catch (const std::exception& e) {
      std::cout << __LINE__ << ":" << e.what() << std::endl;
    }
    return 0;
}