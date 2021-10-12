#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <string>
#include <ranges>
#include <span>
#include <unordered_map>
#include <unordered_set>
#include <iterator>

template<typename Card, size_t DECK_SIZE>
class ShufflableDeck {
  std::mt19937 rng = std::mt19937{ std::random_device{}() };
  std::array<Card, DECK_SIZE> deck_;
  int offset_ = 0;

public:
  ShufflableDeck(void) {
    std::iota(deck_.begin(), deck_.end(), 0);
    shuffle();
  }

  auto deck(void)->std::array<Card, DECK_SIZE> const& { return deck_; }

  void shuffle(void) {
    std::ranges::shuffle(deck_, rng);
    offset_ = 0;
  }

  bool is_dealable(size_t const n) { return offset_ + n <= deck_.size(); }

  template<size_t const N>
  auto deal() -> std::span<Card, N> {
    assert(is_dealable(N));
    auto cards = span<Card, N>{ &deck_[offset_], N };
    offset_ += N;
    return cards;
  }
};

template <typename T>
class ExtendedSet {
  std::unordered_map<T, int> map_;
  std::vector<T> vector_;
  std::random_device rd;
  std::mt19937 rng{ rd() };

public:
  ExtendedSet(void) {}

  size_t size(void) {
    return vector_.size();
  }

  bool insert(const T& x) {
    if (map_.count(x))
      return false;
    map_[x] = vector_.size();
    vector_.push_back(x);
    return true;
  }

  T erase_by_position(size_t n) {
    if ((0 <= n) && (n <= vector_.size())) {
      std::swap(vector_[n], vector_.back());
      std::swap(map_[vector_[n]], map_[vector_.back()]);
      map_.erase(vector_.back());
      T&& ret = std::move(vector_.back());
      vector_.pop_back();
      return ret;
    }
    std::string message = (
      "size=" + std::to_string(vector_.size()) + ", passed=" + std::to_string(n)
    );
    throw std::out_of_range(message);
  }

  size_t erase(const T& x) {
    if (map_.count(x)) {
      erase_by_position(map_[x]);
      return 1;
    }
    return 0;
  }

  size_t count(const T& x) {
    return map_.count(x);
  }

  T pop_random(void) {
    size_t index = rng() % vector_.size();
    return erase_by_position(index);
  }
};
