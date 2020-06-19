#ifndef PARATTICE_INTERNAL_H_
#define PARATTICE_INTERNAL_H_

#include <jni.h>
#include <parattice.hh>

namespace parattice {

  namespace jni {

    class JNIStringAccess {
      JNIStringAccess() = delete;
      JNIStringAccess(const JNIStringAccess&) = delete;

      public:

      JNIStringAccess(JNIEnv* env, jstring string)
        : env_(env)
          , string_(string)
          , size_(env_->GetStringUTFLength(string_))
          , char_ptr_(env_->GetStringUTFChars(string_, 0)) {}

      JNIStringAccess(JNIStringAccess&& r) noexcept
        : env_(r.env_)
        , string_(r.string_)
        , size_(r.size_)
        , char_ptr_(r.char_ptr_) {
          r.env_ = nullptr;
          r.string_ = nullptr;
          r.size_ = 0;
          r.char_ptr_ = nullptr;
        }

      JNIStringAccess& operator=(JNIStringAccess&& r) noexcept {
        env_ = r.env_;
        string_ = r.string_;
        size_ = r.size_;
        char_ptr_ = r.char_ptr_;
        r.env_ = nullptr;
        r.string_ = nullptr;
        r.size_ = 0;
        r.char_ptr_ = nullptr;
        return *this;
      }

      ~JNIStringAccess() {
        if (env_) {
          env_->ReleaseStringUTFChars(string_, char_ptr_);
          env_->DeleteLocalRef(string_);
        }
      }

      inline const char* data() const { return char_ptr_; }
      inline jsize size() const { return size_; }
      inline std::string get_string() const {
        return std::string(char_ptr_);
      }

      private:
      JNIEnv* env_;
      jstring string_;
      jsize size_;
      const char* char_ptr_;

    };  // class JNIStringAccess

    class JNIByteArrayAccess {
      JNIByteArrayAccess(const JNIByteArrayAccess&) = delete;

      public:
      JNIByteArrayAccess()
        : env_(nullptr) {}

      JNIByteArrayAccess(JNIEnv* env, jbyteArray array)
        : env_(env)
          , array_(array)
          , size_(env_->GetArrayLength(array_))
          , data_(reinterpret_cast<jbyte*>(env_->GetPrimitiveArrayCritical(array_, nullptr))) {
          }

      JNIByteArrayAccess(JNIByteArrayAccess&& r) noexcept
        : env_(r.env_)
        , array_(r.array_)
        , size_(r.size_)
        , data_(r.data_) {
          r.env_ = nullptr;
          r.array_ = nullptr;
          r.size_ = 0;
          r.data_ = nullptr;
        }

      JNIByteArrayAccess& operator=(JNIByteArrayAccess&& r) noexcept {
        env_ = r.env_;
        array_ = r.array_;
        size_ = r.size_;
        data_ = r.data_;
        r.env_ = nullptr;
        r.array_ = nullptr;
        r.size_ = 0;
        r.data_ = nullptr;
        return *this;
      }

      ~JNIByteArrayAccess() {
        if (env_) {
          env_->ReleasePrimitiveArrayCritical(array_, data_, 0);
        }
      }

      inline std::size_t size() const { return static_cast<std::size_t>(size_); }
      inline jbyte* data() const {
        return data_;
      }
      inline jbyte* data() {
        return data_;
      }

      private:
      JNIEnv* env_;
      jbyteArray array_;
      jsize size_;
      jbyte* data_;

    };  // class JNIByteArrayAccess

    class JNIIntArrayAccess {
      JNIIntArrayAccess() = delete;
      JNIIntArrayAccess(const JNIIntArrayAccess&) = delete;

      public:
      JNIIntArrayAccess(JNIEnv* env, jintArray array)
        : env_(env)
          , array_(array)
          , size_(env_->GetArrayLength(array_))
          , data_(reinterpret_cast<jint*>(env_->GetPrimitiveArrayCritical(array_, nullptr))) {
          }

      JNIIntArrayAccess(JNIIntArrayAccess&& r) noexcept
        : env_(r.env_)
        , array_(r.array_)
        , size_(r.size_)
        , data_(r.data_) {
          r.env_ = nullptr;
          r.array_ = nullptr;
          r.size_ = 0;
          r.data_ = nullptr;
        }

      JNIIntArrayAccess& operator=(JNIIntArrayAccess&& r) noexcept {
        env_ = r.env_;
        array_ = r.array_;
        size_ = r.size_;
        data_ = r.data_;
        r.env_ = nullptr;
        r.array_ = nullptr;
        r.size_ = 0;
        r.data_ = nullptr;
        return *this;
      }

