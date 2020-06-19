package parattice;

import java.util.List;

import parattice.internal.JNILoader;

public class PaRattice implements AutoCloseable {

  protected long handle;

  public PaRattice(String[][][] dict) {
    this.handle = jniNew(dict);
  }

  public PaRattice(List<List<List<String>>> dict) {
    String[][][] dictArr = new String[dict.size()][][];
    int i = 0;
    for (List<List<String>> group : dict) {
      dictArr[i] = new String[group.size()][];
      int j = 0;
      for (List<String> phrase : group) {
        dictArr[i][j] = phrase.toArray(new String[phrase.size()]);
        ++j;
      }
      ++i;
    }
    this.handle = jniNew(dictArr);
  }

  public void dispose() {
    if (this.handle == 0) {
      return;
    }
    jniDelete(this.handle);
    this.handle = 0;
  }

  @Override
  public void close() {
    dispose();
  }

  public Lattice getLattice(String[] sentence, boolean shrink, int max_depth) {
    Lattice lattice = new Lattice(jniGetLattice(this.handle, sentence, shrink, max_depth));
    lattice.sentence = sentence;
    return lattice;
  }

  public Lattice getLattice(List<String> sentence, boolean shrink, int max_depth) {
    String[] sentencearr = sentence.toArray(new String[sentence.size()]);
    Lattice lattice = new Lattice(jniGetLattice(this.handle, sentencearr, shrink, max_depth));
    lattice.sentence = sentencearr;
    return lattice;
  }

  private native long jniNew(String[][][] dict);
  private native void jniDelete(long handle);
  private native long jniGetLattice(long handle, String[] sentence, boolean shrink, int max_depth);

  static {
    JNILoader.loadLibrary();
  }
}
