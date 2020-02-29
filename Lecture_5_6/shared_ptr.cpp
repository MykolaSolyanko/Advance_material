#include <iostream>
#include <memory>
#include <cstdio>
#include <functional>

void work_with_file() {
  const char hw[]{"Hello World"};
// wrong case
//   FILE* file = fopen("some_file", "w");
//   if (file != nullptr) {
//     return;
//   }
//   if (fwrite(hw, sizeof(char), std::size(hw), file) != std::size(hw)) { // std::size from STL library
//     return; // potential resource leak
//   }
//   fclose(file);

// better use case  
  std::shared_ptr<FILE > file(fopen("some_file", "w"), fclose); // function close will be called in the destructor unique_ptr
  // or using lambda
/*    std::shared_ptr<FILE> file(fopen("some_file", "w"), [](FILE* f) {
      std::cout << "clear resource" << std::endl;
      if (f != nullptr) {
        return -1;
      }
      return fclose(f);
  });  */
  if (fwrite(hw, sizeof(char), std::size(hw), file.get()) != std::size(hw)) {
    std::cout << "unable to write data to file" << std::endl;
  }
}


class debug {
  public:
    debug(){std::cout << __PRETTY_FUNCTION__ << std::endl;}
    ~debug(){std::cout << __PRETTY_FUNCTION__ << std::endl;}
    void print() {std::cout << __PRETTY_FUNCTION__ << std::endl;}
};

// overload default operator new
void* operator new (size_t size) {
  std::cout << __PRETTY_FUNCTION__ << std::endl; 
  return malloc(size);
}

// overload default operator delete
void operator delete (void *ptr) noexcept {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  free(ptr);
}

void test_code_with_one_allcoated_block() {
  {
    std::shared_ptr<debug> s_ptr{new debug}; // allocate memory twice(for resource object and for shared_ptr control_block)
  } // call twice delete object

  {
    std::shared_ptr<debug> s_ptr{std::make_shared<debug>()}; // allocate one block memory(operator new called only once)
  } // call operator delete only once
}

// global object
static std::weak_ptr<debug> w_ptr;

void check_expired() {
  if (w_ptr.expired()) {
    std::cout << __PRETTY_FUNCTION__ << "resource already expired" << std::endl;
    return;
  }
  std::shared_ptr<debug> s_ptr{w_ptr.lock()};
  s_ptr->print();
}

void test_wptr_one_allocation() {
  {
    std::shared_ptr<debug> sptr{std::make_shared<debug>()}; // called operator new
    std::cout << sptr.use_count() << std::endl; // print 1
    w_ptr = sptr; // weak_ptr does not increase the reference counter
    std::cout << sptr.use_count() << std::endl; // print 1
    check_expired(); // inside this function w_ptr will not expire
  } // destroy shared_ptr called destructor for resource object, but memory is still valid
  check_expired(); // print "resource already expired"
} // called operator delete

void test_wptr_two_allocation() {
  {
    std::shared_ptr<debug> sptr{new debug}; // called operator new twice
    std::cout << sptr.use_count() << std::endl; // print 1
    w_ptr = sptr; // weak_ptr does not increase the reference counter
    std::cout << sptr.use_count() << std::endl; // print 1
    check_expired(); // inside this function w_ptr will not expire
  } // destroy shared_ptr called destructor for resource object, and operator delete for resource
  check_expired(); // print "resource already expired"
} // called operator delete for control block

using SPTR = std::shared_ptr<debug>;

void call_print(SPTR s_ptr1, SPTR s_ptr2) {
  if (s_ptr1 == nullptr || s_ptr2 == nullptr) {
    return;
  }
  s_ptr1->print();
  s_ptr2->print();
}

int main(int argc, const char** argv) {
    work_with_file();
    {
      std::shared_ptr<debug> sptr{new debug}; // called debug constructor
    } // after uptr will go out of scope the constructed will be called

    {
      std::shared_ptr<debug> sptr{new debug}; // called debug constructor
      std::cout << sptr.use_count() << std::endl; // print 1
      std::shared_ptr<debug> sptr_copy{sptr}; // increase the reference counter
      std::cout << sptr.use_count() << std::endl; // print 2
      std::shared_ptr<debug> m_ptr{std::move(sptr_copy)}; // does not increase the reference counter
      std::cout << sptr.use_count() << std::endl; // print 2
      std::cout << std::boolalpha;
      std::cout << (sptr_copy == nullptr) << std::endl; // print true
    } // after uptr will go out of scope the constructed will be called

    test_code_with_one_allcoated_block();
    test_wptr_one_allocation();
    test_wptr_two_allocation();
    // WRONG: potential resource leak before C++17, OK in C++17
    // call_print(std::shared_ptr<debug>{new debug}, std::shared_ptr<debug>{new debug});
    // CORRECT
    call_print(std::make_shared<debug>(), std::make_shared<debug>());
    return 0;
}