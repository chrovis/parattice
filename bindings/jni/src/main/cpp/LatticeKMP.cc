#include "parattice_LatticeKMP.h"

#include <parattice.hh>

#include "internal.hh"

namespace parattice {

  namespace jni {

    extern "C" {

      JNIEXPORT jlong JNICALL Java_parattice_LatticeKMP_jniNew(JNIEnv* env, jobject, jobjectArray pattern) {
        const JNIObjectArrayAccess pattern_access(env, pattern);
        return to_jlong(new LatticeKMP(to_string_vector(env, pattern_access)));
      }

      JNIEXPORT void JNICALL Java_parattice_LatticeKMP_jniDelete(JNIEnv*, jobject, jlong handle) {
        delete to_object_ptr<LatticeKMP>(handle);
      }

      JNIEXPORT jlong JNICALL Java_parattice_LatticeKMP_jniSearch(JNIEnv*, jclass, jlong handle, jlong lattice_handle) {
        return to_jlong(new std::vector<std::vector<std::pair<std::string, std::size_t>>>(to_object<LatticeKMP>(handle).search(to_object<Lattice>(lattice_handle))));
      }

      JNIEXPORT jintArray JNICALL Java_parattice_LatticeKMP_jniSearchLength(JNIEnv* env, jclass, jlong result_handle) {
        const auto& result = to_object<std::vector<std::vector<std::pair<std::string, std::size_t>>>>(result_handle);
        jintArray sizes = env->NewIntArray(static_cast<jsize>(result.size()));
        JNIIntArrayAccess sizes_access(env, sizes);
        for(std::size_t i = 0; i < result.size(); ++i){
          sizes_access.data()[i] = static_cast<jint>(result[i].size());
        }
        return sizes;
      }

      JNIEXPORT void JNICALL Java_parattice_LatticeKMP_jniSearchGetDataAndFree(JNIEnv* env, jclass, jlong result_handle, jobjectArray result_string, jintArray result_node_id) {
        JNIObjectArrayAccess result_string_access(env, result_string);
        JNIIntArrayAccess result_node_id_access(env, result_node_id);
        auto results = to_object_ptr<std::vector<std::vector<std::pair<std::string, std::size_t>>>>(result_handle);
        std::size_t i = 0;
        for (auto& result : *results) {
          for (auto& node : result) {
            result_string_access.set(i, env->NewStringUTF(node.first.c_str()));
            result_node_id_access.data()[i] = static_cast<jint>(node.second);
            ++i;
          }
        }
        delete results;
      }

    }  // end extern "C"

  }  // namespace jni

}  // namespace parattice
