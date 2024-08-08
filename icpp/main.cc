/* An icpp module of the C++ standard library reference */
/* By Jesse Liu < neoliu2011@gmail.com >, 2024 */
/* Copyright (c) vpand.com 2024. This file is released under LGPL2.
   See LICENSE in root directory for more details
*/

#include <icpp.hpp>
#include <lexbor/html/html.h>

// generated with: icpp ../tool/index.cc
#include "../build/html-book.h"

int main(int argc, const char *argv[]) {
  if (argc == 1) {
    icpp::prints("Module Man(C++ Manual v1.0.0) usage: icpp man regex\n"
                 "e.g.:\n\ticpp man string\n\ticpp man filesystem\n\ticpp man "
                 "*find*\n\ticpp man *file*iterator*\n");
    return -1;
  }
  return 0;
}
