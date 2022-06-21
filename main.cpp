#include <iostream>

#include "graphics.hpp"
#include "simulation.hpp"

int main() {

  Flock stormo{5., 5., 5.};
  stormo.fill(200);
  graphics(stormo);

} 