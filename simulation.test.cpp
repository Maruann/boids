#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>

#include "doctest.h"
#include "flock.hpp"

TEST_CASE("Test riempimento flock") {
  Flock flock{1, 1, 1};
  flock.fill(10);

 /* std::cout << flock.get_flock().size() << '\n';  // WE GOOD BOIDDDDSSS
  CHECK(flock.get_flock().size() == 10);

  for (Boid boid : flock.get_flock()) {
    std::cout << boid.get_x() << "  " << boid.get_y() << '\n';
  }
  std::cout << "fine test 1" << '\n';
*/
  std::cout << flock.vx_repulsive(1000., 0)
            << '\n';  // stampa la velocità repulsiva sul singolo boid
                      // considerando tutti quelli presenti
  for (Boid boid : flock.get_flock()) {
    std::cout << boid.get_x() << "  " << boid.get_y()
              << '\n';  // stampa le posizioni dei singoli boids per un
                        // calcolo a manina
  }
  std::cout << flock.vx_alignment(0) << '\n';
  for (Boid boid : flock.get_flock()) {
    std::cout << boid.get_vx() << "  " << boid.get_vy()
              << '\n';  // stampa le velocità dei singoli boids per un
                        // calcolo a manina
  }
  std::cout << flock.vx_coesion(0) << '\n';
};