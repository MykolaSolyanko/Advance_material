
#include <fstream>
#include <string>
#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <map>
#include <sstream>
// #include <filesystem>

constexpr size_t OneHundredMb = 100'000'000;
void random_write(const std::string &path) {
  std::mt19937 rng;
  std::uniform_int_distribution dist{0, 1000};
  std::fstream f{path, std::ios_base::out};
  if (f.is_open()) {
    std::generate_n(std::ostreambuf_iterator<char>(f), OneHundredMb, [&dist, &rng](){return dist(rng);});
  }
}

void create_file() {
  std::cout << "creating files...\n";
  for (size_t i = 1; i <= 5; i++) {
    random_write("./test_cash_file/file" + std::to_string(i) + ".txt");    
  }
  std::cout << "files creation completed\n";
}

std::string get_data_from_file(const std::string& path) {
  std::fstream f{path};
  if (f.is_open()) {
    std::stringstream content;
    content << f.rdbuf();
    return content.str();
  }
  return std::string{""};
}

std::string open_file(const std::string& path) {
  static std::map<std::string, std::string> m;
  if (!m.count(path) != m.end()) {
    std::cout << "read from cash" << std::endl;
    return m[path];
  }
  auto content = get_data_from_file(path);
  std::cout << "read data from disk [size = " << content.size() << "]\n";
  if (!content.empty()) {
    m.emplace(path, content);
  }
  return content;
}

int main(int argc, char const *argv[]) {  
  // create_file();
  // while(true) {
  //   std::cout << "Enter file name : ";
  //   std::string file_name;
  //   std::cin >> file_name;
  //   file_name = "test_cash_file/" + file_name;
  //   std::cout << "reading file...\n";
  //   open_file(file_name);
  //   std::cout << "file reading completed\n";
  // }

  // std::map<std::string, std::string> m;
  // if ()

  std::map<std::string, std::string> m;

  // if (m.find("Hello") == m.end()) {
  //   m.insert({"Hello", "World"})
  //   std::cout << "ok\n";
  // }
   if (auto [it, succ] = m.try_emplace("Hello", "World") ; succ) {

   }

  //  auto value = v[10];
  return 0;
}
