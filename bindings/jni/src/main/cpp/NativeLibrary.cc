#include <jni.h>

#include "internal.hh"

namespace parattice {

  namespace jni {

    extern "C" {

      jint JNI_OnLoad(JavaVM* vm, void*) {

        JNIEnv* env;
        if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_8) != JNI_OK) {
          return JNI_ERR;
        }

        return JNI_VERSION_1_8;
      }

      void JNI_OnUnload(JavaVM* vm, void*) {
        JNIEnv* env;
        vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_8);
      }

    }

  }

}
