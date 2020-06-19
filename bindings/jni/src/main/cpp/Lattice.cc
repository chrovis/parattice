#include "parattice_Lattice.h"

#include <parattice.hh>

#include "internal.hh"

namespace parattice {

  namespace jni {

    extern "C" {

      JNIEXPORT void JNICALL Java_parattice_Lattice_jniDelete(JNIEnv*, jobject, jlong handle) {
        delete to_object_ptr<JNILatticeWrapper>(handle);
      }

      JNIEXPORT jint JNICALL Java_parattice_Lattice_jniGetSize(JNIEnv*, jobject, jlong handle) {
        return static_cast<jint>(to_object<JNILatticeWrapper>(handle).lattice_.get_size());
      }

      JNIEXPORT jint JNICALL Java_parattice_Lattice_jniGetRequiredCapacity(JNIEnv*, jobject, jlong handle) {
        return static_cast<jint>(to_object<JNILatticeWrapper>(handle).lattice_.get_required_capacity());
      }

      JNIEXPORT jlong JNICALL Java_parattice_Lattice_jniNewFromBytes(JNIEnv* env, jclass, jbyteArray bytes) {
        JNIByteArrayAccess bytes_access(env, bytes);
        return to_jlong(new JNILatticeWrapper(Lattice::from_bytes(reinterpret_cast<std::uint8_t*>(bytes_access.data()), bytes_access.size()), std::move(bytes_access)));
      }

      JNIEXPORT jbyteArray JNICALL Java_parattice_Lattice_jniToBytes(JNIEnv* env, jobject, jlong handle) {
        const auto data = to_object<JNILatticeWrapper>(handle).lattice_.to_bytes();
        jbyteArray bytes = env->NewByteArray(static_cast<jsize>(data.size()));
        JNIByteArrayAccess bytes_access(env, bytes);
        std::memcpy(bytes_access.data(), data.data(), sizeof(std::uint8_t) * data.size());
        return bytes;
      }

      JNIEXPORT jstring JNICALL Java_parattice_Lattice_jniDumpDot(JNIEnv* env, jobject, jlong handle, jboolean is_numbered) {
        return env->NewStringUTF(to_object<JNILatticeWrapper>(handle).lattice_.dump_dot(is_numbered).c_str());
      }

      JNIEXPORT jint JNICALL Java_parattice_Lattice_jniGetTrunkSpan(JNIEnv* env, jobject, jlong handle, jobjectArray path_string, jintArray path_node_id, jobjectArray result_string, jintArray result_node_id) {
        const JNIObjectArrayAccess path_string_access(env, path_string);
        const JNIIntArrayAccess path_node_id_access(env, path_node_id);
        JNIObjectArrayAccess result_string_access(env, result_string);
        JNIIntArrayAccess result_node_id_access(env, result_node_id);
        std::vector<std::pair<std::string, std::size_t>> path_vector;
        path_vector.reserve(path_string_access.size());
        for (std::size_t i = 0; i < path_string_access.size(); ++i) {
          const JNIStringAccess str_access(env, static_cast<jstring>(path_string_access.get(i)));
          path_vector.emplace_back(str_access.get_string(), path_node_id_access.data()[i]);
        }
        const auto new_path = to_object<JNILatticeWrapper>(handle).lattice_.get_trunk_span(path_vector);
        for (std::size_t i = 0; i < new_path.size(); ++i) {
          result_string_access.set(i, env->NewStringUTF(new_path.at(i).first.c_str()));
          result_node_id_access.data()[i] = static_cast<jint>(new_path.at(i).second);
        }
        return static_cast<jint>(new_path.size());
      }

      JNIEXPORT void JNICALL Java_parattice_Lattice_jniGetTrunkSpans(JNIEnv* env, jobject, jlong handle, jintArray left_trunks, jintArray right_trunks) {
        const auto trunk_spans = to_object<JNILatticeWrapper>(handle).lattice_.get_trunk_spans();
        JNIIntArrayAccess left_trunks_access(env, left_trunks);
        JNIIntArrayAccess right_trunks_access(env, right_trunks);
        for (std::size_t i = 0; i < trunk_spans.size(); ++i) {
          left_trunks_access.data()[i] = static_cast<jint>(trunk_spans[i].first);
          right_trunks_access.data()[i] = static_cast<jint>(trunk_spans[i].second);
        }
      }

      JNIEXPORT jint JNICALL Java_parattice_Lattice_jniDumpForSearchIndex(JNIEnv* env, jobject, jlong handle, jobjectArray texts, jintArray offset_starts, jintArray offset_ends, jintArray increments, jintArray lengths) {
        const auto search_index_nodes = to_object<JNILatticeWrapper>(handle).lattice_.dump_for_search_index();
        JNIObjectArrayAccess texts_access(env, texts);
        JNIIntArrayAccess offset_starts_access(env, offset_starts);
        JNIIntArrayAccess offset_ends_access(env, offset_ends);
        JNIIntArrayAccess increments_access(env, increments);
        JNIIntArrayAccess lengths_access(env, lengths);
        for (std::size_t i = 0; i < search_index_nodes.size(); ++i) {
          texts_access.set(i, env->NewStringUTF(search_index_nodes[i].text.c_str()));
          offset_starts_access.data()[i] = static_cast<jint>(search_index_nodes[i].offset_start);
          offset_ends_access.data()[i] = static_cast<jint>(search_index_nodes[i].offset_end);
          increments_access.data()[i] = static_cast<jint>(search_index_nodes[i].increment);
          lengths_access.data()[i] = static_cast<jint>(search_index_nodes[i].length);
        }
        return static_cast<jint>(search_index_nodes.size());
      }

    }

  }

}
