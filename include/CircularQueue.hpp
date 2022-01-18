//
// Created by Matt on 1/17/2022.
//

#pragma once
#include <type_traits>
#include <initializer_list>
#include <iterator>
#include <ranges>
#include <cstdlib>
#include <bit>
#include <iostream>

template <typename Ty, std::size_t Size>
class CircularQueue {
  std::size_t m_head{};
  std::size_t m_tail{};
  std::size_t m_size{};
  std::aligned_storage_t<sizeof(Ty), alignof(Ty)> m_storage[Size]{};

  constexpr auto increment(std::size_t value) { return (value + 1) % Size; }

 public:
  // has to be explicit to stop initializer_list ctor from being used
  constexpr explicit CircularQueue() : m_head(0), m_tail(0), m_size(0) {}

  constexpr CircularQueue(std::initializer_list<Ty> values) {
    static_assert(values.size() <= Size,
                  "values.size() must be less than total size");
  }

  template <std::input_iterator InputIt>
  constexpr CircularQueue(InputIt begin, InputIt end) {}

  template <std::ranges::range It>
  constexpr CircularQueue(It container) {}

  constexpr ~CircularQueue() { clear(); }

  //  modification
  constexpr auto push(Ty&& value) {
    new (&m_storage[m_tail]) Ty{std::forward<Ty>(value)};
    m_tail = increment(m_tail);
    ++m_size;
  }

  constexpr auto push(auto&&... args) {
    new (&m_storage[m_tail]) Ty{std::forward<decltype(args)>(args)...};
    m_tail = increment(m_tail);
    ++m_size;
  }

  constexpr auto pop() {
    std::destroy_at(std::launder(reinterpret_cast<Ty*>(&m_storage[m_head])));
    m_head = increment(m_head);
    --m_size;
  }

  constexpr auto clear() {
    while (m_size > 0) {
      pop();
    }
  }
};