#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>

#include "doctest.h"
#include "flock.hpp"
#include "simulation.hpp"

TEST_CASE("Test riempimento flock") {
  Flock flock{1, 1, 1};
  flock.fill(1);
  std::cout << flock.get_flock()[0].get_x() << '\n';
  evolve(flock, 10);
  std::cout << flock.get_flock()[0].get_x() << '\n';
  std::cout << solve(flock, 10, flock.get_flock()[0]).get_x() << '\n';
};