//
// Created by matt on 1/15/22.
//

#include <cstdio>
#include <CircularQueue.hpp>
#include <ranges>
#include <iostream>

struct Test {
	Test() { std::printf("default ctor called\n"); }
	explicit Test(int x) : m_x(x) { std::printf("specialized ctor called\n"); }
	~Test() { std::printf("dtor called\n"); }

	Test(const Test& other) {
		std::printf("copy ctor called\n");
		this->m_x = other.m_x;
	}
	auto operator=(const Test& other) -> Test& {
		std::printf("copy assignment called\n");
		this->m_x = other.m_x;
		return *this;
	}


	Test(Test&& other) noexcept {
		std::printf("move ctor called\n");
		std::swap(this->m_x, other.m_x);
		other.m_x = 0;
	}
	auto operator=(Test&& other) noexcept -> Test& {
		std::printf("move assignment called\n");
		std::swap(this->m_x, other.m_x);
		other.m_x = 0;
		return *this;
	}

	int m_x{0};
};

auto main() -> int {
	auto q = CircularQueue<Test, 10>{Test{3}};
	q.push(Test{4});
	auto bb = Test{5};
	q.push(bb);
	q.emplace(6);

	for (const auto& val : q)
		std::cout << val.m_x << std::endl;

	std::vector<int> z{1, 2, 3, 4, 5, 6};
	auto h = CircularQueue<int, 10>{z.begin(), z.end()};
	for (const auto& val : h)
		std::cout << val << std::endl;
}
