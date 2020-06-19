package parattice;

public class SearchIndexNode {
  public String text;
  public int offsetStart;
  public int offsetEnd;
  public int increment;
  public int length;

  public SearchIndexNode(String text, int offsetStart, int offsetEnd, int increment, int length) {
    this.text = text;
    this.offsetStart = offsetStart;
    this.offsetEnd = offsetEnd;
    this.increment = increment;
    this.length = length;
  }
}
