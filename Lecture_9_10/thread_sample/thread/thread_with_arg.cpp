
#include <iostream>
#include <thread>

class Account {
  public:
    Account(int value) : dep{value} {}
    void add_dep(int value) {
      dep += value;
    }
    void minus_dep(int value) {
      dep -= value;
    }
    long long deposit() { return dep; }
  private:
   long long dep{};
};

void thread1(Account& ac1, Account& ac2, int value) {
  if (ac2.deposit() - value <= 0) {
    std::cout << "not enough money in your account\n";
    return;
  }
  ac1.add_dep(value);
  ac2.minus_dep(value);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

class PeopleAccount {
  public:
    void trasfer() {
      std::thread([this] { change_data(); print_account(); }).detach();
    }
    void print_account() {
      std::cout << credit.deposit() << " " << dep.deposit() << std::endl;
    }
    void change_data() {thread1(std::ref(credit), std::ref(dep), 10);}
  private:
    Account credit{1000};
    Account dep{1000};
};

void duglingRef() {
  PeopleAccount account;
  account.trasfer(); // potential dugling references
}

int main(int argc, const char** argv) {
    duglingRef();

    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 0;
}