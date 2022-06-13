#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

#include "flock.hpp"
#include "simulation.hpp"
#include <iostream>

int main() {
  Flock stormo{0.01, 0.01, 0.01};
  stormo.fill(40);

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

  sf::ConvexShape convex;        //genero una forma geometrica come modello del singolo boid (unisco i 6 punti che genero sotto)
  convex.setFillColor(sf::Color::Blue);
  convex.setPointCount(6);

/*  convex.setPoint(0, sf::Vector2f(30,70));
  convex.setPoint(1, sf::Vector2f(0,100));
  convex.setPoint(2, sf::Vector2f(30,0));
  convex.setPoint(3, sf::Vector2f(60,100));*/

  convex.setPoint(0, sf::Vector2f(30,60));
  convex.setPoint(1, sf::Vector2f(10,70));
  convex.setPoint(2, sf::Vector2f(0,50));
  convex.setPoint(3, sf::Vector2f(30,0));
  convex.setPoint(4, sf::Vector2f(60,50));
  convex.setPoint(5, sf::Vector2f(50,70));

  convex.setScale(0.3, 0.3);

  convex.setOrigin(sf::Vector2f(30, 35));  //setto l'origine locale (punto attorno a cui ruota il singolo boid) 
  

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
    // std::cout << stormo.get_flock()[0].get_x() << "   "<<
    // stormo.get_flock()[0].get_vx() << '\n' ;
    update(stormo, steps_per_evolution, delta_t);

    for (auto& boid : stormo.get_flock()) {
      
      float angle = static_cast<float>(orientation(boid.get_vx(), boid.get_vy())) -90;
      convex.setRotation(-angle);
  
      convex.setPosition(boid.get_x(), boid.get_y());

      window.draw(convex);  // disegna sull' oggetto window, ma non "displaya"
                            // ancora la window
    }

    window.display();  // adesso displaya
  }
}

// in sostanza, avendo inizializzato il cerchio prima, ogni ciclo, per
// ogni boid, lo sposto (il cerchio) in corrispondenza della posizione del boid
// e lo disegno

