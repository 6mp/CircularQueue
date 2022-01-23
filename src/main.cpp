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

	Test(const Test& other) { std::printf("%s called\n", __PRETTY_FUNCTION__); }
	Test& operator=(const Test& other) { std::printf("%s called\n", __PRETTY_FUNCTION__); }


	Test(Test&& other) noexcept {
		std::printf("%s called\n", __PRETTY_FUNCTION__);
		std::swap(this->m_x, other.m_x);
		other.m_x = 0;
	}
	Test& operator=(Test&& other) noexcept {
		std::printf("%s called\n", __PRETTY_FUNCTION__);
		std::swap(this->m_x, other.m_x);
		other.m_x = 0;
		return *this;
	}

	int m_x{0};
};

auto main() -> int {
	auto q = CircularQueue<Test, 10>{};

	auto gg = Test{69};

	q.push(gg);
	q.push(Test{11});
	q.emplace(70);
	std::cout << gg.m_x << std::endl;

	for (const auto& a : q) {
		std::cout << a.m_x << std::endl;
	}
}
