#include <parattice.hh>

#include <algorithm>
#include <tuple>
#include <vector>

#include <gtest/gtest.h>

namespace parattice {

  class LatticeKMPTest : public testing::Test {
    protected:
      std::vector<std::vector<std::vector<std::string>>> paradict = {};

      void SetUp() override {
        paradict = {
          {{"blood", "stem", "cell"}, {"造血", "幹", "細胞"}, {"hematopoietic", "stem", "cell"}},
          {{"造血", "幹", "細胞", "移植"}, {"hematopoietic", "stem", "cell", "transplantation"}},
          {{"stem", "cell"}, {"幹", "細胞"}},
          {{"幹", "細胞", "移植"}, {"rescue", "transplant"}, {"stem", "cell", "rescue"}},
          {{"rescue"}, {"救命"}},
          {{"blood"}, {"血液"}},
        };
      }
  };

  TEST_F(LatticeKMPTest, SearchTest) {
    PaRattice parattice(paradict);
    const std::vector<std::string> words = {"造血", "幹", "細胞", "移植"};
    const Lattice lattice = parattice.get_lattice(words, true, 10);
    const std::vector<std::string> pattern = {"幹", "細胞"};
    const LatticeKMP kmp(pattern);
    std::vector<std::vector<std::pair<std::string, std::size_t>>> results = kmp.search(lattice);
    std::sort(results.begin(), results.end());
    std::vector<std::vector<std::pair<std::string, std::size_t>>> expected = {
      {{"", 1}, {"幹", 10}, {"細胞", 15}},
      {{"", 2}, {"幹", 7}, {"細胞", 13}},
      {{"", 3}, {"幹", 9}, {"細胞", 13}},
      {{"", 3}, {"幹", 9}, {"細胞", 14}},
      {{"", 3}, {"幹", 10}, {"細胞", 15}},
    };
    EXPECT_EQ(expected, results);
  }

}  // namespace parattice
