#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <regex>
#include <nlohmann/json.hpp>

using PairUrlText = std::pair<std::string, std::string>;
using VectorRef = std::vector<PairUrlText>;

using json = nlohmann::json;
using namespace std::literals::string_literals;

template<typename InputIt>
[[nodiscard]] VectorRef GetContentFromStream(InputIt begin, InputIt end) {

  auto count = std::distance(begin, end);
  // auto count = end - begin;
  VectorRef vec;
  vec.reserve(count);
  while (begin != end) {
    auto url = *begin++;
    auto text = begin != end ? *begin++ : ""s;
    vec.emplace_back(url, text);

    if (text.empty()) break;
  }
  return vec;
}


enum class TypeUrl {
  HTTPS,
  HTTP
};


void split_url_report(const VectorRef& vec, json& report, TypeUrl type) {
  const char* type_url = type == TypeUrl::HTTPS ? "https" : "http";
  const char* type_node = type == TypeUrl::HTTPS ? "safe" : "unsafe";
  std::regex reg{"^("s + type_url + ")://"};
  report[type_node] = json::object();
  std::for_each(vec.begin(), vec.end(), [&reg, &report, type_url, type_node](const PairUrlText& pr) 
  {
    std::smatch match;
    if (std::regex_search(pr.first, match, reg) && match.size() == 2 && match[1] == type_url) {
      report[type_node][pr.first] = pr.second; 
    }
  });
}

int main(int argc, const char** argv) {
  std::cin >> std::noskipws;
  const std::string content{std::istream_iterator<char>(std::cin), {}};
  std::regex parse_url_content{"<a href=\"([^\"]*)\"[^<]*>([^<]*)</a>"};

  std::sregex_token_iterator it{content.begin(), content.end(), parse_url_content, {1, 2}};

  auto list_url = GetContentFromStream(it, {});

  json report;
  if (!list_url.empty()) {
    report["all"] = json::object();
    std::for_each(list_url.begin(), list_url.end(), [&report](const PairUrlText& pair) {
      report["all"][pair.first] = pair.second;
    });
    split_url_report(list_url, report, TypeUrl::HTTPS);
    split_url_report(list_url, report, TypeUrl::HTTP);
  }
  std::cout << report << std::endl;
  return 0;
}