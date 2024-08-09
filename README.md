# ICPP-Reference - C++ standard library reference
An icpp module of the C++ standard library reference.

```sh
icpp@vpand icpp-reference % icpp man
Module Man(C++ Manual v1.0.0) usage: icpp man regex
e.g.:
        icpp man string
        icpp man filesystem
        icpp man file*iterator
```

**Follow the one-by-one step to build your own version of icpp-reference module.**

## Build ncurses
```sh
mkdir -p build/ncurses
cd build/ncurses
../../third/ncurses/configure --prefix=$PWD/install
make -j8
make install
```

## Build Lynx
```sh
cd ..
CFLAGS="-std=c89 -Wno-implicit-function-declaration -O2" ../third/Lynx/configure --with-curses-dir=$PWD/ncurses/install --prefix=$PWD/install
make -j8
make install
```

## Uncompress
Uncompress the resource/html-book.tar.sz as resource/html-book.

## Html to text
```sh
icpp tool/index.cc
```

## Create icpp module
```sh
imod --create=icpp.json
```

## Install
```sh
# replace the exact .icpp name on your system
imod --create=mac-osx-arm64.icpp
```

## Test
```sh
icpp man vector*push
```
