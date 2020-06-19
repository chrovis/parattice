#ifndef PARATTICE_CXX_H
#define PARATTICE_CXX_H

#include <cstring>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <utility>

extern "C" {
  char* parattice_free_string(char* s);
  char* parattice_free_bytes(std::uint8_t* bytes, std::size_t length);
  void* parattice_parattice_new(const char*** const* dict);
  void parattice_parattice_free(void* parattice);
  void* parattice_parattice_get_lattice(void const* parattice, const char* const* words, std::size_t length, bool shrink, std::size_t max_depth);
  void parattice_lattice_free(void* parattice);
  std::size_t parattice_lattice_get_size(void const* lattice);
  std::size_t parattice_lattice_get_required_capacity(void const* lattice);
  void* parattice_lattice_new_from_bytes(std::uint8_t const* data, const std::size_t length);
  std::uint8_t* parattice_lattice_to_bytes(void const* lattice, std::size_t* length);
  char* parattice_lattice_dump_dot(void const* lattice, bool is_numbered);
  std::size_t parattice_lattice_get_trunk_span(void const* lattice, const char* const* edge_labels, std::size_t const* node_ids, std::size_t length, const char** new_edge_labels, std::size_t* new_edge_label_length, std::size_t* new_node_ids);
  void parattice_lattice_get_trunk_spans(void const* lattice, std::size_t* trunk_lefts, std::size_t* trunk_rights);
  std::size_t parattice_lattice_dump_for_search_index(void const* lattice, const char** texts, std::size_t* text_lengths, std::size_t* offset_starts, std::size_t* offset_ends, std::size_t* increments, std::size_t* lengths);
  void* parattice_lattice_kmp_new(const char* const* pattern, std::size_t length);
  void parattice_lattice_kmp_free(void* latticekmp);
  void* parattice_lattice_kmp_search(void const* latticekmp, void const* lattice);
  void parattice_lattice_kmp_free_result(void* results);
  std::size_t parattice_lattice_kmp_results_size(void const* results);
  std::size_t parattice_lattice_kmp_result_length(void const* results, std::size_t index);
  void parattice_lattice_kmp_result_nodes(void const* results, std::size_t index, std::size_t* nodes);
  void parattice_lattice_kmp_result_edge_labels(void const* results, std::size_t index, const char** edge_labels, std::size_t* edge_label_length);
}

namespace parattice {

  struct search_index_node {
    std::string text;
    std::size_t offset_start;
    std::size_t offset_end;
    std::size_t increment;
    std::size_t length;
  };

  class Lattice {

    Lattice(const Lattice&) = delete;

    public:

    Lattice(): ptr_(nullptr), words_({}), data_({}) {}

    Lattice(Lattice&&) = default;

    explicit Lattice(const std::vector<std::string>& words)
      : ptr_(nullptr), words_(words), data_({}) {}

    explicit Lattice(std::vector<std::string>&& words)
      : ptr_(nullptr), words_(std::forward<std::vector<std::string>>(words)), data_({}) {}

    explicit Lattice(const std::vector<std::uint8_t>& data)
      : ptr_(nullptr), words_({}), data_(data) {}

    explicit Lattice(std::vector<std::uint8_t>&& data)
      : ptr_(nullptr), words_({}), data_(std::forward<std::vector<std::uint8_t>>(data)) {}

    std::size_t get_size() const {
      return parattice_lattice_get_size(ptr_.get());
    }

    std::size_t get_required_capacity() const {
      return parattice_lattice_get_required_capacity(ptr_.get());
    }

    std::string dump_dot(bool is_numbered) const {
      char* s = parattice_lattice_dump_dot(ptr_.get(), is_numbered);
      std::string cpp_str(s);
      parattice_free_string(s);
      return cpp_str;
    }

    std::vector<std::pair<std::string, std::size_t>> get_trunk_span(const std::vector<std::pair<std::string, std::size_t>>& path) const {
      std::vector<const char*> edge_labels;
      std::vector<std::size_t> node_ids;
      edge_labels.reserve(path.size());
      node_ids.reserve(path.size());
      for (auto& edge : path) {
        edge_labels.emplace_back(edge.first.c_str());
        node_ids.emplace_back(edge.second);
      }
      const std::size_t lattice_size = get_size();
      std::vector<const char*> new_edge_labels(lattice_size);
      std::vector<std::size_t> new_edge_label_length(lattice_size);
      std::vector<std::size_t> new_node_ids(lattice_size);
      const std::size_t s = parattice_lattice_get_trunk_span(ptr_.get(), edge_labels.data(), node_ids.data(), path.size(), new_edge_labels.data(), new_edge_label_length.data(), new_node_ids.data());
      std::vector<std::pair<std::string, std::size_t>> result;
      result.reserve(s);
      for (std::size_t i = 0; i < s; ++i) {
        result.emplace_back(std::string(new_edge_labels[i], new_edge_label_length[i]), new_node_ids[i]);
      }
      return result;
    }

