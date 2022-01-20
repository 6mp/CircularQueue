//
// Created by matt on 1/15/22.
//

#include <cstdio>
#include <CircularQueue.hpp>
#include <ranges>
#include <iostream>
#include <immintrin.h>

struct Test {
	Test() {
		std::printf("ctor called\n");
	}

	~Test() {
		std::printf("dtor called\n");
	}
};

auto main() -> int {
	std::printf("hello\n");
	auto q = CircularQueue<Test, 10>{Test{}, Test{}, Test{}, Test{}, Test{}};

	//std::cout << q.peek() << std::endl;
}
