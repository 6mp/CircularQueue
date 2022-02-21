//
// Created by matt on 1/15/22.
//
#include <catch2/catch_all.hpp>
#include <CircularQueue.hpp>

struct Test {
	Test() = default;
	explicit Test(int x) : m_x(x) {}
	~Test() = default;

	Test(const Test& other) : m_x(other.m_x) {}
	auto operator=(const Test& other) -> Test& {
		this->m_x = other.m_x;
		return *this;
	}


	Test(Test&& other) noexcept { std::swap(this->m_x, other.m_x); }
	auto operator=(Test&& other) noexcept -> Test& {
		std::swap(this->m_x, other.m_x);
		return *this;
	}

	int m_x{0};
};

TEST_CASE("Constructors") {
	SECTION("default") {
		auto instance = CircularQueue<Test, 10>{};

		REQUIRE(instance.size() == 0);
		REQUIRE(instance.max_size() == 10);
	}

	SECTION("initializer_list") {
		auto instance = CircularQueue<Test, 5>{Test{0}, Test{1}, Test{2}, Test{3}, Test{4}};

		REQUIRE(instance.size() == 5);
		REQUIRE(instance.max_size() == 5);
	}

	SECTION("input_iterator") {
		std::vector<Test> input_tests{Test{0}, Test{1}, Test{2}, Test{3}, Test{4}, Test{5}, Test{6}, Test{7}, Test{8}};
		auto instance = CircularQueue<Test, 10>{input_tests.begin(), input_tests.end()};

		REQUIRE(instance.size() == 9);
		REQUIRE(instance.max_size() == 10);
	}

	SECTION("range") {
		std::vector<Test> input_tests{Test{0}, Test{1}, Test{2}, Test{3},  Test{4},  Test{5},  Test{6},
									  Test{7}, Test{8}, Test{9}, Test{10}, Test{11}, Test{12}, Test{13}};
		auto instance = CircularQueue<Test, 15>{input_tests};

		REQUIRE(instance.size() == 14);
		REQUIRE(instance.max_size() == 15);
	}

	SECTION("move") {
		auto move_me = CircularQueue<Test, 3>{Test{0}, Test{1}, Test{2}};
		CircularQueue<Test, 3> new_queue = CircularQueue<Test, 3>{std::move(move_me)};

		REQUIRE(move_me.size() == 0);
		REQUIRE(new_queue.size() == 3);
	}

	SECTION("copy") {
	}
}

TEST_CASE("Operators") {
	SECTION("move") {}

	SECTION("copy") {}
}

TEST_CASE("Methods") {
	SECTION("push") {
		SECTION("lvalue") {}
		SECTION("rvalue") {}
	}

	SECTION("emplace") {}

	SECTION("pop") {}

	SECTION("clear") {}

	SECTION("peek") {}
}

TEST_CASE("Iterators") {}
