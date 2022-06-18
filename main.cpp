#include <iostream>

#include "flock.hpp"
#include "graphics.hpp"
#include "simulation.hpp"
#include "velocity.hpp"


int main() {
  Flock stormo{0.5, 0.5, 0.5};
  stormo.fill(100);
  graphics(stormo);

}