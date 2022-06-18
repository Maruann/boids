#include <iostream>

#include "flock.hpp"
#include "graphics.hpp"
#include "simulation.hpp"
#include "velocity.hpp"


int main() {
  Flock stormo{5., 5., 5.};
  stormo.fill(50);

  auto const delta_t{sf::milliseconds(1)};
  double const dt{delta_t.asSeconds()};
  int const fps = 25;
  int const steps_per_evolution{200 / fps};

  const int display_width = 1280;  
  const int display_height = 720;

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

  // rettangolo del menu
  sf::Color menu_color(149, 149, 149, 255);//colore dei rettangoli del menu
  float menu_rectangle_width = static_cast<float>(display_width / 2); //SEMBRA UNA SCELTA ILLOGICA MA HA SENSO PER SOTTO
  float menu_rectangle_height = static_cast<float>(display_height / 10);
  sf::RectangleShape menu_rectangle(sf::Vector2f(menu_rectangle_width,  menu_rectangle_height));
  menu_rectangle.setOrigin(0.f, menu_rectangle.getSize().y);
  menu_rectangle.setFillColor(menu_color);
  menu_rectangle.setOutlineThickness(-5.f);
  menu_rectangle.setOutlineColor(sf::Color::White);
  menu_rectangle.setPosition(0.f, static_cast<float>(display_height));

  //rettangolo per il display statistico
  float stat_rectangle_width = menu_rectangle_width * (2.f / 5.f); //SEMBRA UNA SCELTA ILLOGICA MA HA SENSO PER SOTTO
  float stat_rectangle_height = menu_rectangle_height * (3.f / 2.f);
  sf::RectangleShape stat_rectangle(sf::Vector2f(stat_rectangle_width,  stat_rectangle_height));
  stat_rectangle.setOrigin(stat_rectangle_width, stat_rectangle_height);
  stat_rectangle.setFillColor(menu_color);
  stat_rectangle.setOutlineThickness(-5.f);
  stat_rectangle.setOutlineColor(sf::Color::White);
  stat_rectangle.setPosition(static_cast<float>(display_width), static_cast<float>(display_height));

  //rettangolo deile caselle di testo
  float text_rectangle_width = menu_rectangle_width * (8.f / 60.f); //SEMBRA UNA SCELTA ILLOGICA MA HA SENSO PER SOTTO
  float text_rectangle_height = menu_rectangle_height * (1.f / 3.f);
  sf::RectangleShape text_rectangle(sf::Vector2f(text_rectangle_width,  text_rectangle_height));
  text_rectangle.setOrigin(text_rectangle_width / 2.f, text_rectangle_height / 2.f);
  text_rectangle.setFillColor(sf::Color::White);
  text_rectangle.setOutlineThickness(-5.f);
  text_rectangle.setOutlineColor(sf::Color::White);
  
  //bottoni
  float button_scale = 10.f;
  float button_outline_thickness = -0.2f;
  sf::ConvexShape up;
  up.setPointCount(3);
  up.setOutlineThickness(button_outline_thickness);
  up.setOutlineColor(sf::Color::Black);
  up.setPoint(0, sf::Vector2f(1, 0));
  up.setPoint(1, sf::Vector2f(2, 2));
  up.setPoint(2, sf::Vector2f(0, 2));
  up.setScale(button_scale, button_scale);
  up.setOrigin(sf::Vector2f(1, 1));

  sf::ConvexShape down;
  down.setPointCount(3);
  down.setOutlineThickness(button_outline_thickness);
  down.setOutlineColor(sf::Color::Black);
  down.setPoint(0, sf::Vector2f(0, 0));
  down.setPoint(1, sf::Vector2f(2, 0));
  down.setPoint(2, sf::Vector2f(1, 2));
  down.setScale(button_scale, button_scale);
  down.setOrigin(sf::Vector2f(1, 1));

  sf::ConvexShape double_up;
  double_up.setPointCount(7);
  double_up.setOutlineThickness(button_outline_thickness);
  double_up.setOutlineColor(sf::Color::Black);
  double_up.setPoint(0, sf::Vector2f(1, 0));
  double_up.setPoint(1, sf::Vector2f(2, 1));
  double_up.setPoint(2, sf::Vector2f(1, 1));
  double_up.setPoint(3, sf::Vector2f(2, 2));
  double_up.setPoint(4, sf::Vector2f(0, 2));
  double_up.setPoint(5, sf::Vector2f(1, 1));
  double_up.setPoint(6, sf::Vector2f(0, 1));
  double_up.setScale(button_scale, button_scale);
  double_up.setOrigin(sf::Vector2f(1, 1));

  sf::ConvexShape double_down;
  double_down.setPointCount(7);
  double_down.setOutlineThickness(button_outline_thickness);
  double_down.setOutlineColor(sf::Color::Black);
  double_down.setPoint(0, sf::Vector2f(0, 0));
  double_down.setPoint(1, sf::Vector2f(2, 0));
  double_down.setPoint(2, sf::Vector2f(1, 1));
  double_down.setPoint(3, sf::Vector2f(2, 1));
  double_down.setPoint(4, sf::Vector2f(1, 2));
  double_down.setPoint(5, sf::Vector2f(0, 1));
  double_down.setPoint(6, sf::Vector2f(1, 1));
  double_down.setScale(button_scale, button_scale);
  double_down.setOrigin(sf::Vector2f(1,1));

  sf::Color sep_idle{240, 67, 67, 255};//ROSSO
  sf::Color sep_semiactive{158, 24, 24, 255};
  sf::Color sep_active{94, 9, 9, 255};
  sf::Color ali_idle{82, 219, 61, 255};//VERDE
  sf::Color ali_semiactive{34, 143, 17, 255};
  sf::Color ali_active{17, 87, 6, 255};
  sf::Color coh_idle{77, 102, 232, 255};//BLU
  sf::Color coh_semiactive{17, 38, 143, 255};
  sf::Color coh_active{11, 26, 102, 255};

  Button sep_up{menu_rectangle_width * (4.f/60.f), (display_height - menu_rectangle_height * (3.f / 4.f)), up, sep_idle, sep_semiactive, sep_active, 0.25, sep};
  Button sep_down{menu_rectangle_width * (4.f/60.f), (display_height - menu_rectangle_height * (1.f / 4.f)), down, sep_idle, sep_semiactive, sep_active, -0.25, sep};
  Button sep_dou_up{menu_rectangle_width * (7.f/60.f), (display_height - menu_rectangle_height * (3.f / 4.f)), double_up, sep_idle, sep_semiactive, sep_active, 2, sep};
  Button sep_dou_down{menu_rectangle_width * (7.f/60.f), (display_height - menu_rectangle_height * (1.f / 4.f)), double_down, sep_idle, sep_semiactive, sep_active, -2, sep};

  Button ali_up{menu_rectangle_width * (24.f/60.f), (display_height - menu_rectangle_height * (3.f / 4.f)), up, ali_idle, ali_semiactive, ali_active, 0.25, ali};
  Button ali_down{menu_rectangle_width * (24.f/60.f), (display_height - menu_rectangle_height * (1.f / 4.f)), down, ali_idle, ali_semiactive, ali_active, -0.25, ali};
  Button ali_dou_up{menu_rectangle_width * (27.f/60.f), (display_height - menu_rectangle_height * (3.f / 4.f)), double_up, ali_idle, ali_semiactive, ali_active, 2, ali};
  Button ali_dou_down{menu_rectangle_width * (27.f/60.f), (display_height - menu_rectangle_height * (1.f / 4.f)), double_down, ali_idle, ali_semiactive, ali_active, -2, ali};

  Button coh_up{menu_rectangle_width * (44.f/60.f), (display_height - menu_rectangle_height * (3.f / 4.f)), up, coh_idle, coh_semiactive, coh_active, 0.25, coh};
  Button coh_down{menu_rectangle_width * (44.f/60.f), (display_height - menu_rectangle_height * (1.f / 4.f)), down, coh_idle, coh_semiactive, coh_active, -0.25, coh};
  Button coh_dou_up{menu_rectangle_width * (47.f/60.f), (display_height - menu_rectangle_height * (3.f / 4.f)), double_up, coh_idle, coh_semiactive, coh_active, 2, coh};
  Button coh_dou_down{menu_rectangle_width * (47.f/60.f), (display_height - menu_rectangle_height * (1.f / 4.f)), double_down, coh_idle, coh_semiactive, coh_active, -2, coh};

  std::array<Button, 12> buttons{sep_up, sep_down, sep_dou_up, sep_dou_down, ali_up, ali_down, ali_dou_up, ali_dou_down, coh_up, coh_down, coh_dou_up, coh_dou_down};

  int click_state = unclicked;

  //testi
  sf::Font font;
  font.loadFromFile("./boid_utilities/fonts/arial.ttf");

  sf::Text text;
  text.setFont(font);
  text.setFillColor(sf::Color::Black);
  text.setCharacterSize(12);
  
  //GAME LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOP

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type ==
          sf::Event::Closed) {  // if che serve per chiudere la finestre quando
                                /// si preme sulla "x" in alto a destra
        window.close();
      }
      else if (event.type == sf::Event::MouseButtonReleased &&
          sound_clock.getElapsedTime().asSeconds() > 1.f && !(menu_rectangle.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,
                                        sf::Mouse::getPosition(window).y)) && !(stat_rectangle.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,
                                        sf::Mouse::getPosition(window).y))) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        boom_positionx = static_cast<double>(mousePosition.x);
        boom_positiony = static_cast<double>(mousePosition.y);
        boom_sound.play();
        sound_clock.restart();
        animation_index = 0;
      }
      else if (event.type == sf::Event::MouseButtonReleased){
        click_state = clicked;
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

    //drawo il background
    window.draw(background_sprite);

    //inizio a draware l'interfaccia
    window.draw(menu_rectangle);
    window.draw(stat_rectangle);

    //drawo i rettangoli relativi alle caselle di testo
      //parametri 
    text_rectangle.setPosition(menu_rectangle_width * (14.f / 60.f),(display_height - menu_rectangle_height * (2.f / 5.f)));
    window.draw(text_rectangle);
    text_rectangle.setPosition(menu_rectangle_width * (34.f / 60.f),(display_height - menu_rectangle_height * (2.f / 5.f)));
    window.draw(text_rectangle);
    text_rectangle.setPosition(menu_rectangle_width * (54.f / 60.f),(display_height - menu_rectangle_height * (2.f / 5.f)));
    window.draw(text_rectangle);

      //analisi statistica
    text_rectangle.setPosition(display_width - stat_rectangle_width * (1.f / 5.f),(display_height - stat_rectangle_height * (3.f / 15.f)));
    window.draw(text_rectangle);
    text_rectangle.setPosition(display_width - stat_rectangle_width * (1.f / 5.f),(display_height - stat_rectangle_height * (8.f / 15.f)));
    window.draw(text_rectangle);
    
    //drawo i testi
      //titoli parametri
    text.setCharacterSize(16);
    text.setOutlineThickness(1.f);
    text.setOutlineColor(sf::Color::Black);
    text.setString("Separation");
    text.setFillColor(sep_idle);
    text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
    text.setPosition(menu_rectangle_width * (14.f / 60.f), (display_height - menu_rectangle_height * (9.f / 11.f)));
    window.draw(text);
    text.setString("Alignment");
    text.setFillColor(ali_idle);
    text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
    text.setPosition(menu_rectangle_width * (34.f / 60.f), (display_height - menu_rectangle_height * (9.f / 11.f)));
    window.draw(text);
    text.setString("Cohesion");
    text.setFillColor(coh_idle);
    text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
    text.setPosition(menu_rectangle_width * (54.f / 60.f), (display_height - menu_rectangle_height * (9.f / 11.f)));
    window.draw(text);

      //parametri
    text.setCharacterSize(14);
    text.setOutlineThickness(0.f);
    text.setFillColor(sf::Color::Black);
    text.setString(std::to_string(stormo.get_sep()));
    text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
    text.setPosition(menu_rectangle_width * (14.f / 60.f), (display_height - menu_rectangle_height * (5.f / 11.f)));
    window.draw(text);
    text.setString(std::to_string(stormo.get_ali()));
    text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
    text.setPosition(menu_rectangle_width * (34.f / 60.f), (display_height - menu_rectangle_height * (5.f / 11.f)));
    window.draw(text);
    text.setString(std::to_string(stormo.get_coe()));
    text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
    text.setPosition(menu_rectangle_width * (54.f / 60.f), (display_height - menu_rectangle_height * (5.f / 11.f)));
    window.draw(text);

      //titoli analisi statistica
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Black);
    text.setString("Boids Distance");
    text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
    text.setPosition(display_width - stat_rectangle_width * (1.f / 2.f),(display_height - stat_rectangle_height * (13.f / 15.f)));
    window.draw(text);
    text.setFillColor(sf::Color::Black);
    text.setString("Mean:");
    text.setOrigin(0.f, text.getGlobalBounds().height / 2.f);
    text.setPosition(display_width - stat_rectangle_width * (19.f / 20.f),(display_height - stat_rectangle_height * (9.f / 15.f)));
    window.draw(text);
    text.setFillColor(sf::Color::Black);
    text.setString("Standard Deviation: ");
    text.setOrigin(0.f, text.getGlobalBounds().height / 2.f);
    text.setPosition(display_width - stat_rectangle_width * (19.f / 20.f),(display_height - stat_rectangle_height * (4.f / 15.f)));
    window.draw(text);

      //analisi statistica
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Black);
    text.setString("numero");
    text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
    text.setPosition(display_width - stat_rectangle_width * (1.f / 5.f),(display_height - stat_rectangle_height * (9.f / 15.f)));
    window.draw(text);
    text.setFillColor(sf::Color::Black);
    text.setString("numero");
    text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
    text.setPosition(display_width - stat_rectangle_width * (1.f / 5.f),(display_height - stat_rectangle_height * (4.f / 15.f)));
    window.draw(text);

    //drawo il frame corrente dell'animazione dello scoppio
    window.draw(boom_sprite);

    //aggiorno le interazioni dei bottoni e li drawo
    for(auto& button : buttons){
      button.update(stormo, window, click_state);
      button.draw(window);
    }
    //aggiorno il flock e lo drawo
    update(stormo, steps_per_evolution, dt);
    for (auto& boid : stormo.get_flock()) {
      double angle = orientation(boid.get_vx(), boid.get_vy());
      convex.setRotation(-angle);

      convex.setPosition(boid.get_x(), boid.get_y());

      window.draw(convex);  // disegna sull' oggetto window, ma non "displaya"
                            // ancora la window
    }
    window.display();  // adesso displaya
    
    click_state = unclicked;

    ++animation_index;
    if (animation_index == 8) {
      animation_index = 0;
    }
  }
}