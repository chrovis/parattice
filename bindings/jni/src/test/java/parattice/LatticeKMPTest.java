package parattice;

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import org.junit.jupiter.api.Test;

public class LatticeKMPTest {

  static String[][][] paradict = {
    {{"blood", "stem", "cell"}, {"造血", "幹", "細胞"}, {"hematopoietic", "stem", "cell"}},
    {{"造血", "幹", "細胞", "移植"}, {"hematopoietic", "stem", "cell", "transplantation"}},
    {{"stem", "cell"}, {"幹", "細胞"}},
    {{"幹", "細胞", "移植"}, {"rescue", "transplant"}, {"stem", "cell", "rescue"}},
    {{"rescue"}, {"救命"}},
    {{"blood"}, {"血液"}},
  };

  @Test
  public void search() {
    Comparator<List<Pair<String, Integer>>> comparator = new Comparator<List<Pair<String, Integer>>>() {
      @Override
      public int compare(List<Pair<String, Integer>> o1, List<Pair<String, Integer>> o2) {
        int s = Math.min(o1.size(), o2.size());
        for (int i = 0; i < s; ++i) {
          int x = o1.get(i).compareTo(o2.get(i));
          if (x != 0) {
            return x;
          }
        }
        return o1.size() - o2.size();
      }
    };
    try (PaRattice parattice = new PaRattice(paradict)) {
      String[] words = {"造血", "幹", "細胞", "移植"};
      try (Lattice lattice = parattice.getLattice(words, true, 10)) {
        String[] pattern = {"幹", "細胞"};
        try (LatticeKMP kmp = new LatticeKMP(pattern)) {;
          List<List<Pair<String, Integer>>> result = kmp.search(lattice);
          Collections.sort(result, comparator);
          List<List<Pair<String, Integer>>> expected = Arrays.asList(
            Arrays.asList(new Pair<String, Integer>("", 1), new Pair<String, Integer>("幹", 10), new Pair<String, Integer>("細胞", 15)),
            Arrays.asList(new Pair<String, Integer>("", 2), new Pair<String, Integer>("幹", 7), new Pair<String, Integer>("細胞", 13)),
            Arrays.asList(new Pair<String, Integer>("", 3), new Pair<String, Integer>("幹", 9), new Pair<String, Integer>("細胞", 13)),
            Arrays.asList(new Pair<String, Integer>("", 3), new Pair<String, Integer>("幹", 9), new Pair<String, Integer>("細胞", 14)),
            Arrays.asList(new Pair<String, Integer>("", 3), new Pair<String, Integer>("幹", 10), new Pair<String, Integer>("細胞", 15))
          );
          assertEquals(expected, result);
        }
      }
    }
  }
}