      ~JNIIntArrayAccess() {
        if (env_) {
          env_->ReleasePrimitiveArrayCritical(array_, data_, 0);
        }
      }
      inline std::size_t size() const { return static_cast<std::size_t>(size_); }
      inline jint* data() const {
        return data_;
      }
      inline jint* data() {
        return data_;
      }

      private:
      JNIEnv* env_;
      jintArray array_;
      jsize size_;
      jint* data_;

    };  // class JNIIntArrayAccess

    class JNIObjectArrayAccess {
      JNIObjectArrayAccess() = delete;
      JNIObjectArrayAccess(const JNIObjectArrayAccess&) = delete;

      public:
      JNIObjectArrayAccess(JNIEnv* env, jobjectArray array)
        : env_(env)
          , array_(array)
          , size_(env_->GetArrayLength(array_)) {
          }

      JNIObjectArrayAccess(JNIObjectArrayAccess&& r) noexcept
        : env_(r.env_)
        , array_(r.array_)
        , size_(r.size_) {
          r.env_ = nullptr;
          r.array_ = nullptr;
          r.size_ = 0;
        }

      JNIObjectArrayAccess& operator=(JNIObjectArrayAccess&& r) noexcept {
        env_ = r.env_;
        array_ = r.array_;
        size_ = r.size_;
        r.env_ = nullptr;
        r.array_ = nullptr;
        r.size_ = 0;
        return *this;
      }

      ~JNIObjectArrayAccess() {
        if (env_) {
          env_->DeleteLocalRef(array_);
        }
      }
      inline std::size_t size() const { return static_cast<std::size_t>(size_); }
      inline jobject get(std::size_t index) const {
        return env_->GetObjectArrayElement(array_, static_cast<jsize>(index));
      }
      inline void set(std::size_t index, jobject value) {
        return env_->SetObjectArrayElement(array_, static_cast<jsize>(index), value);
      }

      private:
      JNIEnv* env_;
      jobjectArray array_;
      jsize size_;

    };  // class JNIObjectArrayAccess

    class JNILatticeWrapper {
      JNILatticeWrapper() = delete;
      JNILatticeWrapper(const JNILatticeWrapper&) = delete;

      public:
      JNILatticeWrapper(Lattice&& lattice, std::vector<JNIStringAccess>&& sentence)
        : lattice_(std::forward<Lattice>(lattice))
          , sentence_(std::forward<std::vector<JNIStringAccess>>(sentence)) {}

      JNILatticeWrapper(Lattice&& lattice, JNIByteArrayAccess&& bytes)
        : lattice_(std::forward<Lattice>(lattice))
          , bytes_(std::forward<JNIByteArrayAccess>(bytes)) {}

      public:
      Lattice lattice_;

      private:
      std::vector<JNIStringAccess> sentence_;
      JNIByteArrayAccess bytes_;

    };  // class JNILatticeWrapper

    inline jlong to_jlong(void* instance) {
      return reinterpret_cast<jlong>(instance);
    }

    template<class T>
      inline T *to_object_ptr(jlong pointer) {
        return reinterpret_cast<T *>(pointer);
      }

    template<class T>
      inline T &to_object(jlong pointer) {
        return *reinterpret_cast<T *>(pointer);
      }

    inline std::vector<std::string> to_string_vector(JNIEnv *env, const JNIObjectArrayAccess& array) {
      std::vector<std::string> str_vec;
      str_vec.reserve(array.size());
      for (std::size_t i = 0; i < array.size(); ++i) {
        const JNIStringAccess str_access(env, static_cast<jstring>(array.get(i)));
        str_vec.emplace_back(str_access.get_string());
      }
      return str_vec;
    }

    inline std::vector<std::vector<std::vector<std::string>>> to_string_vector_3d(JNIEnv *env, const JNIObjectArrayAccess& array) {
      std::vector<std::vector<std::vector<std::string>>> vec;
      vec.reserve(array.size());
      for (std::size_t i = 0; i < array.size(); ++i) {
        const JNIObjectArrayAccess array2(env, static_cast<jobjectArray>(array.get(i)));
        vec.emplace_back();
        vec.back().reserve(array2.size());
        for (std::size_t j = 0; j < array2.size(); ++j) {
          const JNIObjectArrayAccess array3(env, static_cast<jobjectArray>(array2.get(j)));
          vec.back().emplace_back();
          vec.back().back().reserve(array3.size());
          for (std::size_t k = 0; k < array3.size(); ++k) {
            const JNIStringAccess str_access(env, static_cast<jstring>(array3.get(k)));
            vec.back().back().emplace_back(str_access.get_string());
          }
        }
      }
      return vec;
    }

  }  // namespace jni

}  // namespace parattice

#endif  // PARATTICE_INTERNAL_H_
