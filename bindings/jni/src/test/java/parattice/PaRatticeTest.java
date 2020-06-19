package parattice;

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import org.junit.jupiter.api.Test;

public class PaRatticeTest {

  static String[][][] paradict = {
    {{"blood", "stem", "cell"}, {"造血", "幹", "細胞"}, {"hematopoietic", "stem", "cell"}},
    {{"造血", "幹", "細胞", "移植"}, {"hematopoietic", "stem", "cell", "transplantation"}},
    {{"stem", "cell"}, {"幹", "細胞"}},
    {{"幹", "細胞", "移植"}, {"rescue", "transplant"}, {"stem", "cell", "rescue"}},
    {{"rescue"}, {"救命"}},
    {{"blood"}, {"血液"}},
  };

  static class EdgeInfo implements Comparable<EdgeInfo> {
    String text;
    int nodeId;
    int targetId;
    int offsetBegin;
    int offsetEnd;
    public EdgeInfo(String text, int nodeId, int targetId, int offsetBegin, int offsetEnd) {
      this.text = text;
      this.nodeId = nodeId;
      this.targetId = targetId;
      this.offsetBegin = offsetBegin;
      this.offsetEnd = offsetEnd;
    }
    @Override
    public int compareTo(EdgeInfo other){
      int result;
      result = this.text.compareTo(other.text);
      if (result != 0) return result;
      result = this.nodeId - other.nodeId;
      if (result != 0) return result;
      result = this.targetId - other.targetId;
      if (result != 0) return result;
      result = this.offsetBegin - other.offsetBegin;
      if (result != 0) return result;
      result = this.offsetEnd - other.offsetEnd;
      return result;
    }
    @Override
    public String toString() {
      return "(" + text + ", " + nodeId + ", " + targetId + ", " + offsetBegin + ", " + offsetEnd + ")";
    }
    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof EdgeInfo)) {
        return false;
      }
      EdgeInfo other = (EdgeInfo) obj;
      return
        this.text.equals(other.text) &&
        this.nodeId == other.nodeId &&
        this.targetId == other.targetId &&
        this.offsetBegin == other.offsetBegin &&
        this.offsetEnd == other.offsetEnd;
    }
  }

  static List<EdgeInfo> searchIndexRelativeToAbsolute(List<SearchIndexNode> data) {
    List<EdgeInfo> new_data = new ArrayList<>();
    int nodeId = 0;
    for (SearchIndexNode node : data) {
      nodeId += node.increment;
      new_data.add(new EdgeInfo(node.text, nodeId - 1, nodeId + node.length - 1, node.offsetStart, node.offsetEnd));
    }
    Collections.sort(new_data);
    return new_data;
  }

  @Test
  public void dumpForSearchIndex() {
    try (PaRattice parattice = new PaRattice(paradict)) {
      String[] words = {"造血", "幹", "細胞", "移植"};
      try (Lattice lattice = parattice.getLattice(words, true, 10)) {
        List<SearchIndexNode> indexData = lattice.dumpForSearchIndex();
        List<EdgeInfo> expected = Arrays.asList(
          new EdgeInfo("造血", 0, 3, 0, 1), new EdgeInfo("blood", 0, 2, 0, 3), new EdgeInfo("血液", 0, 2, 0, 3), new EdgeInfo("hematopoietic", 0, 1, 0, 3),
          new EdgeInfo("stem", 1, 4, 0, 3), new EdgeInfo("stem", 1, 6, 0, 4), new EdgeInfo("幹", 1, 10, 0, 4),
          new EdgeInfo("rescue", 2, 11, 0, 4), new EdgeInfo("幹", 2, 7, 0, 3), new EdgeInfo("stem", 2, 5, 0, 3), new EdgeInfo("stem", 2, 6, 0, 4),
          new EdgeInfo("stem", 3, 8, 1, 3), new EdgeInfo("幹", 3, 9, 1, 2), new EdgeInfo("幹", 3, 10, 1, 4), new EdgeInfo("救命", 3, 11, 1, 4), new EdgeInfo("rescue", 3, 11, 1, 4),
          new EdgeInfo("cell", 4, 13, 0, 3), new EdgeInfo("cell", 5, 12, 0, 4), new EdgeInfo("cell", 5, 13, 0, 3), new EdgeInfo("cell", 6, 15, 0, 4), new EdgeInfo("細胞", 7, 13, 0, 3),
          new EdgeInfo("cell", 8, 13, 1, 3), new EdgeInfo("cell", 8, 14, 1, 4), new EdgeInfo("細胞", 9, 13, 2, 3), new EdgeInfo("細胞", 9, 14, 2, 4), new EdgeInfo("細胞", 10, 15, 1, 4),
          new EdgeInfo("transplant", 11, 16, 1, 4), new EdgeInfo("rescue", 12, 16, 0, 4), new EdgeInfo("救命", 12, 16, 0, 4), new EdgeInfo("移植", 13, 16, 3, 4),
          new EdgeInfo("rescue", 14, 16, 1, 4), new EdgeInfo("transplantation", 15, 16, 0, 4)
        );
        Collections.sort(expected);
        assertEquals(expected, searchIndexRelativeToAbsolute(indexData));
      }
    }
  }

  @Test
  public void serialize() throws IOException, ClassNotFoundException {
    try (PaRattice parattice = new PaRattice(paradict)) {
      String[] words = {"造血", "幹", "細胞", "移植"};
      try (Lattice lattice = parattice.getLattice(words, true, 10)) {
        ByteArrayOutputStream outs = new ByteArrayOutputStream(2000);
        ObjectOutputStream oos = new ObjectOutputStream(outs);
        oos.writeObject(lattice);
        byte[] bytes = outs.toByteArray();
        ByteArrayInputStream ins = new ByteArrayInputStream(bytes);
        ObjectInputStream ois = new ObjectInputStream(ins);
        try (Lattice latticeDeserialized = (Lattice) ois.readObject()) {
          assertEquals(lattice.dumpDot(true), latticeDeserialized.dumpDot(true));
        }
      }
    }
  }

  @Test
  public void getTrunkSpan() {
    try (PaRattice parattice = new PaRattice(paradict)) {
      String[] words = {"造血", "幹", "細胞", "移植"};
      try (Lattice lattice = parattice.getLattice(words, true, 10)) {
        List<List<Pair<String, Integer>>> testCase = Arrays.asList(
          Arrays.asList(new Pair<String, Integer>("", 1), new Pair<String, Integer>("stem", 4)),
          Arrays.asList(new Pair<String, Integer>("", 3), new Pair<String, Integer>("rescue", 11)),
          Arrays.asList(new Pair<String, Integer>("", 15), new Pair<String, Integer>("transplantation", 16))
        );
        List<List<Pair<String, Integer>>> expected = Arrays.asList(
          Arrays.asList(new Pair<String, Integer>("", 0), new Pair<String, Integer>("hematopoietic", 1), new Pair<String, Integer>("stem", 4), new Pair<String, Integer>("cell", 13)),
          Arrays.asList(new Pair<String, Integer>("", 3), new Pair<String, Integer>("rescue", 11), new Pair<String, Integer>("transplant", 16)),
          Arrays.asList(new Pair<String, Integer>("", 0), new Pair<String, Integer>("hematopoietic", 1), new Pair<String, Integer>("stem", 6), new Pair<String, Integer>("cell", 15), new Pair<String, Integer>("transplantation", 16))
        );
        for (int i = 0; i < testCase.size(); ++i) {
          assertEquals(expected.get(i), lattice.getTrunkSpan(testCase.get(i)));
        }
      }
    }
  }

  @Test
  public void maxDepth() {
    try (PaRattice parattice = new PaRattice(paradict)) {
      String[] words = {"造血", "幹", "細胞", "移植"};
      try (Lattice lattice = parattice.getLattice(words, true, 1)) {
        List<SearchIndexNode> indexData = lattice.dumpForSearchIndex();
        List<EdgeInfo> expected = Arrays.asList(
          new EdgeInfo("造血", 0, 3, 0, 1), new EdgeInfo("blood", 0, 1, 0, 3), new EdgeInfo("hematopoietic", 0, 2, 0, 3),
          new EdgeInfo("rescue", 3, 6, 1, 4), new EdgeInfo("stem", 3, 7, 1, 3), new EdgeInfo("幹", 3, 8, 1, 2), new EdgeInfo("stem", 1, 4, 0, 3),
          new EdgeInfo("stem", 2, 4, 0, 3), new EdgeInfo("stem", 2, 5, 0, 4), new EdgeInfo("cell", 7, 10, 1, 4), new EdgeInfo("cell", 7, 11, 1, 3),
          new EdgeInfo("細胞", 8, 11, 2, 3), new EdgeInfo("cell", 4, 11, 0, 3), new EdgeInfo("cell", 5, 9, 0, 4), new EdgeInfo("transplant", 6, 12, 1, 4),
          new EdgeInfo("rescue", 10, 12, 1, 4), new EdgeInfo("移植", 11, 12, 3, 4), new EdgeInfo("transplantation", 9, 12, 0, 4)
        );
        Collections.sort(expected);
        assertEquals(expected, searchIndexRelativeToAbsolute(indexData));
      }
    }
  }
}
