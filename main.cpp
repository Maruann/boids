#include <iostream>

#include "flock.hpp"
#include "graphics.hpp"
#include "simulation.hpp"
#include "velocity.hpp"

// questa funzione evolve, diversa da quella in flock.hpp, fa evolvere lo
// stormo e ritorna il vettore flock (quello composto dai boids). La funzione
// asSeconds converte il valore di delta_t in secondi ( se vedi sotto,
// inizializiamo delta_t a un millisecondo, quindi in pratica questa funzione
// pone il valore di dt pari a 0.001) mele, pere, banane?

int main() {
  Flock stormo{0.5, 0.5, 0.5};
  stormo.fill(100);

  auto const delta_t{sf::milliseconds(1)};
  double const dt{delta_t.asSeconds()};
  int const fps = 25;
  int const steps_per_evolution{200 / fps};

  // unsigned const int display_width =
  //     sf::VideoMode::getDesktopMode().width;  // larghezza quadrato
  // unsigned const int display_height =
  //     sf::VideoMode::getDesktopMode()
  //         .height;  // altezza quadrato
  //                   //(suggerisco almeno 5 volte tanto per entrambi)

  const int display_width = 1280;  // larghezza quadrato
  const int display_height = 720;
  // altezza quadrato
  //(suggerisco almeno 5 volte tanto per entrambi)

  sf::RenderWindow window(sf::VideoMode(display_width, display_height),
                          "Flock Simulation", sf::Style::Titlebar);

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
  background.loadFromFile("./boid_utilities/img/background.png");
  sf::Sprite background_sprite(background);
  background_sprite.setScale(0.68, 0.68);

  sf::Texture boom_texture;
  boom_texture.loadFromFile(
      "./boid_utilities/sheet_sprites/boom_sheetsprite.png");

  sf::IntRect rect_boom_sprite(0, 0, 448 / 8, 56);
  sf::Sprite boom_sprite(boom_texture, rect_boom_sprite);
  boom_sprite.setOrigin(448 / 16, 28);

  sf::SoundBuffer boom_sound_buffer;
  boom_sound_buffer.loadFromFile("./boid_utilities/audio/boom_sound.ogg");
  sf::Sound boom_sound;
  boom_sound.setBuffer(boom_sound_buffer);

  sf::Clock sound_clock;

  boom_positionx = 10000;
  boom_positiony = 10000;

  int animation_index{0};

  ///////////////// PARTE DEGLI "SLIDER"

  // RETTANGOLO MENU
  sf::Vector2f menu_size(static_cast<float>(display_width / 2),
                         static_cast<float>(display_height / 10));
  sf::RectangleShape menu_rectangle(menu_size);
  menu_rectangle.setOrigin(0.f, menu_rectangle.getSize().y);
  menu_rectangle.setFillColor(sf::Color::Black);
  menu_rectangle.setOutlineThickness(-5.f);
  menu_rectangle.setOutlineColor(sf::Color::White);
  menu_rectangle.setPosition(0.f, static_cast<float>(display_height));

  //
  sf::ConvexShape up;
  up.setPointCount(3);
  up.setOutlineThickness(1);
  up.setOutlineColor(sf::Color::Black);
  up.setPoint(0, sf::Vector2f(1, 0));
  up.setPoint(1, sf::Vector2f(2, 2));
  up.setPoint(2, sf::Vector2f(0, 2));
  up.setScale(1, 1);
  up.setOrigin(sf::Vector2f(1, 1));

  sf::ConvexShape down;
  down.setPointCount(3);
  down.setOutlineThickness(1);
  down.setOutlineColor(sf::Color::Black);
  down.setPoint(0, sf::Vector2f(0, 0));
  down.setPoint(1, sf::Vector2f(2, 0));
  down.setPoint(2, sf::Vector2f(1, 2));
  up.setScale(1, 1);
  up.setOrigin(sf::Vector2f(1, 1));

  sf::ConvexShape double_up;
  double_up.setPointCount(7);
  double_up.setOutlineThickness(1);
  double_up.setOutlineColor(sf::Color::Black);
  double_up.setPoint(0, sf::Vector2f(1, 0));
  double_up.setPoint(1, sf::Vector2f(2, 1));
  double_up.setPoint(2, sf::Vector2f(1, 1));
  double_up.setPoint(3, sf::Vector2f(2, 2));
  double_up.setPoint(4, sf::Vector2f(0, 2));
  double_up.setPoint(5, sf::Vector2f(1, 1));
  double_up.setPoint(6, sf::Vector2f(0, 1));
  up.setScale(1, 1);
  up.setOrigin(sf::Vector2f(1, 1));

  sf::ConvexShape double_down;
  double_up.setPointCount(7);
  double_up.setOutlineThickness(1);
  double_up.setOutlineColor(sf::Color::Black);
  double_up.setPoint(0, sf::Vector2f(0, 0));
  double_up.setPoint(1, sf::Vector2f(2, 0));
  double_up.setPoint(2, sf::Vector2f(1, 1));
  double_up.setPoint(3, sf::Vector2f(2, 1));
  double_up.setPoint(4, sf::Vector2f(1, 2));
  double_up.setPoint(5, sf::Vector2f(0, 1));
  double_up.setPoint(6, sf::Vector2f(1, 1));
  up.setScale(1, 1);
  up.setOrigin(sf::Vector2f(1, 1));

  sf::Color sep_idle{240, 67, 67, 255};
  sf::Color sep_semiactive{158, 24, 24, 255};
  sf::Color sep_active{94, 9, 9, 255};
  sf::Color ali_idle{82, 219, 61, 255};
  sf::Color ali_semiactive{34, 143, 17, 255};
  sf::Color ali_active{17, 87, 6, 255};
  sf::Color coh_idle{77, 102, 232, 255};
  sf::Color coh_semiactive{17, 38, 143, 255};
  sf::Color coh_active{11, 26, 102, 255};

  Button sep_up{1, 1, up, sep_idle, sep_semiactive, sep_active, 0.25};
  Button sep_down{1, 1, down, sep_idle, sep_semiactive, sep_active, -0.25};
  Button sep_dou_up{1, 1, double_up, sep_idle, sep_semiactive, sep_active, 2};
  Button sep_dou_down{1, 1, double_down, sep_idle, sep_semiactive, sep_active, -2};

  Button ali_up{1, 1, up, ali_idle, ali_semiactive, ali_active, 0.25};
  Button ali_down{1, 1, down, ali_idle, ali_semiactive, ali_active, -0.25};
  Button ali_dou_up{1, 1, double_up, ali_idle, ali_semiactive, ali_active, 2};
  Button ali_dou_down{1, 1, double_down, ali_idle, ali_semiactive, ali_active, -2};

  Button coh_up{1, 1, up, coh_idle, coh_semiactive, coh_active, 0.25};
  Button coh_down{1, 1, down, coh_idle, coh_semiactive, coh_active, -0.25};
  Button coh_dou_up{1, 1, double_up, coh_idle, coh_semiactive, coh_active, 2};
  Button coh_dou_down{1, 1, double_down, coh_idle, coh_semiactive, coh_active, -2};

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type ==
          sf::Event::Closed) {  // if che serve per chiudere la finestre quando
                                /// si preme sulla "x" in alto a destra
        window.close();
      }
      if (event.type == sf::Event::MouseButtonReleased &&
          sound_clock.getElapsedTime().asSeconds() > 1.) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        boom_positionx = static_cast<double>(mousePosition.x);
        boom_positiony = static_cast<double>(mousePosition.y);
        boom_sound.play();
        sound_clock.restart();
        animation_index = 0;
      }
    }
    if (sound_clock.getElapsedTime().asSeconds() > 1.) {
      boom_positionx = 10000;
      boom_positiony = 10000;
    }

    rect_boom_sprite.left = animation_index * 56;
    boom_sprite.setTextureRect(rect_boom_sprite);
    boom_sprite.setPosition(boom_positionx, boom_positiony);

    window.clear();

    window.draw(background_sprite);
    window.draw(menu_rectangle);
    window.draw(boom_sprite);

    update(stormo, steps_per_evolution, dt);

    for (auto& boid : stormo.get_flock()) {
      double angle = orientation(boid.get_vx(), boid.get_vy());
      convex.setRotation(-angle);

      convex.setPosition(boid.get_x(), boid.get_y());

      window.draw(convex);  // disegna sull' oggetto window, ma non "displaya"
                            // ancora la window
    }

    window.display();  // adesso displaya

    ++animation_index;
    if (animation_index == 8) {
      animation_index = 0;
    }
  }
}