
#include <iostream>
#include <mutex>
#include <thread>

class Account{
  public:
   void add_to_deposit(int amount) {
     m.lock();
     deposit += amount;
     m.unlock();
   }
  void take_from_account(int amount) {
    m.lock();
    deposit -= amount;
    m.unlock();
  }
  int get_deposit() {
    return deposit;
  }

  friend void transfer_money(Account& a1, Account& a2, int amount);
  private:
   std::mutex m;
   int deposit;
};

void transfer_money(Account& a1, Account& a2, int amount) {
  // a1.m.lock();
  // a2.m.lock();
  // std::unique_lock<std::mutex> l1{a1.m};
  // std::unique_lock<std::mutex> l2{a2.m};
  std::lock(a1.m, a2.m);
  std::scoped_lock lock(a1.m, a2.m);

  a1.deposit -= amount;
  a2.deposit += amount;
}

void test_deposit() {
    Account a1, a2;
    a1.add_to_deposit(1000);
    a2.add_to_deposit(2000);

    std::thread t1{transfer_money, std::ref(a1), std::ref(a2), 100};
    std::thread t2{transfer_money, std::ref(a2), std::ref(a1), 500};
    t1.join();
    t2.join();
}


int main(int argc, const char** argv) {
    test_deposit();
    return 0;
}