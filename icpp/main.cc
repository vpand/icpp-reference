/* An icpp module of the C++ standard library reference */
/* By Jesse Liu < neoliu2011@gmail.com >, 2024 */
/* Copyright (c) vpand.com 2024. This file is released under LGPL2.
   See LICENSE in root directory for more details
*/

#include <icpp.hpp>

// generated with: icpp ../tool/index.cc
#include "text-book.inc"

constexpr bool logging = false;

#define log(format, ...)                                                       \
  if (logging) {                                                               \
    icpp::prints(format, __VA_ARGS__);                                         \
  }

struct match_context {
  match_context(size_t start, size_t end,
                const std::vector<std::string_view> &list,
                const icpp::regex &pattern)
      : start_(start), end_(end), list_(list), pattern_(pattern) {}

  void dump() {
    log("match context {}: [{} {})\n", static_cast<const void *>(this), start_,
        end_);
  }

  void match() {
    for (size_t i = start_; i < end_; i++) {
      auto t = list_[i];
      if (pattern_.search(t))
        result.push_back(i);
    }
    log("match context {}: found {}.\n", static_cast<const void *>(this),
        result.size());
  }

  std::vector<size_t> result;

private:
  size_t start_, end_;
  const std::vector<std::string_view> &list_;
  const icpp::regex &pattern_;
};

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
  for (auto ptr = expr; *ptr && std::isalpha(*ptr); ptr++)
    ;
  for (auto ptr = expr; *ptr && !wildcard; ptr++) {
    switch (*ptr) {
    case '.':
    case '*':
    case '+':
    case '?':
    case '(':
    case ')':
    case '|':
    case '\\':
      wildcard = true;
      break;
    default:
      prefix += *ptr;
      break;
    }
  }
  log("Prefix '{}', wildcard {}.\n", prefix, wildcard);

  std::vector<std::string_view> prefounds, founds;
  for (auto &t : text_book_list) {
    if (!wildcard) {
      if (t.find(prefix) != std::string_view::npos)
        founds.push_back(t);
      continue;
    }

    if (t.find(prefix) != std::string_view::npos)
      prefounds.push_back(t);
  }
  if (prefounds.size() == 0 && founds.size() == 0) {
    icpp::prints("Nothing found for '{}'.\n", expr);
    return -1;
  }

  if (prefounds.size()) {
    log("Regular expression '{}'.\n", expr);
    icpp::regex pattern(expr);
    auto threads = std::thread::hardware_concurrency();
    auto eachnum = prefounds.size() / threads;
    size_t start = 0;
    std::vector<match_context> mcontexts;
    for (unsigned i = 0; i < threads - 1; i++) {
      mcontexts.push_back(
          std::move(match_context(start, start + eachnum, prefounds, pattern)));
      start += eachnum;
    }
    mcontexts.push_back(
        std::move(match_context(start, prefounds.size(), prefounds, pattern)));
    for (auto &m : mcontexts)
      m.dump();

    std::thread **ths = new std::thread *[threads];
    log("Create and run {} threads...\n", threads);
    for (int i = 0; i < threads; i++)
      ths[i] = new std::thread([&mcontexts, i] { mcontexts[i].match(); });
    for (int i = 0; i < threads; i++) {
      ths[i]->join();
      delete ths[i];
    }
    delete[] ths;
    log("{}\n", "Done.");

    for (auto &m : mcontexts)
      for (auto i : m.result)
        founds.push_back(prefounds[i]);
    if (founds.size() == 0) {
      icpp::prints("Nothing found for '{}'.\n", expr);
      return -1;
    }
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
