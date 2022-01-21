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


namespace detail {
	constexpr auto increment(std::size_t value) { return (value + 1) % 10; }
	constexpr auto decrement(std::size_t value) { return (value - 1) % 10; }
}// namespace detail

template <typename Ty>
class CircularIterator {
private:
	using pointer = Ty*;
	using reference = Ty&;

	std::size_t m_pos{};
	pointer m_buffer{};

public:
	constexpr CircularIterator(pointer buffer, std::size_t pos) : m_pos(pos), m_buffer(buffer) {}

	constexpr reference operator*() const noexcept { return (m_buffer[m_pos]); }
	constexpr pointer operator->() const noexcept { return std::addressof((m_buffer[m_pos])); }

	// Prefix operators
	constexpr auto& operator++() noexcept {
		m_pos = detail::increment(m_pos);
		return *this;
	}

	constexpr auto& operator--() noexcept {
		m_pos = detail::decrement(m_pos);
		return *this;
	}

	// Postfix operators
	constexpr auto operator++(int) noexcept {
		const auto temp = *this;
		m_pos = detail::increment(m_pos);
		return temp;
	}

	constexpr auto operator--(int) noexcept {
		auto temp = *this;
		m_pos = detail::decrement(m_pos);
		return temp;
	}

	constexpr auto operator==(CircularIterator& rhs) {
		return this->m_buffer == rhs.m_buffer && this->m_pos == rhs.m_pos;
	}

	constexpr auto operator!=(CircularIterator& rhs) { return !(operator==(rhs)); }
};

template <typename Ty, std::size_t Size>
class CircularQueue {
	std::size_t m_head{};
	std::size_t m_tail{};
	Ty* const m_storage{};

	constexpr auto& increment_head() {
		m_head = this->increment(m_head);
		return m_head;
	}
	constexpr auto& increment_tail() {
		m_tail = this->increment(m_tail);
		return m_tail;
	}


public:
	// Has to be explicit to stop initializer_list ctor from being used
	constexpr explicit CircularQueue()
		: m_head(0), m_tail(0), m_storage(std::bit_cast<Ty*>(std::malloc(sizeof(Ty) * Size))) {}

	// Paramter pack must be used due to std::initializer_list
	template <typename... Args>
	constexpr explicit CircularQueue(Args&&... values)
		: m_head(0), m_storage(std::bit_cast<Ty*>(std::malloc(sizeof(Ty) * Size))) {
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

	constexpr ~CircularQueue() { std::free(m_storage); }

	//Copy assignment operator and copy constructor
	constexpr CircularQueue(const CircularQueue& other) {}
	constexpr CircularQueue& operator=(const CircularQueue& other) { return *this; }

	//Move assignment operator and move constructor
	constexpr CircularQueue(CircularQueue&& other) noexcept {}
	constexpr CircularQueue& operator=(CircularQueue&& rhs) noexcept { return *this; }

	//  modification
	constexpr auto push(Ty&& value) {
		m_storage[m_tail] = std::forward<Ty>(value);
		m_tail = increment_tail();
	}

	template <typename... Args>
	constexpr auto push(Args&&... args) {
		static_assert(std::is_constructible_v<Ty, Args...>, "Ty must be constructable with args...");
		std::construct_at(std::addressof(m_storage[m_tail]), std::forward<Args>(args)...);
		m_tail = increment_tail();
	}

	constexpr auto pop() {
		std::destroy_at(std::addressof(m_storage[m_tail]));
		m_head = increment_head();
	}

	constexpr auto peek() {
		auto& value = m_storage[m_head];
		return value;
	}

	constexpr auto clear() {
		for (auto i = m_head; i != m_tail; ++i) { std::destroy_at(std::addressof(m_storage[i])); }
		m_head = 0;
		m_tail = 0;
	}

	// iterators
	constexpr auto begin() { return CircularIterator<Ty>{m_storage, m_head}; }
	constexpr auto end() { return CircularIterator<Ty>{m_storage, m_tail}; }

	constexpr auto cbegin() { return CircularIterator<const Ty>{m_storage, m_head}; }
	constexpr auto cend() { return CircularIterator<const Ty>{m_storage, m_tail}; }

	constexpr auto rbegin() { return std::reverse_iterator{begin()}; }
	constexpr auto rend() { return std::reverse_iterator{end()}; }

	constexpr auto crbegin() { return std::reverse_iterator{cbegin()}; }
	constexpr auto crend() { return std::reverse_iterator{cend()}; }
};