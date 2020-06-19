#include <parattice.hh>

#include <algorithm>
#include <tuple>
#include <vector>

#include <gtest/gtest.h>

namespace parattice {

  std::vector<std::tuple<std::string, std::size_t, std::size_t, std::size_t, std::size_t>> search_index_relative_to_absolute(const std::vector<search_index_node>& data) {
    std::vector<std::tuple<std::string, std::size_t, std::size_t, std::size_t, std::size_t>> new_data;
    std::size_t node_id = 0;
    for (auto& node : data) {
      node_id += node.increment;
      new_data.emplace_back(node.text, node_id - 1, node_id + node.length - 1, node.offset_start, node.offset_end);
    }
    std::sort(new_data.begin(), new_data.end());
    return new_data;
  }

  class PaRatticeTest : public testing::Test {
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

  TEST_F(PaRatticeTest, DumpForSearchIndex) {
    PaRattice parattice(paradict);
    const std::vector<std::string> words = {"造血", "幹", "細胞", "移植"};
    const Lattice lattice = parattice.get_lattice(words, true, 10);
    const std::vector<search_index_node> index_data = lattice.dump_for_search_index();
    using idxitem = std::tuple<std::string, std::size_t, std::size_t, std::size_t, std::size_t>;
    std::vector<std::tuple<std::string, std::size_t, std::size_t, std::size_t, std::size_t>> expected = {
      idxitem {"造血", 0, 3, 0, 1}, idxitem {"blood", 0, 2, 0, 3}, idxitem {"血液", 0, 2, 0, 3}, idxitem {"hematopoietic", 0, 1, 0, 3},
      idxitem {"stem", 1, 4, 0, 3}, idxitem {"stem", 1, 6, 0, 4}, idxitem {"幹", 1, 10, 0, 4},
      idxitem {"rescue", 2, 11, 0, 4}, idxitem {"幹", 2, 7, 0, 3}, idxitem {"stem", 2, 5, 0, 3}, idxitem {"stem", 2, 6, 0, 4},
      idxitem {"stem", 3, 8, 1, 3}, idxitem {"幹", 3, 9, 1, 2}, idxitem {"幹", 3, 10, 1, 4}, idxitem {"救命", 3, 11, 1, 4}, idxitem {"rescue", 3, 11, 1, 4},
      idxitem {"cell", 4, 13, 0, 3}, idxitem {"cell", 5, 12, 0, 4}, idxitem {"cell", 5, 13, 0, 3}, idxitem {"cell", 6, 15, 0, 4}, idxitem {"細胞", 7, 13, 0, 3},
      idxitem {"cell", 8, 13, 1, 3}, idxitem {"cell", 8, 14, 1, 4}, idxitem {"細胞", 9, 13, 2, 3}, idxitem {"細胞", 9, 14, 2, 4}, idxitem {"細胞", 10, 15, 1, 4},
      idxitem {"transplant", 11, 16, 1, 4}, idxitem {"rescue", 12, 16, 0, 4}, idxitem {"救命", 12, 16, 0, 4}, idxitem {"移植", 13, 16, 3, 4},
      idxitem {"rescue", 14, 16, 1, 4}, idxitem {"transplantation", 15, 16, 0, 4},
    };
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(expected, search_index_relative_to_absolute(index_data));
  }

  TEST_F(PaRatticeTest, Serialize) {
    PaRattice parattice(paradict);
    const std::vector<std::string> words = {"造血", "幹", "細胞", "移植"};
    const Lattice lattice = parattice.get_lattice(words, true, 10);
    const std::vector<std::uint8_t> bytes = lattice.to_bytes();
    const Lattice deserialized_lattice = Lattice::from_bytes(bytes);
    EXPECT_EQ(lattice.dump_dot(true), deserialized_lattice.dump_dot(true));
  }

  TEST_F(PaRatticeTest, GetTrunkSpanTest) {
    PaRattice parattice(paradict);
    const std::vector<std::string> words = {"造血", "幹", "細胞", "移植"};
    const Lattice lattice = parattice.get_lattice(words, true, 10);
    const std::vector<std::vector<std::pair<std::string, std::size_t>>> test_case = {
      {{"", 1}, {"stem", 4}},
      {{"", 3}, {"rescue", 11}},
      {{"", 15}, {"transplantation", 16}},
    };
    const std::vector<std::vector<std::pair<std::string, std::size_t>>> expected = {
      {{"", 0}, {"hematopoietic", 1}, {"stem", 4}, {"cell", 13}},
      {{"", 3}, {"rescue", 11}, {"transplant", 16}},
      {{"", 0}, {"hematopoietic", 1}, {"stem", 6}, {"cell", 15}, {"transplantation", 16}},
    };
    for (std::size_t i = 0; i < test_case.size(); ++i) {
      EXPECT_EQ(expected[i], lattice.get_trunk_span(test_case[i]));
    }
  }

  TEST_F(PaRatticeTest, MaxDepth) {
    PaRattice parattice(paradict);
    const std::vector<std::string> words = {"造血", "幹", "細胞", "移植"};
    const Lattice lattice = parattice.get_lattice(words, true, 1);
    const std::vector<search_index_node> index_data = lattice.dump_for_search_index();
    using idxitem = std::tuple<std::string, std::size_t, std::size_t, std::size_t, std::size_t>;
    std::vector<std::tuple<std::string, std::size_t, std::size_t, std::size_t, std::size_t>> expected = {
      idxitem {"造血", 0, 3, 0, 1}, idxitem {"blood", 0, 1, 0, 3}, idxitem {"hematopoietic", 0, 2, 0, 3},
      idxitem {"rescue", 3, 6, 1, 4}, idxitem {"stem", 3, 7, 1, 3}, idxitem {"幹", 3, 8, 1, 2}, idxitem {"stem", 1, 4, 0, 3},
      idxitem {"stem", 2, 4, 0, 3}, idxitem {"stem", 2, 5, 0, 4}, idxitem {"cell", 7, 10, 1, 4}, idxitem {"cell", 7, 11, 1, 3},
      idxitem {"細胞", 8, 11, 2, 3}, idxitem {"cell", 4, 11, 0, 3}, idxitem {"cell", 5, 9, 0, 4}, idxitem {"transplant", 6, 12, 1, 4},
      idxitem {"rescue", 10, 12, 1, 4}, idxitem {"移植", 11, 12, 3, 4}, idxitem {"transplantation", 9, 12, 0, 4},
    };
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(expected, search_index_relative_to_absolute(index_data));
  }

}  // namespace parattice
