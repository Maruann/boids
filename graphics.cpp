#include "graphics.hpp"
#include "velocity.hpp"
#include "simulation.hpp"


void Button::update(Flock& flock, sf::RenderWindow& window, int click_state, double& vision, sf::Text& text) {
  if ((shape_.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,
                                         sf::Mouse::getPosition(window).y)) &&
      click_state == clicked) {
    shape_.setFillColor(active_color_);
    switch (parameter_index_) {
      case sep:
        flock.set_sep(flock.get_sep() + increment_);
        if(flock.get_sep() > 10.){flock.set_sep(10.);}
        else if(flock.get_sep() < 0.){flock.set_sep(0.);}
        text.setString(std::to_string(flock.get_sep()));
        break;
      case ali:
        flock.set_ali(flock.get_ali() + increment_);
        if(flock.get_ali() > 10.){flock.set_ali(10.);}
        else if(flock.get_ali() < 0.){flock.set_ali(0.);}
        text.setString(std::to_string(flock.get_ali()));
        break;
      case coh:
        flock.set_coe(flock.get_coe() + increment_);
        if(flock.get_coe() > 10.){flock.set_coe(10.);}
        else if(flock.get_coe() < 0.){flock.set_coe(0.);}
        text.setString(std::to_string(flock.get_coe()));
        break;
      case vis:
        vision += increment_;
        if(vision > 2.){vision = 2.;}
        else if(vision < 0.){vision = 0.;}
        text.setString(std::to_string(vision));
      default:
        break;
    }
  } else if (shape_.getGlobalBounds().contains(
                 sf::Mouse::getPosition(window).x,
                 sf::Mouse::getPosition(window).y)) {
    shape_.setFillColor(semiactive_color_);
  } else {
    shape_.setFillColor(idle_color_);
  }
}

