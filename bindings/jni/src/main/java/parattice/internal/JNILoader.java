package parattice.internal;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.security.AccessController;
import java.security.PrivilegedAction;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.IOUtils;
import org.apache.commons.lang3.SystemUtils;

import parattice.PaRattice;

public class JNILoader {
  public static void loadLibrary() {
    try {
      InputStream in;
      File tempFile;
      if (SystemUtils.IS_OS_LINUX) {
        in = PaRattice.class.getResourceAsStream("platform/linux-amd64/libparattice.so");
        tempFile = File.createTempFile("libparattice", ".so");
      } else if (SystemUtils.IS_OS_MAC_OSX) {
        in = PaRattice.class.getResourceAsStream("platform/darwin-amd64/libparattice.dylib");
        tempFile = File.createTempFile("libparattice", ".dylib");
      } else {
        throw new IOException("Unsupported system");
      }
      OutputStream out = FileUtils.openOutputStream(tempFile);
      IOUtils.copy(in, out);
      in.close();
      out.close();
      AccessController.doPrivileged(new PrivilegedAction<Void>() {
        public Void run() {
          System.load(tempFile.toString());
          return null;
        }
      });
    } catch (IOException e) {
      System.err.println(e);
    }
  }
}
