#include <iostream>
#include <string_view>
#include <string>
#include <optional>
#include <vector>

// std::optional<int> convertStringToInt(const std::string& str) {
//   int value;
// //   auto [ptr, err] = std::from_char()
// }

void *operator new(size_t size) {
  std::cout << "allocate\n";
  return malloc(size);
}


void test(std::string_view str) {
  auto ext = str.substr(str.find_first_of(".") + 1);
  std::cout << ext << std::endl;
}


void test(const std::string& str) {
}

int main(int argc, char const *argv[]) {
  std::vector<int> v;

  v.begin();
  v.end();


  test("Hello");
  return 0;
}
