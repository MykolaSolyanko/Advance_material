#include <iostream>
#include <typeinfo>

class Base {
  public:
    virtual void test() = 0; // pure function
    virtual ~Base() = default; // without it memory leak
};

class Derived1 : public Base {
  public:
    void test() override {std::cout << __PRETTY_FUNCTION__ << std::endl;}
    void print_data() {
      for(auto var : array) {
        std::cout << var << std::endl;
      }
    }
  private:
    int array[10]{};
};

class Derived2 : public Base {
  public:
    void test() override {std::cout << __PRETTY_FUNCTION__ << std::endl;}
    void print_data() {
      int* end = array + 100;
      int* begin = array;
      while (begin != end) {
         std::cout << *begin++ << std::endl; 
      }
    }
  private:
    int *array = new int[10]{};
};

void incorect_proccess_data(Base *base) {
  Derived1* d = static_cast<Derived1*>(base); // UB
  d->print_data();
}

void correct_proccess_data(Base *base) { // if pase base as references (exception could throw)
  Derived1* d = dynamic_cast<Derived1*>(base); // using RTTI
  if (d != nullptr) {
    d->print_data();
    return;
  }
  Derived2* d2 = dynamic_cast<Derived2*>(base);
  if (d2 != nullptr) {
    d2->print_data();
  }
}

void correct_using_typeid_proccess_data(Base *base) { // if pase base as references (exception could throw)

  if (typeid(*base).name() == typeid(Derived1).name()) {
     Derived1* d = static_cast<Derived1*>(base); // using RTTI
     if (d != nullptr) {
       d->print_data();
       return;
     }
  }
  Derived2* d2 = static_cast<Derived2*>(base);
  if (d2 != nullptr) {
    d2->print_data();
  }
}

int main(int argc, char const *argv[]) {
    Derived1 d1;
    incorect_proccess_data(&d1);
    Derived1 d2;
    incorect_proccess_data(&d2); // UB

    correct_proccess_data(&d1);
    correct_proccess_data(&d2);

    correct_using_typeid_proccess_data(&d1);
    return 0;
}
