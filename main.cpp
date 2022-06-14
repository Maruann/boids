
#include <iostream>

#include "flock.hpp"
#include "simulation.hpp"

// questa funzione evolve, diversa da quella in flock.hpp, fa evolvere lo
// stormo e ritorna il vettore flock (quello composto dai boids). La funzione
// asSeconds converte il valore di delta_t in secondi ( se vedi sotto,
// inizializiamo delta_t a un millisecondo, quindi in pratica questa funzione
// pone il valore di dt pari a 0.001) mele, pere, banane?

int main() {
  Flock stormo{0.5, 0.5, 0.5};
  stormo.fill(20);


  auto const delta_t{sf::milliseconds(1)};
  int const fps = 25;
  int const steps_per_evolution{200 / fps};

  // unsigned const int display_width =
  //     sf::VideoMode::getDesktopMode().width;  // larghezza quadrato
  // unsigned const int display_height =
  //     sf::VideoMode::getDesktopMode()
  //         .height;  // altezza quadrato
  //                   //(suggerisco almeno 5 volte tanto per entrambi)


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
  convex.setPoint(2, sf::Vector2f(5,50));
  convex.setPoint(3, sf::Vector2f(30,0));
  convex.setPoint(4, sf::Vector2f(55,50));
  convex.setPoint(5, sf::Vector2f(50,70));

  convex.setScale(0.3, 0.3);

  convex.setOrigin(sf::Vector2f(30, 35));  //setto l'origine locale (punto attorno a cui ruota il singolo boid) 
  

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type ==
          sf::Event::Closed) {  // if che serve per chiudere la finestre quando
                                /// si preme sulla "x" in alto a destra
        window.close();
      }
    }

    window.clear(sf::Color::White);
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

