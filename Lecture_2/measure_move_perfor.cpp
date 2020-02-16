// movePerformance.cpp
 
#include <array>
#include <forward_list>
#include <chrono>
#include <deque>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

const int SIZE = 100'000'000; 

template <typename T>
void measurePerformance(T& t, const std::string& cont){
  
  std::cout << std::fixed << std::setprecision(10);

  auto begin= std::chrono::system_clock::now();
  // copy container
  T t1(t);
  auto last=  std::chrono::system_clock::now() - begin;
  std::cout << cont << std::endl;
  auto copyTime=  std::chrono::duration<double>(last).count();
  std::cout <<  "    Copy: " << copyTime << " sec" << std::endl;

  begin= std::chrono::system_clock::now();
  // move container
  T t2(std::move(t));
  last=  std::chrono::system_clock::now() - begin;
  auto moveTime= std::chrono::duration<double>(last).count();
  std::cout <<  "    Move: " << moveTime << " sec" << std::endl;
  
  std::cout << std::setprecision(2);
  std::cout << "    Ratio (copy time/move time): " << (copyTime/moveTime) << std::endl;
  
  std::cout << std::endl;
     
}

int main(){
    
    std::cout << std::endl;
    
    {
      std::vector<int> myVec(SIZE);
      // type deduction T == vector<int>
      measurePerformance(myVec, "std::vector<int>(SIZE)");
    }
    
    {
      std::list<int>myList(SIZE);
      // // type deduction T == list<int>
      measurePerformance(myList,"std::list<int>(SIZE)");
    }
    
}