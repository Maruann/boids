#include "flock.hpp"
#include "simulation.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

  // questa funzione evolve, diversa da quella in flock.hpp, fa evolvere lo
   // stormo e ritorna il vettore flock (quello composto dai boids). La funzione
   // asSeconds converte il valore di delta_t in secondi ( se vedi sotto,
   // inizializiamo delta_t a un millisecondo, quindi in pratica questa funzione
   // pone il valore di dt pari a 0.001)
    

int main() {

  Flock stormo{0.01, 0.01, 0.01};
  stormo.fill(10);

  auto const delta_t{sf::milliseconds(1)};
  int const fps = 25;
  int const steps_per_evolution{1000 / fps};

  unsigned const int display_width = 1920;  // larghezza quadrato
  unsigned const int display_height =
      1080;  // altezza quadrato
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
    //std::cout << stormo.get_flock()[0].get_x() << "   "<< stormo.get_flock()[0].get_vx() << '\n' ;
    update(stormo, steps_per_evolution, delta_t);

    for (auto& boid : stormo.get_flock()) {
      circ.setPosition(boid.get_x(), boid.get_y());

      window.draw(circ);  // disegna sull' oggetto window, ma non "displaya"
                          // ancora la window
    }

    window.display();  // adesso displaya
  }
}

// in sostanza, avendo inizializzato il cerchio prima, ogni ciclo, per
// ogni boid, lo sposto (il cerchio) in corrispondenza della posizione del boid
// e lo disegno
