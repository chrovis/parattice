# Examples

This directory contains example codes that generate a paraphrase lattice and
search a phrase in the generated lattice.

All bindings require `cargo` and `rustc` for building the parattice library.
Install the latest Rust refering [this documentation](https://www.rust-lang.org/tools/install).
To convert the generated lattice to an image file, install *Graphviz* beforehand:

Debian/Ubuntu:
```shell
sudo apt install graphviz
```

Mac OS X (Homebrew)
```shell
brew install graphviz
```

## Rust

Just run `cargo run` in [rust](/examples/rust) directory.

To convert the generated lattice to an image file, run `dot -Tpng -O ./paraphrase-lattice.dot`.

## C++

1. Install *cmake* as follows:

    Debian/Ubuntu:
    ```shell
    sudo apt install cmake
    ```

    Mac OS X (Homebrew):
    ```shell
    brew install cmake
    ```

2. Run `cargo build --release` in the [top derectory](/) of this repository to build parattice library.
3. Run `cmake . && make` in [cpp](/examples/cpp) directory to build the example code.
4. Run `./parattice_example`.
5. To convert the generated lattice to an image file, run `dot -Tpng -O ./paraphrase-lattice.dot`.

## Clojure

1. Install *Leiningen* refering [this documentation](https://leiningen.org/).
2. Install *gradle* as follows:

    Debian/Ubuntu:
    ```shell
    sudo apt install gradle
    ```

    Mac OS X (Homebrew):
    ```shell
    brew install gradle
    ```

2. Run `cargo build --release` in the [top derectory](/) of this repository to build parattice library.
3. Run the following commands in the [top directory](/) of this repository to build and to install JAR files of parattice:

    ```shell
    gradle -b bindings/jni/build.gradle build
    gradle -b bindings/jni/build.gradle publishToMavenLocal
    ```

4. If you use Mac OS X, please edit [project.clj](/examples/clojure/project.clj) as follows:

    ```clojure
    [parattice "0.2.1-SNAPSHOT" :classifier "linux-amd64"]
    ```
    ↓↓↓
    ```clojure
    [parattice "0.2.1-SNAPSHOT" :classifier "darwin-amd64"]
    ```

5. Run `lein run` in [clojure](/examples/clojure) directory.
6. To convert the generated lattice to an image file, run `dot -Tpng -O ./paraphrase-lattice.dot`.

![Example graph](/examples/graph.svg)