    std::vector<std::pair<std::size_t, std::size_t>> get_trunk_spans() const {
      const std::size_t lattice_size = get_size();
      std::vector<std::pair<std::size_t, std::size_t>> result;
      result.reserve(lattice_size);
      std::vector<std::size_t> trunk_lefts(lattice_size);
      std::vector<std::size_t> trunk_rights(lattice_size);
      parattice_lattice_get_trunk_spans(ptr_.get(), trunk_lefts.data(), trunk_rights.data());
      for (std::size_t i = 0; i < lattice_size; ++i) {
        result.emplace_back(trunk_lefts[i], trunk_rights[i]);
      }
      return result;
    }

    std::vector<search_index_node> dump_for_search_index() const {
      const std::size_t capacity = get_required_capacity();
      std::vector<const char*> texts(capacity);
      std::vector<std::size_t> text_lengths(capacity);
      std::vector<std::size_t> offset_starts(capacity);
      std::vector<std::size_t> offset_ends(capacity);
      std::vector<std::size_t> increments(capacity);
      std::vector<std::size_t> lengths(capacity);
      const std::size_t s = parattice_lattice_dump_for_search_index(ptr_.get(), texts.data(), text_lengths.data(), offset_starts.data(), offset_ends.data(), increments.data(), lengths.data());
      std::vector<search_index_node> result;
      result.reserve(s);
      for (std::size_t i = 0; i < s; ++i) {
        result.emplace_back(search_index_node {
            std::string(texts[i], text_lengths[i]),
            offset_starts[i],
            offset_ends[i],
            increments[i],
            lengths[i],
            });
      }
      return result;
    }

    std::vector<std::uint8_t> to_bytes() const {
      std::size_t length;
      std::uint8_t* data = parattice_lattice_to_bytes(ptr_.get(), &length);
      std::vector<std::uint8_t> result(length);
      std::memcpy(result.data(), data, sizeof(std::uint8_t) * length);
      parattice_free_bytes(data, length);
      return result;
    }

    static Lattice from_bytes(const std::vector<std::uint8_t>& data) {
      Lattice lattice(data);
      lattice.ptr_ = std::unique_ptr<void, std::function<void(void*)>>(parattice_lattice_new_from_bytes(lattice.data_.data(), lattice.data_.size()), parattice_lattice_free);
      return lattice;
    }

    static Lattice from_bytes(std::vector<std::uint8_t>&& data) {
      Lattice lattice(std::forward<std::vector<std::uint8_t>>(data));
      lattice.ptr_ = std::unique_ptr<void, std::function<void(void*)>>(parattice_lattice_new_from_bytes(lattice.data_.data(), lattice.data_.size()), parattice_lattice_free);
      return lattice;
    }

    static Lattice from_bytes(std::uint8_t const* data, std::size_t size) {
      Lattice lattice;
      lattice.ptr_ = std::unique_ptr<void, std::function<void(void*)>>(parattice_lattice_new_from_bytes(data, size), parattice_lattice_free);
      return lattice;
    }

    public:

    std::unique_ptr<void, std::function<void(void*)>> ptr_;
    std::vector<std::string> words_;
    std::vector<std::uint8_t> data_;

  };

  class PaRattice {

    PaRattice() = delete;
    PaRattice(const PaRattice&) = delete;

    private:

    void init() {
      const char**** dict_c = new const char***[dict_.size() + 1];
      for (std::size_t i = 0; i < dict_.size(); ++i) {
        dict_c[i] = new const char**[dict_[i].size() + 1];
        for (std::size_t j = 0; j < dict_[i].size(); ++j) {
          dict_c[i][j] = new const char*[dict_[i][j].size() + 1];
          for (std::size_t k = 0; k < dict_[i][j].size(); ++k) {
            dict_c[i][j][k] = dict_[i][j][k].c_str();
          }
          dict_c[i][j][dict_[i][j].size()] = nullptr;
        }
        dict_c[i][dict_[i].size()] = nullptr;
      }
      dict_c[dict_.size()] = nullptr;
      ptr_ = std::unique_ptr<void, std::function<void(void*)>>(parattice_parattice_new(dict_c), parattice_parattice_free);
      for (std::size_t i = 0; dict_c[i] != nullptr; ++i) {
        for (std::size_t j = 0; dict_c[i][j] != nullptr; ++j) {
          delete[] dict_c[i][j];
        }
        delete[] dict_c[i];
      }
      delete[] dict_c;
    }

    public:

    explicit PaRattice(const std::vector<std::vector<std::vector<std::string>>>& dict)
      : ptr_(nullptr), dict_(dict) {
        init();
      }

    explicit PaRattice(std::vector<std::vector<std::vector<std::string>>>&& dict)
      : ptr_(nullptr), dict_(std::forward<std::vector<std::vector<std::vector<std::string>>>>(dict)) {
        init();
      }

