//
// Created by matt on 1/15/22.
//
#include <catch2/catch_all.hpp>
#include <CircularQueue.hpp>

struct Test {
	Test() { std::printf("default ctor called\n"); }
	explicit Test(int x) : m_x(x) { std::printf("specialized ctor called\n"); }
	~Test() { std::printf("dtor called\n"); }

	Test(const Test& other) : m_x(other.m_x) { std::printf("copy ctor called\n"); }
	auto operator=(const Test& other) -> Test& {
		std::printf("copy assignment called\n");
		this->m_x = other.m_x;
		return *this;
	}


	Test(Test&& other) noexcept {
		std::printf("move ctor called\n");
		std::swap(this->m_x, other.m_x);
	}
	auto operator=(Test&& other) noexcept -> Test& {
		std::printf("move assignment called\n");
		std::swap(this->m_x, other.m_x);
		return *this;
	}

	int m_x{0};
};

TEST_CASE("constructors") {
	SECTION("default") {}

	SECTION("initializer_list") {}

	SECTION("input_iterator") {}

	SECTION("range") {}

	SECTION("move") {}

	SECTION("copy") {}
}

TEST_CASE("operators") {
	SECTION("move") {}

	SECTION("copy") {}
}

TEST_CASE("modifiers") {
	SECTION("push") {
		SECTION("lvalue") {}
		SECTION("rvalue") {}
	}

	SECTION("emplace") {}

	SECTION("pop") {}

	SECTION("clear") {}
}

TEST_CASE("viewers") {
	SECTION("peek") {}

	SECTION("iterators") {}
}
