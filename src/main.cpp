//
// Created by matt on 1/15/22.
//

#include <cstdio>
#include <CircularQueue.hpp>
#include <ranges>
#include <iostream>
#include <immintrin.h>

auto main() -> int {
  std::printf("hello\n");
  constexpr std::vector<int> aaa{1, 2, 3, 4};
  auto q = CircularQueue<int, 10>{};
  q.pop();
  q.pop();

  std::cout << q.peek() << std::endl;
}
