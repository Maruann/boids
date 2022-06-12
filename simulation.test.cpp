#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>

#include "doctest.h"
#include "flock.hpp"

TEST_CASE("Test riempimento flock")
{
  Flock flock{1, 1, 1};
  flock.fill(10);
  SUBCASE("Controllo che siano 10 boids")
  {
    std::cout << flock.get_flock().size() << '\n'; // WE GOOD BOIDDDDSSS
    CHECK(flock.get_flock().size() == 10);
  }

  SUBCASE("stampo posizioni x e y di ogni boid")
  { // nota, queste posizioni  sono "sbagliate"
    for (Boid boid : flock.get_flock()) {
      std::cout << boid.get_x() << "  " << boid.get_y() << '\n';
    }
    std::cout << "fine test 1"
              << '\n'; // SUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU
                       // funziona bene, i numeri sono random
  }

  SUBCASE("vediamo se la posizione del centro di massa è calcolata bene")
  {
    flock.upt_xc();
    std::cout << "la posizione del centro di massa è: " << flock.get_xc_x()
              << "     " << flock.get_xc_y() << '\n';
    for (Boid boid :
         flock.get_flock()) { // stampa la posizione del singoli boids per
                              // permettere un calcolo a manina
      std::cout << boid.get_x() << "  " << boid.get_y() << '\n';
    }
  }

  SUBCASE("vediamo se la velocità repulsiva di un boid va bene")
  {
    std::cout << flock.vx_repulsive(1000., 0)
              << '\n'; // stampa la velocità repulsiva sul singolo boid
                       // considerando tutti quelli presenti
    for (Boid boid : flock.get_flock()) {
      std::cout << boid.get_x() << "  " << boid.get_y()
                << '\n'; // stampa le posizioni dei singoli boids per un calcolo
                         // a manina
    }
  }
};