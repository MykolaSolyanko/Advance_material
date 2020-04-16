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

class iterator_json: public std::iterator<std::output_iterator_tag, PairUrlText> {
  public:
    iterator_json(json& json_journal) : json_doc{json_journal} {
      json_doc[safe] = json::object();
      json_doc[unsafe] = json::object();
    }
    iterator_json& operator=(const PairUrlText& pr) {
      std::smatch match;
      if (std::regex_search(pr.first, match, reg_https) && match.size() == 2 && match[1] == "https") {
        json_doc[safe][pr.first] = pr.second;
      } else if (std::regex_search(pr.first, match, reg_http) && match.size() == 2 && match[1] == "http") {
        json_doc[unsafe][pr.first] = pr.second;
      }
      return *this;
    }
    iterator_json& operator*() {
      return *this;
    }
    iterator_json& operator++() {
      return *this;
    }
    iterator_json& operator++(int) {
      return *this;
    }

  private:
    json& json_doc;
    std::regex reg_https{"^(https)://"};
    std::regex reg_http{"^(http)://"};
    const char* safe{"safe"};
    const char* unsafe{"unsafe"};
};

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

[[nodiscard]] iterator_json iterator_json_adapter(json& report) {
  return iterator_json(report);
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

bool Compare (const PairUrlText& pr) {
    std::regex reg{"^https?://(.*)"};
    std::smatch base_match;
    return std::regex_match(pr.first, base_match, reg);
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
    std::for_each(std::next(list_url.begin(), 3), list_url.end(), [&report](const PairUrlText& pair) {
      report["all"][pair.first] = pair.second;
    });
    std::copy(list_url.begin(), list_url.end(), iterator_json_adapter(report));
  }
  bool is_true = std::all_of(list_url.begin(), list_url.end(), Compare);

  std::cout << std::boolalpha;
  std::cout << is_true << std::endl;

  is_true = std::any_of(list_url.begin(), list_url.end(), [](const PairUrlText& pr) {
    std::regex reg{"^https?://(.*)"};
    std::smatch base_match;
    return std::regex_match(pr.first, base_match, reg);
  });

  std::cout << is_true << std::endl;
  // std::cout << report << std::endl;
  return 0;
}