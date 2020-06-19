package parattice;

import java.util.ArrayList;
import java.util.List;

import parattice.internal.JNILoader;

public class LatticeKMP implements AutoCloseable {

  public LatticeKMP(String[] pattern) {
    this.handle = jniNew(pattern);
  }

  public LatticeKMP(List<String> pattern) {
    this.handle = jniNew(pattern.toArray(new String[pattern.size()]));
  }

  protected long handle;

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

  public List<List<Pair<String, Integer>>> search(Lattice lattice) {
    long resultHandle = jniSearch(this.handle, lattice.getHandle());
    int[] resultSizes = jniSearchLength(resultHandle);
    int sizeTotal = 0;
    for (int resultSize : resultSizes) {
      sizeTotal += resultSize;
    }
    String[] resultString = new String[sizeTotal];
    int[] resultNodeId = new int[sizeTotal];
    jniSearchGetDataAndFree(resultHandle, resultString, resultNodeId);
    List<List<Pair<String, Integer>>> result = new ArrayList<>();
    int k = 0;
    for (int i = 0; i < resultSizes.length; ++i) {
      List<Pair<String, Integer>> resultArr = new ArrayList<>();
      for (int j = 0; j < resultSizes[i]; ++j) {
        resultArr.add(new Pair<String, Integer>(resultString[k], resultNodeId[k]));
        ++k;
      }
      result.add(resultArr);
    }
    return result;
  }

  private native long jniNew(String[] pattern);
  private native void jniDelete(long handle);
  private static native long jniSearch(long handle, long latticeHandle);
  private static native int[] jniSearchLength(long resultHandle);
  private static native void jniSearchGetDataAndFree(long resultHandle, String[] resultString, int[] resultNodeId);

  static {
    JNILoader.loadLibrary();
  }
}
