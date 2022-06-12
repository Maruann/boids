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

  /*SUBCASE("stam     po velocità x e y di ogni boid"){
      for(Boid boid : flock.get_flock()){
          std::cout << boid.get_vx() << "  " << boid.get_vy() << '\n';
      }
      std::cout << "fine test 1" <<'\n';
  }*/

  SUBCASE("vediamo se la posizione del centro di massa è calcolata bene")
  { // le posizioni stampate non corrispondono con quelle che considera
    flock
        .upt_xc(); // quando va a fare la media, non so perchè. Ma almeno sono consistenti quindi va "bene"
    std::cout << "la posizione del centro di massa è: " << flock.get_xc_x()
              << "     " << flock.get_xc_y() << '\n';
    for (Boid boid : flock.get_flock()) {
      std::cout << boid.get_x() << "  " << boid.get_y() << '\n';
    }
    flock.upt_xc();
    std::cout << "la posizione del centro di massa è: " << flock.get_xc_x()
              << "     " << flock.get_xc_y() << '\n';
  }

  SUBCASE("vediamo se la velocità repulsiva di un boid va bene")  
  {
    std::cout << flock.vx_repulsive(100, 0) << '\n';
  }
};