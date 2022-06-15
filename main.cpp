
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
                          "Flock Simulation", sf::Style::Resize);

  window.setFramerateLimit(fps);

  sf::ConvexShape convex;  // genero una forma geometrica come modello del
                           // singolo boid (unisco i 6 punti che genero sotto)
  convex.setFillColor(sf::Color::Black);
  convex.setPointCount(6);

  convex.setPoint(0, sf::Vector2f(30, 60));
  convex.setPoint(1, sf::Vector2f(10, 70));
  convex.setPoint(2, sf::Vector2f(5, 50));
  convex.setPoint(3, sf::Vector2f(30, 0));
  convex.setPoint(4, sf::Vector2f(55, 50));
  convex.setPoint(5, sf::Vector2f(50, 70));

  convex.setScale(0.1, 0.1);

  convex.setOrigin(
      sf::Vector2f(30, 35));  // setto l'origine locale (punto attorno a cui
                              // ruota il singolo boid)

  sf::Texture background;
  background.loadFromFile("./boid_utilities/img/background1.png");
  sf::Sprite sprite_background(background);

  sf::Texture boom_texture;
  boom_texture.loadFromFile("./boid_utilities/sheetsprites/boom_sheetsprite.png");

  sf::IntRect rect_boom_sprite(0,0, 450 / 80, 56);
  sf::Sprite boom_sprite(boom_texture, rect_boom_sprite);

  sf::Clock clock;

  sf::SoundBuffer boom_sound_buffer;
  boom_sound_buffer.loadFromFile("./boid_utilities/audio/boom_sound.wav");
  sf::Sound boom_sound(boom_sound_buffer);
  

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type ==
          sf::Event::Closed) {  // if che serve per chiudere la finestre quando
                                /// si preme sulla "x" in alto a destra
        window.close();
      }
    }

    boom_positionx = 10000.f;
    boom_positiony = 10000.f;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
      boom_positionx = static_cast<double>(mousePosition.x);
      boom_positiony = static_cast<double>(mousePosition.y);

      if (clock.getElapsedTime().asSeconds() > 1.f){
        if (rect_boom_sprite.left = 392)
      }
      boom_sound.play();
    }

    window.clear(sf::Color::White);
    window.draw(sprite_background);

    

    update(stormo, steps_per_evolution, delta_t);

    for (auto& boid : stormo.get_flock()) {
      float angle =
          static_cast<float>(orientation(boid.get_vx(), boid.get_vy())) - 90;
      convex.setRotation(-angle);

      convex.setPosition(boid.get_x(), boid.get_y());

      window.draw(convex);  // disegna sull' oggetto window, ma non "displaya"
                            // ancora la window
    }

    window.display();  // adesso displaya
  } 
}

