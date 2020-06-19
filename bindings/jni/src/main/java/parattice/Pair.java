package parattice;

public class Pair<A extends Comparable<A>, B extends Comparable<B>> implements Comparable<Pair<A, B>> {
  public A first;
  public B second;

  public Pair(A first, B second) {
    this.first = first;
    this.second = second;
  }

  @Override
  public boolean equals(Object obj) {
    if (!(obj instanceof Pair)) {
      return false;
    }
    @SuppressWarnings("unchecked")
    Pair<A, B> other = (Pair<A, B>)obj;
    return
        (this.first == null ? other.first == null : this.first.equals(other.first)) &&
        (this.second == null ? other.second == null : this.second.equals(other.second));
  }

  @Override
  public int compareTo(Pair<A, B> other){
    int result = this.first.compareTo(other.first);
    if (result != 0) {
      return result;
    } else {
      return this.second.compareTo(other.second);
    }
  }

  @Override
  public int hashCode() {
    int f = this.first != null ? this.first.hashCode() : 0;
    int s = this.second != null ? this.second.hashCode() : 0;
    return f ^ s;
  }

  public String toString() {
    String f = this.first != null ? this.first.toString() : "(null)";
    String s = this.second != null ? this.second.toString() : "(null)";
    return "(" + f + ", " + s + ")";
  }
}
