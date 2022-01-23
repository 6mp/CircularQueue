//
// Created by matt on 1/15/22.
//

#include <cstdio>
#include <CircularQueue.hpp>
#include <ranges>
#include <iostream>
#include <immintrin.h>

struct Test {
	Test() { std::printf("default ctor called\n"); }
	explicit Test(int x) : m_x(x) { std::printf("specialized ctor called\n"); }
	~Test() { std::printf("dtor called\n"); }

	Test(const Test& other) {
		std::printf("copy ctor called\n");
		this->m_x = other.m_x;
	}
	Test& operator=(const Test& other) {
		std::printf("copy assignment called\n");
		this->m_x = other.m_x;
		return *this;
	}


	Test(Test&& other) noexcept {
		std::printf("move ctor called\n");
		std::swap(this->m_x, other.m_x);
		other.m_x = 0;
	}
	Test& operator=(Test&& other) noexcept {
		std::printf("move assignment called\n");
		std::swap(this->m_x, other.m_x);
		other.m_x = 0;
		return *this;
	}

	int m_x{0};
};

auto main() -> int {
	auto q = CircularQueue<Test, 10>{};
}
