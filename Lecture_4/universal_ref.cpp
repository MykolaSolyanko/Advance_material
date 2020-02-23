struct Point {
 int x{};
 int y{};
};


using test = Point&;

int main(int argc, const char** argv) {
  auto&& ref = Point{1, 1}; // ref is &&-> rvalue reference (&& && -> &&)

  Point p;

  test t = p; // t is & lvalue ref
  decltype(t)& check = p; // check is & lvalue ref (& & -> &)

  auto&& l_ref = p; // l_ref is &-> lvalue references (&& & -> &)

  
  return 0;
}