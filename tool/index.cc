/* An icpp module of the C++ standard library reference */
/* By Jesse Liu < neoliu2011@gmail.com >, 2024 */
/* Copyright (c) vpand.com 2024. This file is released under LGPL2.
   See LICENSE in root directory for more details
*/

#include <icpp.hpp>

int main(int argc, const char *argv[]) {
  auto thisdir = fs::absolute(argv[0]).parent_path();
  auto projroot = thisdir / "..";

  auto htmldir = projroot / "resource/html-book/reference/en";
  std::vector<std::string> htmls;
  for (auto &entry : fs::recursive_directory_iterator(htmldir)) {
    auto path = entry.path().string();
    if (path.ends_with(".html"))
      htmls.push_back(icpp::split(
          path, std::format("{0}en{0}", fs::path::preferred_separator))[1]);
  }
  std::ranges::sort(htmls);

  auto listhdr = projroot / "build/html-book.h";
  std::ofstream outf(listhdr);
  if (!outf) {
    icpp::prints("Failed to create {}.\n", listhdr.string());
    return -1;
  }
  outf << "constexpr std::string_view html_book_list[] = {\n";
  for (auto &h : htmls)
    outf << R"(  ")" << h << R"(",)" << std::endl;
  outf << "};\n";
  icpp::prints("Created {}.\n", listhdr.string());
  return 0;
}
