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
	~Test() { std::printf("dtor called\n"); }

	Test(const Test& other) = delete;
	Test& operator=(const Test& other) {
		std::printf("copy assignment operator\n");
		this->m_x = other.m_x;
		return *this;
	}

	Test(Test&& other) noexcept { std::printf("move ctor\n"); }
	Test& operator=(Test&& other) noexcept {
		std::printf("move assignment operator\n");
		std::swap(this->m_x, other.m_x);
		return *this;
	}

	int m_x{0};
};

auto main() -> int {
	//	int* b = new int[100];
	//	b[88] = 1;
	//
	//	std::printf("hello\n");
	//	auto q = CircularQueue<int, 10>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	//
	//	auto iterator = q.begin();
	//	std::cout << *iterator << std::endl;
	//	std::cout << *++iterator << std::endl;
	//	std::cout << *++iterator << std::endl;
	//	std::cout << *++iterator << std::endl;
	//	std::cout << *++iterator << std::endl;
	//	std::cout << *++iterator << std::endl;
	//	std::cout << *++iterator << std::endl;
	//	std::cout << *++iterator << std::endl;
	//	std::cout << *++iterator << std::endl;
	//	std::cout << *++iterator << std::endl;
	//	std::cout << *q.end() << std::endl;
	//
	//	std::printf("anmdjndlkahjdklasnd\n");
	//
	//
	//	for (auto end = q.end(); end != q.begin(); --end) {
	//		std::cout << *end << std::endl;
	//	}


	int i;

	for (i = 0; i < 1000; ++i) { free(malloc(1)); }

	return 0;
}