void graphics(Flock& stormo){
auto const delta_t{sf::milliseconds(1)};
  double const dt{delta_t.asSeconds()};
  int const fps = 25;
  int const steps_per_evolution{200 / fps};

  const int display_width = 1280;  
  const int display_height = 720;

  double dist_mult = 1.;

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
  float menu_rectangle_width = static_cast<float>(display_width * (2.f / 3.f)); //SEMBRA UNA SCELTA ILLOGICA MA HA SENSO PER SOTTO
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
  
  //rettangoli per il testo dei parametri
  sf::Color text_color(sf::Color::White);
  float text_rectangle_width = menu_rectangle_width * (8.f / 60.f); 
  float text_rectangle_height = menu_rectangle_height * (1.f / 3.f);
  sf::RectangleShape sep_text_rectangle(sf::Vector2f(text_rectangle_width,  text_rectangle_height));
  sep_text_rectangle.setOrigin(text_rectangle_width / 2.f, text_rectangle_height / 2.f);
  sep_text_rectangle.setPosition(menu_rectangle_width * (13.f / 80.f),(display_height - menu_rectangle_height * (2.f / 5.f)));
  sep_text_rectangle.setFillColor(text_color);
  sf::RectangleShape ali_text_rectangle(sf::Vector2f(text_rectangle_width,  text_rectangle_height));
  ali_text_rectangle.setOrigin(text_rectangle_width / 2.f, text_rectangle_height / 2.f);
  ali_text_rectangle.setPosition(menu_rectangle_width * (33.f / 80.f),(display_height - menu_rectangle_height * (2.f / 5.f)));
  ali_text_rectangle.setFillColor(text_color);
  sf::RectangleShape coh_text_rectangle(sf::Vector2f(text_rectangle_width,  text_rectangle_height));
  coh_text_rectangle.setOrigin(text_rectangle_width / 2.f, text_rectangle_height / 2.f);
  coh_text_rectangle.setPosition(menu_rectangle_width * (53.f / 80.f),(display_height - menu_rectangle_height * (2.f / 5.f)));
  coh_text_rectangle.setFillColor(text_color);
  sf::RectangleShape vis_text_rectangle(sf::Vector2f(text_rectangle_width,  text_rectangle_height));
  vis_text_rectangle.setOrigin(text_rectangle_width / 2.f, text_rectangle_height / 2.f);
  vis_text_rectangle.setPosition(menu_rectangle_width * (73.f / 80.f),(display_height - menu_rectangle_height * (2.f / 5.f)));
  vis_text_rectangle.setFillColor(text_color);
  
  //rettangoli per il testo dei dati statistici
  sf::RectangleShape mean_text_rectangle(sf::Vector2f(text_rectangle_width,  text_rectangle_height));
  mean_text_rectangle.setOrigin(text_rectangle_width / 2.f, text_rectangle_height / 2.f);
  mean_text_rectangle.setPosition(display_width - stat_rectangle_width * (1.f / 5.f),(display_height - stat_rectangle_height * (3.f / 15.f)));
  mean_text_rectangle.setFillColor(text_color);
  sf::RectangleShape std_text_rectangle(sf::Vector2f(text_rectangle_width,  text_rectangle_height));
  std_text_rectangle.setOrigin(text_rectangle_width / 2.f, text_rectangle_height / 2.f);
  std_text_rectangle.setPosition(display_width - stat_rectangle_width * (1.f / 5.f),(display_height - stat_rectangle_height * (8.f / 15.f)));
  std_text_rectangle.setFillColor(text_color);
  
  
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
  sf::Color vis_idle{231, 237, 62, 255};//GIALLO
  sf::Color vis_semiactive{176, 181, 29, 255};
  sf::Color vis_active{119, 122, 9, 255};


  Button sep_up{menu_rectangle_width * (3.f/80.f), (display_height - menu_rectangle_height * (3.f / 4.f)), up, sep_idle, sep_semiactive, sep_active, 0.25, sep};
  Button sep_down{menu_rectangle_width * (3.f/80.f), (display_height - menu_rectangle_height * (1.f / 4.f)), down, sep_idle, sep_semiactive, sep_active, -0.25, sep};
  Button sep_dou_up{menu_rectangle_width * (6.f/80.f), (display_height - menu_rectangle_height * (3.f / 4.f)), double_up, sep_idle, sep_semiactive, sep_active, 2, sep};
  Button sep_dou_down{menu_rectangle_width * (6.f/80.f), (display_height - menu_rectangle_height * (1.f / 4.f)), double_down, sep_idle, sep_semiactive, sep_active, -2, sep};

  Button ali_up{menu_rectangle_width * (23.f/80.f), (display_height - menu_rectangle_height * (3.f / 4.f)), up, ali_idle, ali_semiactive, ali_active, 0.25, ali};
  Button ali_down{menu_rectangle_width * (23.f/80.f), (display_height - menu_rectangle_height * (1.f / 4.f)), down, ali_idle, ali_semiactive, ali_active, -0.25, ali};
  Button ali_dou_up{menu_rectangle_width * (26.f/80.f), (display_height - menu_rectangle_height * (3.f / 4.f)), double_up, ali_idle, ali_semiactive, ali_active, 2, ali};
  Button ali_dou_down{menu_rectangle_width * (26.f/80.f), (display_height - menu_rectangle_height * (1.f / 4.f)), double_down, ali_idle, ali_semiactive, ali_active, -2, ali};

  Button coh_up{menu_rectangle_width * (43.f/80.f), (display_height - menu_rectangle_height * (3.f / 4.f)), up, coh_idle, coh_semiactive, coh_active, 0.25, coh};
  Button coh_down{menu_rectangle_width * (43.f/80.f), (display_height - menu_rectangle_height * (1.f / 4.f)), down, coh_idle, coh_semiactive, coh_active, -0.25, coh};
  Button coh_dou_up{menu_rectangle_width * (46.f/80.f), (display_height - menu_rectangle_height * (3.f / 4.f)), double_up, coh_idle, coh_semiactive, coh_active, 2, coh};
  Button coh_dou_down{menu_rectangle_width * (46.f/80.f), (display_height - menu_rectangle_height * (1.f / 4.f)), double_down, coh_idle, coh_semiactive, coh_active, -2, coh};

  Button vis_up{menu_rectangle_width * (63.f/80.f), (display_height - menu_rectangle_height * (3.f / 4.f)), up, vis_idle, vis_semiactive, vis_active, 0.05, vis};
  Button vis_down{menu_rectangle_width * (63.f/80.f), (display_height - menu_rectangle_height * (1.f / 4.f)), down, vis_idle, vis_semiactive, vis_active, -0.05, vis};
  Button vis_dou_up{menu_rectangle_width * (66.f/80.f), (display_height - menu_rectangle_height * (3.f / 4.f)), double_up, vis_idle, vis_semiactive, vis_active, 0.2, vis};
  Button vis_dou_down{menu_rectangle_width * (66.f/80.f), (display_height - menu_rectangle_height * (1.f / 4.f)), double_down, vis_idle, vis_semiactive, vis_active, -0.2, vis};

  std::array<Button, 4> sep_buttons{sep_up, sep_down, sep_dou_up, sep_dou_down};
  std::array<Button, 4> ali_buttons{ali_up, ali_down, ali_dou_up, ali_dou_down};
  std::array<Button, 4> coh_buttons{coh_up, coh_down, coh_dou_up, coh_dou_down};
  std::array<Button, 4> vis_buttons{vis_up, vis_down, vis_dou_up, vis_dou_down};

  int click_state = unclicked;

  //testi
  sf::Font font;
  font.loadFromFile("./boid_utilities/fonts/arial.ttf");

      //testi titoli parametri
  sf::Text sep_title_text;
  sep_title_text.setFont(font);
  sep_title_text.setCharacterSize(16);
  sep_title_text.setOutlineThickness(1.f);
  sep_title_text.setOutlineColor(sf::Color::Black);
  sep_title_text.setString("Separation");
  sep_title_text.setFillColor(sep_idle);
  sep_title_text.setOrigin(sep_title_text.getGlobalBounds().width / 2.f, sep_title_text.getGlobalBounds().height / 2.f);
  sep_title_text.setPosition(menu_rectangle_width * (13.f / 80.f), (display_height - menu_rectangle_height * (9.f / 11.f)));
  sf::Text ali_title_text;
  ali_title_text.setFont(font);
  ali_title_text.setCharacterSize(16);
  ali_title_text.setOutlineThickness(1.f);
  ali_title_text.setOutlineColor(sf::Color::Black);
  ali_title_text.setString("Alignment");
  ali_title_text.setFillColor(ali_idle);
  ali_title_text.setOrigin(ali_title_text.getGlobalBounds().width / 2.f, ali_title_text.getGlobalBounds().height / 2.f);
  ali_title_text.setPosition(menu_rectangle_width * (33.f / 80.f), (display_height - menu_rectangle_height * (9.f / 11.f)));
  sf::Text coh_title_text;
  coh_title_text.setFont(font);
  coh_title_text.setCharacterSize(16);
  coh_title_text.setOutlineThickness(1.f);
  coh_title_text.setOutlineColor(sf::Color::Black);
  coh_title_text.setString("Cohesion");
  coh_title_text.setFillColor(coh_idle);
  coh_title_text.setOrigin(coh_title_text.getGlobalBounds().width / 2.f, coh_title_text.getGlobalBounds().height / 2.f);
  coh_title_text.setPosition(menu_rectangle_width * (53.f / 80.f), (display_height - menu_rectangle_height * (9.f / 11.f)));
  sf::Text vis_title_text;
  vis_title_text.setFont(font);
  vis_title_text.setCharacterSize(16);
  vis_title_text.setOutlineThickness(1.f);
  vis_title_text.setOutlineColor(sf::Color::Black);
  vis_title_text.setString("Vision");
  vis_title_text.setFillColor(vis_idle);
  vis_title_text.setOrigin(vis_title_text.getGlobalBounds().width / 2.f, vis_title_text.getGlobalBounds().height / 2.f);
  vis_title_text.setPosition(menu_rectangle_width * (73.f / 80.f), (display_height - menu_rectangle_height * (9.f / 11.f)));

      //testi titoli dati statistici
  sf::Text boid_distance_title_text;
  boid_distance_title_text.setFont(font);
  boid_distance_title_text.setCharacterSize(16);
  boid_distance_title_text.setString("Boids Distance");
  boid_distance_title_text.setFillColor(sf::Color::Black);
  boid_distance_title_text.setOrigin(boid_distance_title_text.getGlobalBounds().width / 2.f, boid_distance_title_text.getGlobalBounds().height / 2.f);
  boid_distance_title_text.setPosition(display_width - stat_rectangle_width * (1.f / 2.f),(display_height - stat_rectangle_height * (13.f / 15.f)));
  sf::Text mean_title_text;
  mean_title_text.setFont(font);
  mean_title_text.setCharacterSize(16);
  mean_title_text.setString("Mean:");
  mean_title_text.setFillColor(sf::Color::Black);
  mean_title_text.setOrigin(0.f, mean_title_text.getGlobalBounds().height / 2.f);
  mean_title_text.setPosition(display_width - stat_rectangle_width * (19.f / 20.f),(display_height - stat_rectangle_height * (9.f / 15.f)));
  sf::Text std_title_text;
  std_title_text.setFont(font);
  std_title_text.setCharacterSize(16);
  std_title_text.setString("Standard Deviation:");
  std_title_text.setFillColor(sf::Color::Black);
  std_title_text.setOrigin(0.f, std_title_text.getGlobalBounds().height / 2.f);
  std_title_text.setPosition(display_width - stat_rectangle_width * (19.f / 20.f),(display_height - stat_rectangle_height * (4.f / 15.f)));
  
      //testi parametri
  sf::Text sep_text;
  sep_text.setFont(font);
  sep_text.setCharacterSize(14);
  sep_text.setFillColor(sf::Color::Black);
  sep_text.setOrigin(0.f, 0.f);
  sep_text.setString(std::to_string(stormo.get_sep()));
  sep_text.setPosition(menu_rectangle_width * (10.f / 80.f), (display_height - menu_rectangle_height * (7.f / 13.f)));
  sf::Text ali_text;
  ali_text.setFont(font);
  ali_text.setCharacterSize(14);
  ali_text.setFillColor(sf::Color::Black);
  ali_text.setOrigin(0.f, 0.f);
  ali_text.setString(std::to_string(stormo.get_ali()));
  ali_text.setPosition(menu_rectangle_width * (30.f / 80.f), (display_height - menu_rectangle_height * (7.f / 13.f)));
  sf::Text coh_text;
  coh_text.setFont(font);
  coh_text.setCharacterSize(14);
  coh_text.setFillColor(sf::Color::Black);
  coh_text.setOrigin(0.f, 0.f);
  coh_text.setString(std::to_string(stormo.get_coe()));
  coh_text.setPosition(menu_rectangle_width * (50.f / 80.f), (display_height - menu_rectangle_height * (7.f / 13.f)));
  sf::Text vis_text;
  vis_text.setFont(font);
  vis_text.setCharacterSize(14);
  vis_text.setFillColor(sf::Color::Black);
  vis_text.setOrigin(0.f, 0.f);
  vis_text.setString(std::to_string(dist_mult));
  vis_text.setPosition(menu_rectangle_width * (70.f / 80.f), (display_height - menu_rectangle_height * (7.f / 13.f)));

      //testi dati statistici
  sf::Text mean_text;
  mean_text.setFont(font);
  mean_text.setCharacterSize(16);
  mean_text.setFillColor(sf::Color::Black);
  mean_text.setOrigin(0.f, mean_text.getGlobalBounds().height / 2.f);
  mean_text.setPosition(display_width - stat_rectangle_width * (3.f / 10.f),(display_height - stat_rectangle_height * (10.f / 15.f)));
  sf::Text std_text;
  std_text.setFont(font);
  std_text.setCharacterSize(16);
  std_text.setFillColor(sf::Color::Black);
  std_text.setOrigin(0.f, std_text.getGlobalBounds().height / 2.f);
  std_text.setPosition(display_width - stat_rectangle_width * (3.f / 10.f),(display_height - stat_rectangle_height * (5.f / 15.f)));

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
    boom_sprite.setPosition(boom_positionx, boom_positiony);//DA RIVEDERE 

    //aggiorno i bottoni
    for(auto& button : sep_buttons){
      button.update(stormo, window, click_state, dist_mult, sep_text);
    }
    for(auto& button : ali_buttons){
      button.update(stormo, window, click_state, dist_mult, ali_text);
    }
    for(auto& button : coh_buttons){
      button.update(stormo, window, click_state, dist_mult, coh_text);
    }
    for(auto& button : vis_buttons){
      button.update(stormo, window, click_state, dist_mult, vis_text);
    }

    //aggiorno il flock
    update(stormo, steps_per_evolution, dt, dist_mult);

    window.clear();

    //drawo il background
    window.draw(background_sprite);

    //drawo il flock
    for (auto& boid : stormo.get_flock()) {
      double angle = orientation(boid.get_vx(), boid.get_vy());
      convex.setRotation(-angle);

      convex.setPosition(boid.get_x(), boid.get_y());

      window.draw(convex);  // disegna sull' oggetto window, ma non "displaya"
                            // ancora la window
    }

    //drawo il frame corrente dell'animazione dello scoppio
    window.draw(boom_sprite);

    //inizio a draware l'interfaccia
    window.draw(menu_rectangle);
    window.draw(stat_rectangle);

    //drawo i rettangoli relativi alle caselle di testo
        //parametri 
    window.draw(sep_text_rectangle);
    window.draw(ali_text_rectangle);
    window.draw(coh_text_rectangle);

    window.draw(vis_text_rectangle);
        //analisi statistica
    window.draw(mean_text_rectangle);
    window.draw(std_text_rectangle);
    
    //drawo i testi
        //titoli parametri
    window.draw(sep_title_text);
    window.draw(ali_title_text);
    window.draw(coh_title_text);
    window.draw(vis_title_text);
        //titoli analisi statistica
    window.draw(boid_distance_title_text);
    window.draw(mean_title_text);
    window.draw(std_title_text);
        //parametri

    window.draw(sep_text);
    window.draw(ali_text);
    window.draw(coh_text);
    window.draw(vis_text);
        //analisi statistica
    mean_text.setString("numero"); //VA LASCIATA QUI; PERCHé (non ho il tastierino) AD OGNI LOOP DOVRA RICALCOLARLA (PIù O MENO, POI IN REALTà LO METTERò DIRETTAMENTE NELLA FUNZIONE, CHE SARà CHIAMATA OGNI 3 SEC) 
    window.draw(mean_text);
    std_text.setString("numero");//STESSA COSA
    window.draw(std_text);

    
    //drawo i bottoni
    for(auto& button : sep_buttons){
      button.draw(window);
    }
    for(auto& button : ali_buttons){
      button.draw(window);
    }
    for(auto& button : coh_buttons){
      button.draw(window);
    }
    for(auto& button : vis_buttons){
      button.draw(window);
    }

    window.display();  // adesso displaya
    
    click_state = unclicked;

    ++animation_index;
    if (animation_index == 8) {
      animation_index = 0;
    }
  }
}

