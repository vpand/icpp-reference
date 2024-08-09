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
    icpp::prints("Module Man(C++ Manual v1.0.0) usage: icpp man regex\n"
                 "e.g.:\n\ticpp man string\n\ticpp man filesystem\n\ticpp man "
                 "file*iterator\n");
    return -1;
  }
  std::regex pattern(std::string("*") + argv[1] + "*",
                     std::regex_constants::ECMAScript |
                         std::regex_constants::icase);
  std::vector<std::string_view> founds;
  for (auto &t : text_book_list) {
    std::string text{t};
    auto words_begin = std::sregex_iterator(text.begin(), text.end(), pattern);
    auto words_end = std::sregex_iterator();
    if (std::distance(words_begin, words_end))
      founds.push_back(text);
  }
  size_t sel = 0;
  if (founds.size() > 1) {
    icpp::prints("Totally found {} items of {}:\n", founds.size(), argv[1]);
    for (size_t i = 0; i < founds.size(); i++)
      icpp::prints(" * [{}] {}\n", i, founds[i]);
    icpp::prints("Which item [0, {}] do you want to browse ? ",
                 founds.size() - 1);
    std::cin >> sel;
    if (sel >= founds.size()) {
      icpp::prints("Invalid index {} input.\n", sel);
      return -1;
    }
  }
  auto textpath =
      fs::path(icpp::home_directory()) / ".icpp/asset/man" / founds[sel];
  std::ifstream inf(textpath);
  if (!fs::exists(textpath)) {
    icpp::prints("{} is missing.\n", textpath.string());
    return -1;
  }
  std::cout << inf.rdbuf();
  return 0;
}
