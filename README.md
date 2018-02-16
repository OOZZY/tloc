# tloc

A C library.

## Build Requirements

* CMake
* C11 development environment for which CMake can generate build files

## Clone, Build, and Test

Clone into tloc directory.

```
$ git clone --branch develop <url/to/tloc.git>
```

Build.

```
$ mkdir tlocbuild
$ cd tlocbuild
$ cmake -G 'Unix Makefiles' -DCMAKE_BUILD_TYPE=Debug ../tloc
$ make
```

Run tests with make.

```
$ make test
```

Run tests directly.

```
$ ./test/tloc_test
```