    Lattice get_lattice(const std::vector<std::string>& words, bool shrink, std::size_t max_depth) const {
      Lattice lattice(words);
      std::vector<const char*> words_c;
      words_c.reserve(lattice.words_.size());
      for (std::size_t i = 0; i < lattice.words_.size(); ++i) {
        words_c.emplace_back(lattice.words_[i].c_str());
      }
      lattice.ptr_ = std::unique_ptr<void, std::function<void(void*)>>(parattice_parattice_get_lattice(ptr_.get(), words_c.data(), words_c.size(), shrink, max_depth), parattice_lattice_free);
      return lattice;
    }

    Lattice get_lattice(std::vector<std::string>&& words, bool shrink, std::size_t max_depth) const {
      Lattice lattice(std::forward<std::vector<std::string>>(words));
      std::vector<const char*> words_c;
      words_c.reserve(lattice.words_.size());
      for (std::size_t i = 0; i < lattice.words_.size(); ++i) {
        words_c.emplace_back(lattice.words_[i].c_str());
      }
      lattice.ptr_ = std::unique_ptr<void, std::function<void(void*)>>(parattice_parattice_get_lattice(ptr_.get(), words_c.data(), words_c.size(), shrink, max_depth), parattice_lattice_free);
      return lattice;
    }

    Lattice get_lattice(const std::vector<const char*>& words, bool shrink, std::size_t max_depth) const {
      Lattice lattice;
      lattice.ptr_ = std::unique_ptr<void, std::function<void(void*)>>(parattice_parattice_get_lattice(ptr_.get(), words.data(), words.size(), shrink, max_depth), parattice_lattice_free);
      return lattice;
    }

    Lattice get_lattice(std::vector<const char*>&& words, bool shrink, std::size_t max_depth) const {
      Lattice lattice;
      lattice.ptr_ = std::unique_ptr<void, std::function<void(void*)>>(parattice_parattice_get_lattice(ptr_.get(), words.data(), words.size(), shrink, max_depth), parattice_lattice_free);
      return lattice;
    }

    private:

    std::unique_ptr<void, std::function<void(void*)>> ptr_;
    std::vector<std::vector<std::vector<std::string>>> dict_;

  };

  class LatticeKMP {

    LatticeKMP() = delete;
    LatticeKMP(const LatticeKMP&) = delete;

    public:

    explicit LatticeKMP(const std::vector<std::string>& pattern)
      : ptr_(nullptr), pattern_(pattern) {
        std::vector<const char*> pattern_c;
        pattern_c.reserve(pattern_.size());
        for (auto& s : pattern_) {
          pattern_c.emplace_back(s.c_str());
        }
        ptr_ = std::unique_ptr<void, std::function<void(void*)>>(parattice_lattice_kmp_new(pattern_c.data(), pattern_c.size()), parattice_lattice_kmp_free);
      }

    explicit LatticeKMP(std::vector<std::string>&& pattern)
      : ptr_(nullptr), pattern_(std::forward<std::vector<std::string>>(pattern)) {
        std::vector<const char*> pattern_c;
        pattern_c.reserve(pattern_.size());
        for (auto& s : pattern_) {
          pattern_c.emplace_back(s.c_str());
        }
        ptr_ = std::unique_ptr<void, std::function<void(void*)>>(parattice_lattice_kmp_new(pattern_c.data(), pattern_c.size()), parattice_lattice_kmp_free);
      }

    std::vector<std::vector<std::pair<std::string, std::size_t>>> search(const Lattice& lattice) const {
      void* search_result = parattice_lattice_kmp_search(ptr_.get(), lattice.ptr_.get());
      std::vector<std::vector<std::pair<std::string, std::size_t>>> results;
      std::size_t s = parattice_lattice_kmp_results_size(search_result);
      results.reserve(s);
      for (std::size_t i = 0; i < s; ++i) {
        std::size_t l = parattice_lattice_kmp_result_length(search_result, i);
        std::vector<const char*> str_vec(l);
        std::vector<std::size_t> str_len_vec(l);
        std::vector<std::size_t> node_vec(l);
        parattice_lattice_kmp_result_edge_labels(search_result, i, str_vec.data(), str_len_vec.data());
        parattice_lattice_kmp_result_nodes(search_result, i, node_vec.data());
        std::vector<std::pair<std::string, std::size_t>> result;
        result.reserve(l);
        for (std::size_t j = 0; j < l; ++j) {
          result.emplace_back(std::string(str_vec[j], str_len_vec[j]), node_vec[j]);
        }
        results.emplace_back(result);
      }
      parattice_lattice_kmp_free_result(search_result);
      return results;
    }

    private:

    std::unique_ptr<void, std::function<void(void*)>> ptr_;
    std::vector<std::string> pattern_;

  };

} // namespace parattice

#endif // PARATTICE_CXX_H
