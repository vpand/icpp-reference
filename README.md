# ICPP-Reference - C++ standard library reference
An icpp module of the C++ standard library reference.

```sh
icpp@vpand icpp-reference % icpp refs
Module CppReference(v1.0.0) usage: icpp refs regex
e.g.:
        icpp refs filesystem
        icpp refs "file.*iterator"
        icpp refs "vector.*push" 
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
icpp@vpand icpp-reference % icpp refs "vector.*push"  
std::vector<T,Allocator>::push_back

   From cppreference.com
   < cpp | container | vector

   ...
      void push_back( const T& value );
   (1) (constexpr since C++20)
   void push_back( T&& value );
   (2) (since C++11)
   (constexpr since C++20)

   Appends the given element value to the end of the container.
   1) The new element is initialized as a copy of value.
   2) value is moved into the new element.

   If after the operation the new size() is greater than old capacity() a
   reallocation takes place, in which case all iterators (including the
   end() iterator) and all references to the elements are invalidated.
   Otherwise only the end() iterator is invalidated.

Parameters

   value - the value of the element to append
   Type requirements
   -
   T must meet the requirements of CopyInsertable in order to use overload
   (1).
   -
   T must meet the requirements of MoveInsertable in order to use overload
   (2).

Return value

   (none)
   ...
```
