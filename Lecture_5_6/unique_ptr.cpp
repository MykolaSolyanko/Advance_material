#include <iostream>
#include <memory>
#include <cstdio>
#include <functional>

class debug {
  public:
    debug(){std::cout << __PRETTY_FUNCTION__ << std::endl;}
    ~debug(){std::cout << __PRETTY_FUNCTION__ << std::endl;}
    void print() {std::cout << __PRETTY_FUNCTION__ << std::endl;}
};

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
//   std::unique_ptr<FILE, decltype(&fclose)/*or void (*) (FILE*) */> file(fopen("some_file", "w"), fclose); // function close will be called in the destructor unique_ptr
  // or using lambda
   std::unique_ptr<FILE, std::function<decltype(fclose)>> file(fopen("some_file", "w"), [](FILE* f) {
      std::cout << "clear resource" << std::endl;
      if (f != nullptr) {
        return -1;
      }
      return fclose(f);
  }); 
  if (fwrite(hw, sizeof(char), std::size(hw), file.get()) != std::size(hw)) {
    std::cout << "unable to write data to file" << std::endl;
  }
}

using Dptr = std::unique_ptr<debug>; // alias for unique_ptr

void call(Dptr first, Dptr second) {
  if (first == nullptr || second == nullptr) {
    return;
  }
  first->print();
  second->print();
}

int main(int argc, char const *argv[]) {
    {
        std::unique_ptr<debug> uptr {new debug}; // called debug constructor
    } // after uptr will go out of scope the constructed will be called

    // the unique ptr uses only the move semantics
    {
      std::unique_ptr<debug> uptr {new debug};
      // std::unique_ptr<debug> uptr_copy {uptr}; // compile error: call implicit deleted copy constructor
      std::unique_ptr<debug> uptr_copy {std::move(uptr)}; // compile error: call implicit deleted copy constructor
    }
    {
      // std::unique_ptr<debug[]> u_ptr_array{new debug[10]}; // allocated memory for 10 debug object 
      std::unique_ptr<debug[]> u_ptr_array{std::make_unique<debug[]>(10)};

    }

    work_with_file();
    // WRONG: potential resource leak before C++17, OK in C++17
    // call(std::unique_ptr<debug>{new debug}, std::unique_ptr<debug>{new debug});
    // BETTER
    call(std::make_unique<debug>(), std::make_unique<debug>());
    return 0;
}
