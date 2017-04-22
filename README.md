# Clone, Build, and Test

```
$ git clone <url/to/tloc.git> # clone into tloc directory
$ mkdir tlocbuild
$ cd tlocbuild
$ cmake -G 'Unix Makefiles' -DCMAKE_BUILD_TYPE=Debug ../tloc
$ make
$ make test # run tests with make
$ ./tloc_test # run tests directly
```
