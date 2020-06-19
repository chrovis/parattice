package parattice;

import java.io.Externalizable;
import java.io.IOException;
import java.io.ObjectInput;
import java.io.ObjectOutput;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.List;

import parattice.internal.JNILoader;

public class Lattice implements Externalizable, AutoCloseable {

  public Lattice(long handle) {
    this.handle = handle;
  }

  public Lattice() {}

  protected long handle = 0;
  protected String[] sentence;

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

  public long getHandle() {
    return this.handle;
  }

  public int getSize() {
    if (this.handle == 0) {
      throw new IllegalStateException();
    }
    return jniGetSize(this.handle);
  }

  public byte[] toBytes() {
    if (this.handle == 0) {
      throw new IllegalStateException();
    }
    return jniToBytes(this.handle);
  }

  public static Lattice fromBytes(byte[] data) {
    return new Lattice(jniNewFromBytes(data));
  }

  public void writeExternal(ObjectOutput out) throws IOException {
    if (this.handle == 0) {
      throw new IllegalStateException();
    }
    byte[] data = jniToBytes(this.handle);
    out.writeInt(data.length);
    out.write(data);
  }

  public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {
    if (this.handle == 0) {
      int cap = in.readInt();
      byte[] data = new byte[cap];
      int s = 0;
      int sizeSum = 0;
      do {
        if ((s = in.read(data, sizeSum, cap - sizeSum)) == -1) {
          break;
        }
        sizeSum += s;
      } while (sizeSum != cap);
      if (sizeSum != cap) {
        throw new IOException("size mismatch: read " + sizeSum + " bytes, expected " + cap + " bytes");
      }
      this.handle = jniNewFromBytes(data);
    } else {
      throw new IllegalStateException();
    }
  }

  public String dumpDot(boolean isNumbered) {
    if (this.handle == 0) {
      throw new IllegalStateException();
    }
    return jniDumpDot(this.handle, isNumbered);
  }

  public List<Pair<String, Integer>> getTrunkSpan(List<Pair<String, Integer>> path) {
    if (this.handle == 0) {
      throw new IllegalStateException();
    }
    String[] pathString = new String[path.size()];
    int[] pathNodeId = new int[path.size()];
    for (int i = 0; i < path.size(); ++i) {
      pathString[i] = path.get(i).first;
      pathNodeId[i] = path.get(i).second;
    }
    int latticeSize = getSize();
    String[] resultString = new String[latticeSize];
    int[] resultNodeId = new int[latticeSize];
    int s = jniGetTrunkSpan(this.handle, pathString, pathNodeId, resultString, resultNodeId);
    List<Pair<String, Integer>> result = new ArrayList<>();
    for (int i = 0; i < s; ++i) {
      result.add(new Pair<String, Integer>(resultString[i], resultNodeId[i]));
    }
    return result;
  }

  public List<Pair<Integer, Integer>> getTrunkSpans() {
    if (this.handle == 0) {
      throw new IllegalStateException();
    }
    final int latticeSize = this.getSize();
    int[] leftTrunks = new int[latticeSize];
    int[] rightTrunks = new int[latticeSize];
    List<Pair<Integer, Integer>> result = new ArrayList<>();
    jniGetTrunkSpans(this.handle, leftTrunks, rightTrunks);
    for (int i = 0; i < latticeSize; ++i) {
      result.add(new Pair<Integer, Integer>(leftTrunks[i], rightTrunks[i]));
    }
    return result;
  }

  public List<SearchIndexNode> dumpForSearchIndex() {
    if (this.handle == 0) {
      throw new IllegalStateException();
    }
    int capacity = jniGetRequiredCapacity(this.handle);
    String[] texts = new String[capacity];
    int[] offsetStarts = new int[capacity];
    int[] offsetEnds = new int[capacity];
    int[] increments = new int[capacity];
    int[] lengths = new int[capacity];
    int s = jniDumpForSearchIndex(this.handle, texts, offsetStarts, offsetEnds, increments, lengths);
    List<SearchIndexNode> result = new ArrayList<>();
    for (int i = 0; i < s; ++i) {
      result.add(new SearchIndexNode(texts[i], offsetStarts[i], offsetEnds[i], increments[i], lengths[i]));
    }
    return result;
  }

  private native void jniDelete(long handle);
  private native int jniGetSize(long handle);
  private native int jniGetRequiredCapacity(long handle);
  private static native long jniNewFromBytes(byte[] data);
  private native byte[] jniToBytes(long handle);
  private native String jniDumpDot(long handle, boolean isNumbered);
  private native int jniGetTrunkSpan(long handle, String[] pathString, int[] pathNodeId, String[] resultString, int[] resultNodeId);
  private native void jniGetTrunkSpans(long handle, int[] leftTrunks, int[] rightTrunks);
  private native int jniDumpForSearchIndex(long handle, String[] texts, int[] offsetStarts, int[] offsetEnds, int[] increments, int[] lengths);

  static {
    JNILoader.loadLibrary();
  }

}
