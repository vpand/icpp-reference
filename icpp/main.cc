/* An icpp module of the C++ standard library reference */
/* By Jesse Liu < neoliu2011@gmail.com >, 2024 */
/* Copyright (c) vpand.com 2024. This file is released under LGPL2.
   See LICENSE in root directory for more details
*/

#include <icpp.hpp>

// generated with: icpp ../tool/index.cc
#include "text-book.inc"

int main(int argc, const char *argv[]) {
  if (argc == 1) {
    icpp::prints("Module CppReference(v1.0.0) usage: icpp refs regex\n"
                 "e.g.:\n\ticpp refs filesystem\n\ticpp refs "
                 "\"file.*iterator\"\n\ticpp refs \"vector.*push\"\n");
    return -1;
  }

  auto expr = argv[1];
  for (; *expr && !std::isalpha(*expr); expr++)
    ;
  if (!*expr) {
    icpp::prints("Regex without any letter doesn't make any sense.\n");
    return -1;
  }

  auto prefix = ""s;
  bool wildcard = false;
  for (auto ptr = expr; *ptr && std::isalpha(*ptr); prefix += *ptr++)
    ;
  for (auto ptr = expr; *ptr && !wildcard; ptr++) {
    switch (*ptr) {
    case '.':
    case '*':
    case '+':
    case '?':
    case '\\':
      wildcard = true;
      break;
    default:
      break;
    }
  }

  std::regex pattern(expr, std::regex_constants::extended |
                               std::regex_constants::icase);
  std::vector<std::string_view> founds;
  bool prefix_matched = false;
  for (auto &t : text_book_list) {
    if (!wildcard) {
      if (t.find(prefix) != std::string_view::npos)
        founds.push_back(t);
      continue;
    }

    if (!prefix_matched) {
      if (t.find(prefix) == std::string_view::npos)
        continue;
      prefix_matched = true;
    } else if (t.find(prefix) == std::string_view::npos) {
      prefix_matched = false;
      continue;
    }

    if (std::regex_search(t.data(), t.data() + t.size(), pattern))
      founds.push_back(t);
  }
  if (founds.size() == 0) {
    icpp::prints("Nothing found for '{}'.\n", expr);
    return -1;
  }

  size_t sel = 0;
  if (founds.size() > 1) {
    icpp::prints("Totally found {} items of '{}':\n", founds.size(), expr);
    for (size_t i = 0; i < founds.size(); i++)
      icpp::prints(" * [{}] {}\n", i, fs::path(founds[i]).stem().string());
    icpp::prints("Which item [0, {}] do you want to browse ? ",
                 founds.size() - 1);
    std::cin >> sel;
    if (sel >= founds.size()) {
      icpp::prints("Invalid index {} input.\n", sel);
      return -1;
    }
  }
  auto textpath =
      fs::path(icpp::home_directory()) / ".icpp/asset/refs" / founds[sel];
  std::ifstream inf(textpath);
  if (!fs::exists(textpath)) {
    icpp::prints("{} is missing.\n", textpath.string());
    return -1;
  }
  std::cout << inf.rdbuf();
  return 0;
}
