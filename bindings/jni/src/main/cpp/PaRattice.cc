#include "parattice_PaRattice.h"

#include <parattice.hh>

#include "internal.hh"

namespace parattice {

  namespace jni {

    extern "C" {

      JNIEXPORT jlong JNICALL Java_parattice_PaRattice_jniNew(JNIEnv* env, jobject, jobjectArray arr_dict) {
        const JNIObjectArrayAccess arr_dict_access(env, arr_dict);
        return to_jlong(new PaRattice(to_string_vector_3d(env, arr_dict_access)));
      }

      JNIEXPORT void JNICALL Java_parattice_PaRattice_jniDelete(JNIEnv*, jobject, jlong handle) {
        delete to_object_ptr<PaRattice>(handle);
      }

      JNIEXPORT jlong JNICALL Java_parattice_PaRattice_jniGetLattice(JNIEnv* env, jobject, jlong handle, jobjectArray sentence, jboolean shrink, jint max_depth) {
        const JNIObjectArrayAccess sentence_access(env, sentence);
        std::vector<const char*> str_vec;
        std::vector<JNIStringAccess> str_access_vec;
        str_vec.reserve(sentence_access.size());
        str_access_vec.reserve(sentence_access.size());
        for (std::size_t i = 0; i < sentence_access.size(); ++i) {
          JNIStringAccess str_access(env, static_cast<jstring>(sentence_access.get(i)));
          str_vec.emplace_back(str_access.data());
          str_access_vec.emplace_back(std::move(str_access));
        }
        return to_jlong(new JNILatticeWrapper(to_object<PaRattice>(handle).get_lattice(str_vec, shrink, max_depth), std::move(str_access_vec)));
      }

    }  // end extern "C"

  }  // namespace jni

}  // namespace parattice
