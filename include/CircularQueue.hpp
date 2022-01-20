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
#include <algorithm>
#include <vector>

template<typename Ty, std::size_t Size>
class CircularQueue {
	std::size_t m_head{};
	std::size_t m_tail{};
	std::vector<Ty> m_storage{};

	constexpr auto increment(std::size_t value) { return (value + 1) % Size; }

public:
	// has to be explicit to stop initializer_list ctor from being used
	constexpr explicit CircularQueue() : m_head(0), m_tail(0), m_storage(Size) {}

	constexpr CircularQueue(auto&&... values) {
		static_assert(sizeof...(values) <= Size, "parameter pack size must be <= Size");
	}

	template<std::input_iterator InputIt>
	constexpr CircularQueue(InputIt begin, InputIt end)
		: m_head(0), m_tail(std::distance(begin, end)), m_storage(Size) {
		static_assert(std::distance(begin, end) <= Size, "size is too large");
		std::copy(begin, end, std::back_inserter(m_storage));
	}

	template<std::ranges::range It>
	constexpr CircularQueue(It container) : m_head(0), m_tail(container.size() - 1), m_storage(Size) {
		static_assert(container.size() <= m_storage.size(), "size too large");
		std::ranges::copy(container, m_storage);
	}

	constexpr ~CircularQueue() { clear(); }

	//  modification
	constexpr auto push(Ty&& value) {
		std::construct_at(&m_storage[m_tail], std::forward<Ty>(value));
		m_tail = increment(m_tail);
	}

	template<typename... Args>
	constexpr auto push(Args&&... args) {
		static_assert(std::is_constructible_v<Ty, Args...>, "Ty must be constructable with args...");
		std::construct_at(&m_storage[m_tail], std::forward<Args>(args)...);
		m_tail = increment(m_tail);
	}

	constexpr auto pop() {
		std::destroy_at(&m_storage[m_head]);
		m_head = increment(m_head);
	}

	constexpr auto peek() {
		auto& value = m_storage[m_head];
		return value;
	}

	constexpr auto clear() {
		// I don't want to deallocate until I absolutely have to
		std::ranges::for_each(m_storage, [](auto& value) { value.~Ty(); });
		m_head = 0;
		m_tail = 0;
	}

	// iterators
};