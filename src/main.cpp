//
// Created by matt on 1/15/22.
//

#include <cstdio>
#include <CircularQueue.hpp>
#include <ranges>
#include <iostream>
#include <immintrin.h>

struct Test {
	Test() { std::printf("%s called\n", __PRETTY_FUNCTION__); }
	explicit Test(int x) : m_x(x) { std::printf("%s called\n", __PRETTY_FUNCTION__); }
	~Test() { std::printf("%s called\n", __PRETTY_FUNCTION__); }

	Test(const Test& other) = delete;
	Test& operator=(const Test& other) = delete;

	Test(Test&& other) noexcept {
		std::printf("%s called\n", __PRETTY_FUNCTION__);
		std::swap(this->m_x, other.m_x);
	}
	Test& operator=(Test&& other) noexcept {
		std::printf("%s called\n", __PRETTY_FUNCTION__);
		std::swap(this->m_x, other.m_x);
		return *this;
	}

	int m_x{0};
};

auto main() -> int {
	auto q = CircularQueue<Test, 10>{Test{1}, Test{2}, Test{3}, Test{4}, Test{5}};
	q.push(69);

	for (const auto& a : q) { std::cout << a.m_x << std::endl; }
}
