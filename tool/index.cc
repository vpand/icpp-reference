/* An icpp module of the C++ standard library reference */
/* By Jesse Liu < neoliu2011@gmail.com >, 2024 */
/* Copyright (c) vpand.com 2024. This file is released under LGPL2.
   See LICENSE in root directory for more details
*/

#include <icpp.hpp>

int main(int argc, const char *argv[]) {
  auto thisdir = fs::absolute(argv[0]).parent_path();
  auto projroot = thisdir / "..";

  auto lynxroot = projroot / "build/install";
  auto htmldir = projroot / "resource/html-book/reference/en";
  auto textdir = projroot / "resource/text-book";
  if (!fs::exists(textdir))
    fs::create_directory(textdir);
  std::vector<std::string> htmls;
  for (auto &entry : fs::recursive_directory_iterator(htmldir)) {
    auto path = entry.path().string();
    if (path.ends_with(".html"))
      htmls.push_back(icpp::split(
          path, std::format("{0}en{0}", fs::path::preferred_separator))[1]);
  }
  std::ranges::sort(htmls);

  size_t index = 0;
  auto listhdr = projroot / "icpp/text-book.inc";
  std::ofstream outf(listhdr);
  if (!outf) {
    icpp::prints("Failed to create {}.\n", listhdr.string());
    return -1;
  }
  outf << "constexpr std::string_view text_book_list[] = {\n";
  for (auto &h : htmls) {
    auto name = ""s;
    for (size_t i = 0; i < h.size() - sizeof(".html") + 1; i++) {
      auto c = h[i];
      switch (c) {
      case '/':
      case '=':
      case '(':
      case ')':
        c = '_';
        break;
      default:
        break;
      }
      name += c;
    }
    name += ".txt";
    outf << R"(  ")" << name << R"(",)" << std::endl;

    index++;
    auto text = textdir / name;
    if (fs::exists(text))
      continue;

    icpp::prints("[{}/{}] converting {}...\n", index, htmls.size(), h);
    std::system(std::format("{0}/bin/lynx -cfg={0}/etc/lynx.cfg "
                            "-lss={0}/etc/lynx.lss -dump -nolist \"{1}\" > {2}",
                            lynxroot.string(), (htmldir / h).string(),
                            text.string())
                    .data());
  }
  outf << "};\n";
  icpp::prints("Created {}.\n", listhdr.string());
  return 0;
}
