//
// Created by Matt on 1/17/2022.
//

#pragma once
#include <type_traits>
#include <initializer_list>
#include <iterator>
#include <ranges>
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <bit>


template <typename Ty>
class CircularIterator {
public:
	using iterator_category = std::bidirectional_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = Ty;
	using pointer = Ty*;
	using reference = Ty&;

	explicit CircularIterator(pointer buffer, std::size_t pos, std::size_t displacement)
		: m_pos(pos), m_displacement(displacement), m_buffer(buffer) {}

	constexpr auto increment(std::size_t value) { return (value + 1) % 10; }
	constexpr auto decrement(std::size_t value) { return (value + 10 - 1) % 10; }

	constexpr auto& operator++() {
		m_pos = increment(m_pos);
		--m_displacement;
		return *this;
	}
	constexpr auto& operator--() {
		m_pos = decrement(m_pos);
		++m_displacement;
		return *this;
	}

	constexpr auto operator++(int) {
		auto temp = *this;
		++(*this);
		return temp;
	}

	constexpr auto operator--(int) {
		auto temp = *this;
		--(*this);
		return temp;
	}

	template <class Tx>
	constexpr bool operator==(const CircularIterator<Tx>& rhs) const noexcept {
		return rhs.m_buffer == this->m_buffer /*&& rhs.m_pos == this->m_pos*/ &&
			   rhs.m_displacement == this->m_displacement;
	}

	template <typename Tx>
	constexpr bool operator!=(const CircularIterator<Tx>& rhs) const noexcept {
		return !(operator==(rhs));
	}

	constexpr auto operator*() const { return m_buffer[m_pos]; }
	constexpr auto operator->() const { return std::addressof((m_buffer[m_pos])); }

	std::size_t m_pos{};
	std::size_t m_displacement{};
	pointer m_buffer{};
};

template <typename Ty, std::size_t Size>
class CircularQueue {
	std::size_t m_head{};
	std::size_t m_tail{};
	Ty m_storage[Size]{};

	constexpr auto increment(std::size_t value) { return (value + 1) % Size; }

public:
	//
	// Has to be explicit to stop paramter pack ctor from being used
	//
	constexpr explicit CircularQueue() : m_head(0), m_tail(0), m_storage() {}

	//
	// Parameter pack must be used due to std::initializer_list not functioning properly at compile time
	//
	template <typename... Args>
	constexpr explicit CircularQueue(Args&&... values) : m_head(0), m_storage() {
		static_assert(sizeof...(values) <= Size, "parameter pack size must be <= Size");
		((m_storage[m_tail++] = std::forward<Args>(values)), ...);
	}

	//	template <std::input_iterator InputIt>
	//	constexpr CircularQueue(InputIt begin, InputIt end)
	//		: m_head(0), m_tail(std::distance(begin, end) - 1), m_storage(Size) {
	//		static_assert(std::distance(begin, end) <= Size, "size is too large");
	//		std::copy(begin, end, std::back_inserter(m_storage));
	//	}

	//	template <std::ranges::range It>
	//	explicit CircularQueue(It container) : m_head(0), m_tail(container.size() - 1), m_storage(Size) {
	//		// static_assert(container.size() <= m_storage.size(), "size too large");
	//		std::ranges::copy(container, m_storage);
	//	}

	constexpr ~CircularQueue() = default;

	//
	//Copy assignment operator and copy constructor
	//
	constexpr CircularQueue(const CircularQueue& other) {}
	constexpr CircularQueue& operator=(const CircularQueue& other) { return *this; }

	//
	//Move assignment operator and move constructor
	//
	constexpr CircularQueue(CircularQueue&& other) noexcept {}
	constexpr CircularQueue& operator=(CircularQueue&& rhs) noexcept { return *this; }

	//
	//
	//
	constexpr auto push(Ty&& value) {
		const auto currentTail = m_tail;
		const auto nextTail = increment(currentTail);

		if (nextTail == m_head)
			return false;

		m_storage[currentTail] = std::forward<Ty>(value);
		m_tail = nextTail;
		return true;
	}

	//
	//
	//
	template <typename... Args>
	constexpr auto push(Args&&... args) {
		static_assert(std::is_constructible_v<Ty, Args...>, "Ty must be constructable with args...");

		const auto currentTail = m_tail;
		const auto nextTail = increment(currentTail);

		if (nextTail == m_head)
			return false;

		std::construct_at(std::addressof(m_storage[currentTail]), std::forward<Args>(args)...);
		m_tail = nextTail;
		return true;
	}

	//
	//
	//
	constexpr auto pop() {
		if (m_tail == m_head)
			return false;

		std::destroy_at(std::addressof(m_storage[m_tail]));
		m_head = increment(m_head);
		return true;
	}

	//
	//
	//
	constexpr auto peek() const { return m_storage[m_head]; }

	//
	//
	//
	constexpr auto clear() {
		for (auto i = m_head; i != m_tail; ++i) { std::destroy_at(std::addressof(m_storage[i])); }
		m_head = 0;
		m_tail = 0;
	}

	// iterators
	constexpr auto begin() { return CircularIterator<Ty>{m_storage, m_head, m_tail - m_head}; }
	constexpr auto end() { return CircularIterator<Ty>{m_storage, m_tail, 0}; }

	constexpr auto cbegin() { return CircularIterator<const Ty>{m_storage, m_head}; }
	constexpr auto cend() { return CircularIterator<const Ty>{m_storage, m_tail}; }

	constexpr auto rbegin() { return std::reverse_iterator{end()}; }
	constexpr auto rend() { return std::reverse_iterator{begin()}; }

	constexpr auto crbegin() { return std::reverse_iterator{cend()}; }
	constexpr auto crend() { return std::reverse_iterator{cbegin()}; }
};