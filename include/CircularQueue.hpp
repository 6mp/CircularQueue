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

template <typename Ty, std::size_t Size>
class CircularQueue {
	std::size_t m_head{};
	std::size_t m_tail{};
	Ty* const m_storage{};

	constexpr auto increment(std::size_t value) { return (value + 1) % Size; }

public:
	// Has to be explicit to stop initializer_list ctor from being used
	constexpr explicit CircularQueue()
		: m_head(0), m_tail(0), m_storage(std::bit_cast<Ty*>(std::malloc(sizeof(Ty) * Size))) {}

	template <typename... Args>
	constexpr explicit CircularQueue(Args&&... values)
		: m_head(0), m_storage(std::bit_cast<Ty*>(std::malloc(sizeof(Ty) * Size))) {
		static_assert(sizeof...(values) <= Size, "parameter pack size must be <= Size");
		((m_storage[m_tail++] = std::forward<Args>(values)), ...);
		std::printf("current tail %llu\n", m_tail);
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
	CircularQueue(const CircularQueue& other) {}
	CircularQueue& operator=(const CircularQueue& other) { return *this; }

	//Move assignment operator and move constructor
	CircularQueue(CircularQueue&& other) noexcept {}
	CircularQueue& operator=(CircularQueue&& rhs) noexcept { return *this; }

	//  modification
	constexpr auto push(Ty&& value) {
		m_storage[m_tail] = std::forward<Ty>(value);
		m_tail = increment(m_tail);
	}

	template <typename... Args>
	constexpr auto push(Args&&... args) {
		static_assert(std::is_constructible_v<Ty, Args...>, "Ty must be constructable with args...");
		std::construct_at(&m_storage[m_tail], std::forward<Args>(args)...);
		m_tail = increment(m_tail);
	}

	constexpr auto pop() {
		std::destroy_at(&m_storage[m_tail]);
		m_head = increment(m_head);
	}

	constexpr auto peek() {
		auto& value = m_storage[m_head];
		return value;
	}

	constexpr auto clear() {
		for (auto i = m_head; i != m_tail; ++i) { std::destroy_at(m_storage[i]); }
		m_head = 0;
		m_tail = 0;
	}

	// iterators
};