#include <iostream>

int* alloc(size_t count) {
  int* array = new int[count];

  return array;
}
// Raii class, for automatic remove resource when object leave leaves scope
class Raii {
  public:
   Raii(int* ptr) : res{ptr} {}
   ~Raii() {
     std::cout << "dst\n";
     delete [] res;
    }
  private:
   int* res;
};

bool chck() {
    return false;
}

int main(int argc, const char** argv) {

    Raii raii {alloc(100)};
    if (chck()) {
      return 0; // if result if operatora equal true, then then call the destructor
    }
    // if result if operatora equal false, then then call the destructor
    return 0;
}