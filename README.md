# parattice: Recursive paraphrase lattice generator 🔄

This library takes a sentence and a paraphrase corpus, recursively finds
paraphrases based on the corpus, expands the given sentence, and generates a
paraphrase lattice.

This library also provides a method to search a phrase in the paraphrase
lattice.

parattice is implemented in Rust, and this repository also provides C++ and
Java bindings.

[![](http://meritbadge.herokuapp.com/parattice)](https://crates.io/crates/parattice)

## Build and Installation

Prerequisites:
* Rust 2018 (>= 1.31)

Run the following commands:
```shell
cargo build --release
cargo test --release
```

### C++ binding

Prerequisites:
* C++11 compiler (GCC, Clang)
* Googletest

```shell
mkdir cpp_build
cd cpp_build
cmake ../bindings/cpp -DPARATTICE_BUILD_TESTS=ON -DPARATTICE_GTEST_SOURCE_DIR=${PATH_TO_GOOGLETEST}
make VERBOSE=1
make test ARGS="-V"
```

### Java binding

Prerequisites:
* JDK (>= 8) and header files

```shell
gradle -b bindings/jni/build.gradle build
gradle -b bindings/jni/build.gradle publishToMavenLocal
```

The above commands generates two JAR files: a normal JAR file and a native library.

## Examples

Examples are contained in [examples](/examples) directory.

## Patents

* [JP2019153267](https://patentscope2.wipo.int/search/en/detail.jsf?docId=JP274788235)
* [特許第6435467号](https://www.j-platpat.inpit.go.jp/c1800/PU/JP-2019-153267/E7C117D77F8BF276A28A31DC60BF7E4CC5B53B3F230980164BD96541AA9DAA0F/11/ja)

## License

Copyright 2020 [Xcoo, Inc.](https://xcoo.jp/)

Licensed under the [Apache License, Version 2.0](/LICENSE).
