#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>

#include "doctest.h"
#include "flock.hpp"
#include "simulation.hpp"

TEST_CASE("Test riempimento flock") {
  Flock flock{1, 1, 1};
  flock.fill(1);
};