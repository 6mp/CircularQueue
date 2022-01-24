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

template <std::size_t N>
struct IndexWrapper {
	static constexpr auto increment(std::size_t value) { return (value + 1) % N; }
	static constexpr auto decrement(std::size_t value) { return (value + N - 1) % N; }
};

template <typename Ty, std::size_t Size>
struct CircularIterator {
	using iterator_category = std::bidirectional_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = Ty;
	using pointer = Ty*;
	using reference = Ty&;
	using wrapper = IndexWrapper<Size>;

	explicit CircularIterator(pointer buffer, std::size_t pos, std::size_t displacement)
		: m_pos(pos), m_displacement(displacement), m_buffer(buffer) {}

	constexpr auto operator++() -> auto& {
		m_pos = wrapper::increment(m_pos);
		--m_displacement;
		return *this;
	}
	constexpr auto operator--() -> auto& {
		m_pos = wrapper::decrement(m_pos);
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

	template <typename Tx, std::size_t N>
	constexpr auto operator==(const CircularIterator<Tx, N>& rhs) const noexcept -> bool {
		return rhs.m_buffer == this->m_buffer /*&& rhs.m_pos == this->m_pos*/ &&
			   rhs.m_displacement == this->m_displacement;
	}

	template <typename Tx, std::size_t N>
	constexpr auto operator!=(const CircularIterator<Tx, N>& rhs) const noexcept -> bool {
		return !(operator==(rhs));
	}

	constexpr auto operator*() const noexcept -> reference { return m_buffer[m_pos]; }
	constexpr auto operator->() const noexcept -> pointer { return std::addressof(m_buffer[m_pos]); }

	std::size_t m_pos{};
	std::size_t m_displacement{};
	pointer m_buffer{};
};

template <typename Ty, std::size_t Size>
class CircularQueue {
	std::size_t m_head{};
	std::size_t m_tail{};
	Ty* m_storage;
	using wrapper = IndexWrapper<Size>;

public:
	//
	// Default constructor, just allocate needed memory
	//
	constexpr explicit CircularQueue() : m_storage((Ty*)std::malloc(Size * sizeof(Ty))) {}

	//
	// Construct a CircularQueue from multiple values
	//
	CircularQueue(std::initializer_list<Ty> values)
		: m_tail(values.size()), m_storage((Ty*)std::malloc(Size * sizeof(Ty))) {
		if (values.size() >= Size) {
			throw std::runtime_error("std::initializer_list size is too large");
		}
		std::ranges::copy(values, m_storage);
	}

	//
	// Construct a CircularQueue from a container with begin and end iterators
	//
	template <std::input_iterator InputIt>
	CircularQueue(InputIt begin, InputIt end)
		: m_tail(std::distance(begin, end)), m_storage((Ty*)std::malloc(Size * sizeof(Ty))) {
		if ((std::size_t)std::distance(begin, end) >= Size) {
			throw std::runtime_error("container size is too large");
		}

		std::copy(begin, end, m_storage);
	}

	//
	// Construct a CircularQueue from a range container
	//
	template <std::ranges::range It>
	explicit CircularQueue(It container) : m_tail(container.size()), m_storage((Ty*)std::malloc(Size * sizeof(Ty))) {
		if (container.size() >= Size) {
			throw std::runtime_error("container size is too large");
		}
		std::ranges::copy(container, m_storage);
	}

	//
	// Call destructor of each item in the queue and free allocated memory
	//
	~CircularQueue() {
		if constexpr (std::is_destructible_v<Ty>) {
			clear();
		}
		std::free(m_storage);
	}

	//
	// Copy assignment operator and copy constructor
	//
	CircularQueue(const CircularQueue& other) {
		memcpy(&this->m_storage, &other.m_storage, sizeof(Ty) * Size);
		this->m_head = other.m_head;
		this->m_tail = other.m_tail;
	}
	auto operator=(const CircularQueue& rhs) -> CircularQueue& {
		if (rhs == this)
			return *this;

		memcpy(&this->m_storage, &rhs.m_storage, sizeof(Ty) * Size);
		this->m_head = rhs.m_head;
		this->m_tail = rhs.m_tail;
		return *this;
	}

	//
	// Move assignment operator and move constructor
	//
	CircularQueue(CircularQueue&& other) noexcept {
		std::swap(this->m_storage, other.m_storage);
		std::swap(this->m_head, other.m_head);
		std::swap(this->m_tail, other.m_tail);
	}
	auto operator=(CircularQueue&& rhs) noexcept -> CircularQueue& {
		std::swap(this->m_storage, rhs.m_storage);
		std::swap(this->m_head, rhs.m_head);
		std::swap(this->m_tail, rhs.m_tail);
		return *this;
	}


	//
	//
	//
	auto push(const Ty& value) -> bool {
		const auto currentTail = m_tail;
		const auto nextTail = wrapper::increment(currentTail);

		if (nextTail == m_head)
			return false;

		std::construct_at(std::addressof(m_storage[currentTail]), value);
		m_tail = nextTail;
		return true;
	}

	//
	//
	//
	auto push(Ty&& value) -> bool {
		const auto currentTail = m_tail;
		const auto nextTail = wrapper::increment(currentTail);

		if (nextTail == m_head)
			return false;

		std::construct_at(std::addressof(m_storage[currentTail]), std::move(value));
		m_tail = nextTail;
		return true;
	}

	//
	//
	//
	template <typename... Args>
	auto emplace(Args&&... args) -> bool {
		static_assert(std::is_constructible_v<Ty, Args...>, "Ty must be constructable with args...");

		const auto currentTail = m_tail;
		const auto nextTail = wrapper::increment(currentTail);

		if (nextTail == m_head)
			return false;

		std::construct_at(std::addressof(m_storage[currentTail]), std::forward<Args>(args)...);
		m_tail = nextTail;
		return true;
	}

	//
	// Call the destructor of the queue head and increment
	//
	auto pop() -> bool {
		if (m_tail == m_head)
			return false;

		std::destroy_at(std::addressof(m_storage[m_tail]));
		m_head = wrapper::increment(m_head);
		return true;
	}

	//
	// Return a reference to the head of the queue
	//
	auto peek() -> Ty& { return m_storage[m_head]; }

	//
	// Call the destructor of each item
	// Reset head and tail
	//
	auto clear() -> void {
		if constexpr (std::is_destructible_v<Ty>) {
			for (auto i = m_head; i != m_tail; ++i)
				std::destroy_at(std::addressof(m_storage[i]));
		}
		m_head = 0;
		m_tail = 0;
	}

	//
	// Iterators
	//
	constexpr auto begin() { return CircularIterator<Ty, Size>{m_storage, m_head, m_tail - m_head}; }
	constexpr auto end() { return CircularIterator<Ty, Size>{m_storage, m_tail, 0}; }

	constexpr auto cbegin() const { return CircularIterator<const Ty, Size>{m_storage, m_head, m_tail - m_head}; }
	constexpr auto cend() const { return CircularIterator<const Ty, Size>{m_storage, m_tail, 0}; }

	constexpr auto rbegin() { return std::reverse_iterator{end()}; }
	constexpr auto rend() { return std::reverse_iterator{begin()}; }

	constexpr auto crbegin() { return std::reverse_iterator{cend()}; }
	constexpr auto crend() { return std::reverse_iterator{cbegin()}; }
};