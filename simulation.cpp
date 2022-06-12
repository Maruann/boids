#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

#include "flock.hpp"

auto evolve(Flock& flock, int steps_per_evolution, sf::Time delta_t) {
  double const dt{delta_t.asSeconds()};

  for (int s{0}; s != steps_per_evolution; ++s) {
    evolve(flock, dt);
  }

  return flock.get_flock();
}  // questa funzione evolve, diversa da quella in flock.hpp, fa evolvere lo
   // stormo e ritorna il vettore flock (quello composto dai boids). La funzione
   // asSeconds converte il valore di delta_t in secondi ( se vedi sotto,
   // inizializiamo delta_t a un millisecondo, quindi in pratica questa funzione
   // pone il valore di dt pari a 0.001)
   

int main() {
  for (int i = 1; i<10;++i){std::cout<<"hello, world" <<"1n";};

  Flock stormo{0.5, 0.5, 0.5};
  stormo.fill(1);

  auto const delta_t{sf::milliseconds(1)};
  int const fps = 30;
  int const steps_per_evolution{1000 / fps};

  unsigned const int display_width = 100;  // larghezza quadrato
  unsigned const int display_height =
      100;  // altezza quadrato
            //(suggerisco almeno 5 volte tanto per entrambi)

  sf::RenderWindow window(sf::VideoMode(display_width, display_height),
                          "BOOOIDZZZZZ");

  window.setFramerateLimit(fps);

  sf::CircleShape circ{5.0f};
  circ.setFillColor(sf::Color::Blue);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type ==
          sf::Event::Closed) {  // if che serve per chiudere la finestre quando
                                // si preme sulla "x" in alto a destra
        window.close();
      }
    }

    window.clear(sf::Color::White);

    auto const state = evolve(stormo, steps_per_evolution, delta_t);

    for (auto& boid : state) {
      circ.setPosition(boid.get_x(), boid.get_y());

      window.draw(circ);  // disegna sull' oggetto window, ma non "displaya"
                          // ancora la window
    }

    window.display();  // adesso display
  }
}

// in sostanza, avendo inizializzato il cerchio prima, ogni ciclo, per
// ogni boid, lo sposto (il cerchio) in corrispondenza della posizione del boid
// e lo disegno
