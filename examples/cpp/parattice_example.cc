#include "parattice.hh"
#include <fstream>
#include <iostream>

int main() {
  // initialization
  const std::vector<std::vector<std::vector<std::string>>> paradict
    = {{{"blood", "stem", "cell"}, {"造血", "幹", "細胞"}, {"hematopoietic", "stem", "cell"}},
       {{"造血", "幹", "細胞", "移植"}, {"hematopoietic", "stem", "cell", "transplantation"}},
       {{"stem", "cell"}, {"幹", "細胞"}},
       {{"幹", "細胞", "移植"}, {"rescue", "transplant"}, {"stem", "cell", "rescue"}},
       {{"rescue"}, {"救命"}},
       {{"blood"}, {"血液"}}};
  const parattice::PaRattice parattice(paradict);

  // lattice generation
  const std::vector<std::string> words = {"造血", "幹", "細胞", "移植"};
  const auto lattice = parattice.get_lattice(words, true, 2);

  // dump a generated lattice
  std::ofstream file("paraphrase-lattice.dot");
  if (!file.is_open()) {
    return -1;
  }
  file << lattice.dump_dot(true) << std::endl;

  // serialization & deserialization
  const auto bytes = lattice.to_bytes();
  const auto new_lattice = parattice::Lattice::from_bytes(bytes);

  // search
  const parattice::LatticeKMP kmp({"幹", "細胞"});
  const auto results = kmp.search(new_lattice);
  for (const auto& result : results) {
    for (const auto& edge : result) {
      std::cout << "(" << edge.first << ", " << edge.second << ") ";
    }
    std::cout << std::endl;
    const auto s = lattice.get_trunk_span(result);
    for (const auto& edge : s) {
      std::cout << "(" << edge.first << ", " << edge.second << ") ";
    }
    std::cout << std::endl << "===========" << std::endl;
  }

  return 0;
}
