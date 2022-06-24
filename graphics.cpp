#include "graphics.hpp"


#include <cassert>
#include <iostream>
#include <sstream>

#include "simulation.hpp"
#include "velocity.hpp"

class FPS {
 public:
  /// @brief Constructor with initialization.
  ///
  FPS() : mFrame(0), mFps(0) {}

  /// @brief Update the frame count.
  ///

  /// @brief Get the current FPS count.
  /// @return FPS count.
  unsigned int getFPS() const { return mFps; }

 private:
  unsigned int mFrame;
  unsigned int mFps;
  sf::Clock mClock;

 public:
  void update() {
    if (mClock.getElapsedTime().asSeconds() >= 1.f) {
      mFps = mFrame;
      mFrame = 0;
      mClock.restart();
    }

    ++mFrame;
  }
};
// Si definisce una funzione che permette di convertire dei double in stringhe
// arrotondate alla cifra decimale desiderata (viene cancellata dalla n-esima
// cifra decimale esclusa in poi)
std::string roundto(double const num, int const n) {
  std::string string{std::to_string(num)};
  int i{1};
  for (char& character : string) {
    if (character == '.') {
      break;
    } else {
      ++i;
    }
  }
  assert(i + n <= static_cast<int>(string.size()) + 1);
  return (string.erase(i + n));
}

// Di seguito sono riportate 3 funzioni che permetteranno successivamente con
// un'unica espressione di modificare gli oggetti grafici in modo da poterli
// disporre nel modo necessario per il game loop

// Shape_init_setting permette di modificare degli oggetti di tipo ConvexShape
// in modo da potergli fare assumere la forma desiderata tramite il
// vettore(della stl) di vettori(di sfml) in pratica i punti descritti dai
// vettori bidimensionali andranno ad unirsi secondo l'ordine dell'indice
// andando a generare una forma
void shape_init_setting(sf::ConvexShape& shape_name,
                        std::vector<sf::Vector2f>& vector,
                        float const outl_thickness, float const button_scale,
                        int const color_choice, sf::Color const fill_color) {
  shape_name.setPointCount(vector.size());
  int vec_size{static_cast<int>(vector.size())};
  for (int i{0}; i != vec_size; ++i) {
    shape_name.setPoint(i, vector[i]);
  }
  if (outl_thickness != 0.f) {
    shape_name.setOutlineThickness(outl_thickness);
    shape_name.setOutlineColor(sf::Color::Black);
  }
  shape_name.setOrigin(sf::Vector2f(shape_name.getGlobalBounds().width / 2.f, shape_name.getGlobalBounds().height / 2.f));
  shape_name.setScale(button_scale, button_scale);
  if (color_choice == colored) {
    shape_name.setFillColor(fill_color);
  }
}

// funzione che serve per impostare i rettangoli del menu (i due grandi che
// appaiono a schermo) e i rettangolini bianchi su cui verranno posizionate le
// caselle di testo relative ai valori dei parametri
void rect_init_setting(sf::RectangleShape& rect, float const width,
                       float const height, float const outl_thickness,
                       sf::Color const fill_color, int const origin_choice,
                       float const posit_x, float const posit_y) {
  rect.setSize(sf::Vector2f(width, height));
  if (outl_thickness != 0.f) {
    rect.setOutlineThickness(outl_thickness);
    rect.setOutlineColor(sf::Color::White);
  }
  rect.setFillColor(fill_color);
  if (origin_choice == bottom_left_origin) {
    rect.setOrigin(0.f, height);
  } else if (origin_choice == bottom_right_origin) {
    rect.setOrigin(width, height);
  } else {
    rect.setOrigin(width / 2.f, height / 2.f);
  }
  rect.setPosition(posit_x, posit_y);
}

// funzione che permette di impostare le caselle di testo
void text_init_setting(sf::Text& text_name, sf::Font& font, int const char_size,
                       float const outl_thickness,
                       std::string const text_to_display,
                       sf::Color const fill_color, float const posit_x,
                       float const posit_y) {
  text_name.setFont(font);
  text_name.setCharacterSize(char_size);
  if (outl_thickness != 0.f) {
    text_name.setOutlineThickness(outl_thickness);
    text_name.setOutlineColor(sf::Color::Black);
  }
  text_name.setString(text_to_display);
  text_name.setFillColor(fill_color);
  text_name.setOrigin(text_name.getGlobalBounds().width / 2.f, text_name.getGlobalBounds().height / 2.f);
  text_name.setPosition(posit_x, posit_y);
}


void Button::draw(sf::RenderWindow& window) const { window.draw(shape_); }

// Si definisce la funzione membro update dei pulsanti che permette nel game
// loop di far variare il colore di questi in base a come vi si interagisce e di
// far variare i parametri del flock a seconda di quali pulsanti vengono
// clickati (enumeration all'inizio di graphics.hpp)
void Button::update(Flock& flock, sf::RenderWindow& window,
                    int const click_state, double& vision, sf::Text& text,
                    double const display_height,
                    double const menu_rectangle_width,
                    double const menu_rectangle_height) {
  // la condizione necessita che il mouse si trovi dentro i bordi del pulsante e
  // che il mouse sia stato clickato
  if ((shape_.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,
                                         sf::Mouse::getPosition(window).y)) &&
      click_state == clicked) {
    shape_.setFillColor(active_color_);
    switch (parameter_index_) {
      case sep:
        flock.set_sep(flock.get_sep() + increment_);
        // si vanno ad imporre in questo e negli altri 3 parametri dei limiti ai
        // valori che possono essere assunti
        if (flock.get_sep() > 10.) {
          flock.set_sep(10.);
        } else if (flock.get_sep() < 0.) {
          flock.set_sep(0.);
        }
        // viene aggiornato l'oggetto testo relativo al parametro
        text.setString(roundto(flock.get_sep(), 2));
        // una volta aggiornato il testo, in base alla lunghezza di questo
        // e quindi in base alle dimensioni del rettangolo relativo al testo,
        // viene aggiornata l'origine del rettangolo relativo al testo, ovvero
        // il punto in cui vengo applicate le trasformazioni come spostamenti,
        // rotazioni ecc... in modo che la posizione della casella non risulti
        // mai decentrata se varia la lunghezza del testo
        text.setOrigin(text.getGlobalBounds().width / 2.f,
                       text.getGlobalBounds().height / 2.f);
        text.setPosition(
            menu_rectangle_width * (13.f / 80.f),
            (display_height - menu_rectangle_height * (5.f / 11.f)));
        break;
      case ali:
        flock.set_ali(flock.get_ali() + increment_);
        if (flock.get_ali() > 10.) {
          flock.set_ali(10.);
        } else if (flock.get_ali() < 0.) {
          flock.set_ali(0.);
        }
        text.setString(roundto(flock.get_ali(), 2));
        text.setOrigin(text.getGlobalBounds().width / 2.f,
                       text.getGlobalBounds().height / 2.f);
        text.setPosition(
            menu_rectangle_width * (33.f / 80.f),
            (display_height - menu_rectangle_height * (5.f / 11.f)));
        break;
      case coh:
        flock.set_coe(flock.get_coe() + increment_);
        if (flock.get_coe() > 10.) {
          flock.set_coe(10.);
        } else if (flock.get_coe() < 0.) {
          flock.set_coe(0.);
        }
        text.setString(roundto(flock.get_coe(), 2));
        text.setOrigin(text.getGlobalBounds().width / 2.f,
                       text.getGlobalBounds().height / 2.f);
        text.setPosition(
            menu_rectangle_width * (53.f / 80.f),
            (display_height - menu_rectangle_height * (5.f / 11.f)));
        break;
      case vis:
        vision += increment_;
        if (vision > 2.) {
          vision = 2.;
        } else if (vision < 0.) {
          vision = 0.;
        }
        text.setString(roundto(vision, 2));
        break;
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

// Funzione che aggiorna i dati statistici con i valori correnti quando viene
// chiamata
void statistics_update(Flock& stormo, double& mean_dis, sf::Text& mean_dis_text,
                       double& std_dev_dis, sf::Text& std_dev_dis_text,
                       double& mean_vel, sf::Text& mean_vel_text,
                       double& std_dev_vel, sf::Text& std_dev_vel_text,
                       double const display_width, double const display_height,
                       double const stat_rectangle_width,
                       double const stat_rectangle_height) {
  mean_dis = stormo.mean_distance();
  // vengono aggiornate anche le relative caselle di testo
  mean_dis_text.setString(roundto(mean_dis, 1));
  // come per l'update dei pulsanti viene settata l'origine locale del
  // rettangolo relativo alla casella di testo ogni volta che questa viene
  // cambiata, in modo da mantenere sempre il testo centrato
  mean_dis_text.setOrigin(mean_dis_text.getGlobalBounds().width / 2.f,
                          mean_dis_text.getGlobalBounds().height / 2.f);
  // la casella viene riposizionata una volta che è stata cambiata l'origine
  mean_dis_text.setPosition(
      display_width - stat_rectangle_width * (3.f / 5.f),
      (display_height - stat_rectangle_height * (17.f / 30.f)));
  std_dev_dis = stormo.stnd_deviation_distance(mean_dis);
  std_dev_dis_text.setString(roundto(std_dev_dis, 1));
  std_dev_dis_text.setOrigin(std_dev_dis_text.getGlobalBounds().width / 2.f,
                             std_dev_dis_text.getGlobalBounds().height / 2.f);
  std_dev_dis_text.setPosition(
      display_width - stat_rectangle_width * (3.f / 5.f),
      (display_height - stat_rectangle_height * (7.f / 30.f)));

  mean_vel = stormo.mean_velocity();
  mean_vel_text.setString(roundto(mean_vel, 1));
  mean_vel_text.setOrigin(mean_vel_text.getGlobalBounds().width / 2.f,
                          mean_vel_text.getGlobalBounds().height / 2.f);
  mean_vel_text.setPosition(
      display_width - stat_rectangle_width * (1.f / 5.f),
      (display_height - stat_rectangle_height * (17.f / 30.f)));
  std_dev_vel = stormo.stnd_deviation_velocity(mean_vel);
  std_dev_vel_text.setString(roundto(std_dev_vel, 1));
  std_dev_vel_text.setOrigin(std_dev_vel_text.getGlobalBounds().width / 2.f,
                             std_dev_vel_text.getGlobalBounds().height / 2.f);
  std_dev_vel_text.setPosition(
      display_width - stat_rectangle_width * (1.f / 5.f),
      (display_height - stat_rectangle_height * (7.f / 30.f)));
}


// funzione che verrà poi chiamata nel main è che ha il principale scopo di
// definire gli oggetti grafici e di eseguire il game loop
void graphics(Flock& stormo) {
  auto const delta_t{sf::milliseconds(1)};
  double const dt{delta_t.asSeconds()};
  int const fps{25};
  int const steps_per_evolution{200 / fps};

  double dist_mult{1.};

  FPS fps_count;

  sf::RenderWindow window(sf::VideoMode(display_width, display_height),
                          "Flock Simulation", sf::Style::Default);

  window.setFramerateLimit(fps);

  // boid designing
  sf::ConvexShape boid_shape;
  std::vector<sf::Vector2f> boid_shape_vector{
      sf::Vector2f(5, 12), sf::Vector2f(1, 14),  sf::Vector2f(0, 10),
      sf::Vector2f(5, 0),  sf::Vector2f(10, 10), sf::Vector2f(9, 14)};
  //per aumentare la size diminuire il numero sottostante (è il numero di boids che stanno per largo nella finestra)
  float const boid_scale_const = 165;
  float const boid_scale{display_width/(boid_scale_const * (boid_shape_vector[4].x - boid_shape_vector[2].x))};
  shape_init_setting(boid_shape, boid_shape_vector, 0.f, boid_scale, colored,
                     sf::Color::Black);

  // preparazione dello sfondo
  sf::Texture background;
  // il metodo loadfromfile lancia un'eccezione se non riesce a caricare il file
  background.loadFromFile("./boid_utilities/img/background.png");
  sf::Sprite background_sprite(background);
  background_sprite.setScale(display_width / background_sprite.getLocalBounds().width, display_height / background_sprite.getLocalBounds().height);

  // preparazione delle texture per l'animazione
  sf::Texture boom_texture;
  boom_texture.loadFromFile(
      "./boid_utilities/sheet_sprites/boom_sheetsprite.png");
  // il rettangolo rect_boom_sprite è il rettangolo (che si sposterà lungo lo
  // sheet sprite ad ogni frame) che dice allo sprite dell'esplosione quale
  // texture assumere
  // nel costruttore si danno le coordinate del punto in alto a sinistra e del
  // punto in basso a destra del rettangolo. 448 è la larghezza dell' intero
  // sheet sprite (che contiene 8 frames)
  sf::IntRect rect_boom_sprite(0, 0, 448 / 8, 56);
  sf::Sprite boom_sprite(boom_texture, rect_boom_sprite);
  boom_sprite.setOrigin(448 / 16, 28);

  ///////////////// PARTE DEL MENÙ CON I PULSANTI

  // rettangolo del menu
  sf::Color menu_color(149, 149, 149, 255);
  float const menu_rectangle_width{display_width * (3.f / 5.f)};
  float const menu_rectangle_height{display_height / 10.f};
  sf::RectangleShape menu_rectangle;
  rect_init_setting(menu_rectangle, menu_rectangle_width, menu_rectangle_height,
                    -5.f, menu_color, bottom_left_origin, 0.f, display_height);

  // rettangolo per il display statistico
  float const stat_rectangle_width{menu_rectangle_width * (3.f / 5.f)};
  float const stat_rectangle_height{menu_rectangle_height * (3.f / 2.f)};
  sf::RectangleShape stat_rectangle;
  rect_init_setting(stat_rectangle, stat_rectangle_width, stat_rectangle_height,
                    -5.f, menu_color, bottom_right_origin, display_width,
                    display_height);

  // rettangoli per il testo dei parametri
  sf::Color text_color(sf::Color::White);
  float const text_rectangle_width{menu_rectangle_width * (8.f / 60.f)};
  float const text_rectangle_height{menu_rectangle_height * (1.f / 3.f)};

  sf::RectangleShape sep_text_rectangle;
  rect_init_setting(sep_text_rectangle, text_rectangle_width,
                    text_rectangle_height, 0.f, text_color, center_origin,
                    menu_rectangle_width * (13.f / 80.f),
                    display_height - menu_rectangle_height * (2.f / 5.f));

  sf::RectangleShape ali_text_rectangle;
  rect_init_setting(ali_text_rectangle, text_rectangle_width,
                    text_rectangle_height, 0.f, text_color, center_origin,
                    menu_rectangle_width * (33.f / 80.f),
                    display_height - menu_rectangle_height * (2.f / 5.f));

  sf::RectangleShape coh_text_rectangle;
  rect_init_setting(coh_text_rectangle, text_rectangle_width,
                    text_rectangle_height, 0.f, text_color, center_origin,
                    menu_rectangle_width * (53.f / 80.f),
                    display_height - menu_rectangle_height * (2.f / 5.f));

  sf::RectangleShape vis_text_rectangle;
  rect_init_setting(vis_text_rectangle, text_rectangle_width,
                    text_rectangle_height, 0.f, text_color, center_origin,
                    menu_rectangle_width * (73.f / 80.f),
                    display_height - menu_rectangle_height * (2.f / 5.f));

  // rettangoli per il testo dei dati statistici
  sf::RectangleShape mean_dis_text_rectangle;
  rect_init_setting(mean_dis_text_rectangle, text_rectangle_width,
                    text_rectangle_height, 0.f, text_color, center_origin,
                    display_width - stat_rectangle_width * (3.f / 5.f),
                    display_height - stat_rectangle_height * (3.f / 15.f));

  sf::RectangleShape std_dev_dis_text_rectangle;
  rect_init_setting(std_dev_dis_text_rectangle, text_rectangle_width,
                    text_rectangle_height, 0.f, text_color, center_origin,
                    display_width - stat_rectangle_width * (3.f / 5.f),
                    display_height - stat_rectangle_height * (8.f / 15.f));

  sf::RectangleShape mean_vel_text_rectangle;
  rect_init_setting(mean_vel_text_rectangle, text_rectangle_width,
                    text_rectangle_height, 0.f, text_color, center_origin,
                    display_width - stat_rectangle_width * (1.f / 5.f),
                    display_height - stat_rectangle_height * (3.f / 15.f));

  sf::RectangleShape std_dev_vel_text_rectangle;
  rect_init_setting(std_dev_vel_text_rectangle, text_rectangle_width,
                    text_rectangle_height, 0.f, text_color, center_origin,
                    display_width - stat_rectangle_width * (1.f / 5.f),
                    display_height - stat_rectangle_height * (8.f / 15.f));

  ///// pulsanti
  float const button_outl_thickness{-0.2f};

  // forme dei pulsanti
  sf::ConvexShape up_but;
  std::vector<sf::Vector2f> up_but_vector{
      sf::Vector2f(1, 0), sf::Vector2f(2, 2), sf::Vector2f(0, 2)};
  //per aumentare la size diminuire il numero sottostante (è il numero di bottoni che stanno per largo nella finestra)
  float const button_scale_const = 75;
  float const button_scale{display_width/(button_scale_const * (up_but_vector[1].x - up_but_vector[2].x))};
  shape_init_setting(up_but, up_but_vector, button_outl_thickness, button_scale,
                     uncolored, sf::Color::Black);

  sf::ConvexShape down_but;
  std::vector<sf::Vector2f> down_vector{sf::Vector2f(0, 0), sf::Vector2f(2, 0),
                                        sf::Vector2f(1, 2)};
  shape_init_setting(down_but, down_vector, button_outl_thickness, button_scale,
                     uncolored, sf::Color::Black);

  sf::ConvexShape double_up_but;
  std::vector<sf::Vector2f> dou_up__but_vector{
      sf::Vector2f(1, 0), sf::Vector2f(2, 1), sf::Vector2f(1, 1),
      sf::Vector2f(2, 2), sf::Vector2f(0, 2), sf::Vector2f(1, 1),
      sf::Vector2f(0, 1)};
  shape_init_setting(double_up_but, dou_up__but_vector, button_outl_thickness,
                     button_scale, uncolored, sf::Color::Black);

  sf::ConvexShape double_down_but;
  std::vector<sf::Vector2f> dou_down_but_vector{
      sf::Vector2f(0, 0), sf::Vector2f(2, 0), sf::Vector2f(1, 1),
      sf::Vector2f(2, 1), sf::Vector2f(1, 2), sf::Vector2f(0, 1),
      sf::Vector2f(1, 1)};
  shape_init_setting(double_down_but, dou_down_but_vector,
                     button_outl_thickness, button_scale, uncolored,
                     sf::Color::Black);

  // colori dei pulsanti
  sf::Color sep_idle{240, 67, 67, 255};  // ROSSO
  sf::Color sep_semiactive{158, 24, 24, 255};
  sf::Color sep_active{94, 9, 9, 255};
  sf::Color ali_idle{82, 219, 61, 255};  // VERDE
  sf::Color ali_semiactive{34, 143, 17, 255};
  sf::Color ali_active{17, 87, 6, 255};
  sf::Color coh_idle{77, 102, 232, 255};  // BLU
  sf::Color coh_semiactive{17, 38, 143, 255};
  sf::Color coh_active{11, 26, 102, 255};
  sf::Color vis_idle{231, 237, 62, 255};  // GIALLO
  sf::Color vis_semiactive{176, 181, 29, 255};
  sf::Color vis_active{119, 122, 9, 255};

  // inizializzazione degli oggetti pulsanti
  Button sep_up{menu_rectangle_width * (3.f / 80.f),
                (display_height - menu_rectangle_height * (3.f / 4.f)),
                up_but,
                sep_idle,
                sep_semiactive,
                sep_active,
                0.25,
                sep};
  Button sep_down{menu_rectangle_width * (3.f / 80.f),
                  (display_height - menu_rectangle_height * (1.f / 4.f)),
                  down_but,
                  sep_idle,
                  sep_semiactive,
                  sep_active,
                  -0.25,
                  sep};
  Button sep_dou_up{menu_rectangle_width * (6.f / 80.f),
                    (display_height - menu_rectangle_height * (3.f / 4.f)),
                    double_up_but,
                    sep_idle,
                    sep_semiactive,
                    sep_active,
                    2,
                    sep};
  Button sep_dou_down{menu_rectangle_width * (6.f / 80.f),
                      (display_height - menu_rectangle_height * (1.f / 4.f)),
                      double_down_but,
                      sep_idle,
                      sep_semiactive,
                      sep_active,
                      -2,
                      sep};

  Button ali_up{menu_rectangle_width * (23.f / 80.f),
                (display_height - menu_rectangle_height * (3.f / 4.f)),
                up_but,
                ali_idle,
                ali_semiactive,
                ali_active,
                0.25,
                ali};
  Button ali_down{menu_rectangle_width * (23.f / 80.f),
                  (display_height - menu_rectangle_height * (1.f / 4.f)),
                  down_but,
                  ali_idle,
                  ali_semiactive,
                  ali_active,
                  -0.25,
                  ali};
  Button ali_dou_up{menu_rectangle_width * (26.f / 80.f),
                    (display_height - menu_rectangle_height * (3.f / 4.f)),
                    double_up_but,
                    ali_idle,
                    ali_semiactive,
                    ali_active,
                    2,
                    ali};
  Button ali_dou_down{menu_rectangle_width * (26.f / 80.f),
                      (display_height - menu_rectangle_height * (1.f / 4.f)),
                      double_down_but,
                      ali_idle,
                      ali_semiactive,
                      ali_active,
                      -2,
                      ali};

  Button coh_up{menu_rectangle_width * (43.f / 80.f),
                (display_height - menu_rectangle_height * (3.f / 4.f)),
                up_but,
                coh_idle,
                coh_semiactive,
                coh_active,
                0.25,
                coh};
  Button coh_down{menu_rectangle_width * (43.f / 80.f),
                  (display_height - menu_rectangle_height * (1.f / 4.f)),
                  down_but,
                  coh_idle,
                  coh_semiactive,
                  coh_active,
                  -0.25,
                  coh};
  Button coh_dou_up{menu_rectangle_width * (46.f / 80.f),
                    (display_height - menu_rectangle_height * (3.f / 4.f)),
                    double_up_but,
                    coh_idle,
                    coh_semiactive,
                    coh_active,
                    2,
                    coh};
  Button coh_dou_down{menu_rectangle_width * (46.f / 80.f),
                      (display_height - menu_rectangle_height * (1.f / 4.f)),
                      double_down_but,
                      coh_idle,
                      coh_semiactive,
                      coh_active,
                      -2,
                      coh};

  Button vis_up{menu_rectangle_width * (63.f / 80.f),
                (display_height - menu_rectangle_height * (3.f / 4.f)),
                up_but,
                vis_idle,
                vis_semiactive,
                vis_active,
                0.05,
                vis};
  Button vis_down{menu_rectangle_width * (63.f / 80.f),
                  (display_height - menu_rectangle_height * (1.f / 4.f)),
                  down_but,
                  vis_idle,
                  vis_semiactive,
                  vis_active,
                  -0.05,
                  vis};
  Button vis_dou_up{menu_rectangle_width * (66.f / 80.f),
                    (display_height - menu_rectangle_height * (3.f / 4.f)),
                    double_up_but,
                    vis_idle,
                    vis_semiactive,
                    vis_active,
                    0.2,
                    vis};
  Button vis_dou_down{menu_rectangle_width * (66.f / 80.f),
                      (display_height - menu_rectangle_height * (1.f / 4.f)),
                      double_down_but,
                      vis_idle,
                      vis_semiactive,
                      vis_active,
                      -0.2,
                      vis};

  std::array<Button, 4> sep_buttons{sep_up, sep_down, sep_dou_up, sep_dou_down};
  std::array<Button, 4> ali_buttons{ali_up, ali_down, ali_dou_up, ali_dou_down};
  std::array<Button, 4> coh_buttons{coh_up, coh_down, coh_dou_up, coh_dou_down};
  std::array<Button, 4> vis_buttons{vis_up, vis_down, vis_dou_up, vis_dou_down};

  // testi
  sf::Font font;
  font.loadFromFile("./boid_utilities/fonts/arial.ttf");

  // testi titoli parametri
  sf::Text sep_title_text;
  text_init_setting(sep_title_text, font, 16, 1.f, "Separation", sep_idle,
                    menu_rectangle_width * (13.f / 80.f),
                    display_height - menu_rectangle_height * (9.f / 11.f));

  sf::Text ali_title_text;
  text_init_setting(ali_title_text, font, 16, 1.f, "Alignment", ali_idle,
                    menu_rectangle_width * (33.f / 80.f),
                    display_height - menu_rectangle_height * (9.f / 11.f));

  sf::Text coh_title_text;
  text_init_setting(coh_title_text, font, 16, 1.f, "Cohesion", coh_idle,
                    menu_rectangle_width * (53.f / 80.f),
                    display_height - menu_rectangle_height * (9.f / 11.f));

  sf::Text vis_title_text;
  text_init_setting(vis_title_text, font, 16, 1.f, "Vision", vis_idle,
                    menu_rectangle_width * (73.f / 80.f),
                    display_height - menu_rectangle_height * (9.f / 11.f));

  // testi titoli dati statistici
  sf::Text boid_distance_title_text;
  text_init_setting(boid_distance_title_text, font, 16, 0.f, "Boids Distance",
                    sf::Color::Black,
                    display_width - stat_rectangle_width * (3.f / 5.f),
                    display_height - stat_rectangle_height * (13.f / 15.f));

  sf::Text boid_vel_title_text;
  text_init_setting(boid_vel_title_text, font, 16, 0.f, "Boids Velocity",
                    sf::Color::Black,
                    display_width - stat_rectangle_width * (1.f / 5.f),
                    display_height - stat_rectangle_height * (13.f / 15.f));

  sf::Text mean_title_text;
  text_init_setting(mean_title_text, font, 16, 0.f, "Mean:", sf::Color::Black,
                    display_width - stat_rectangle_width * (141.f / 160.f),
                    display_height - stat_rectangle_height * (17.f / 30.f));

  sf::Text std_dev_title_text;
  text_init_setting(std_dev_title_text, font, 16, 0.f,
                    "Stnd. Dev:", sf::Color::Black,
                    display_width - stat_rectangle_width * (136.f / 160.f),
                    display_height - stat_rectangle_height * (7.f / 30.f));

  // testi parametri
  sf::Text sep_text;
  text_init_setting(sep_text, font, 14, 0.f, roundto(stormo.get_sep(), 2),
                    sf::Color::Black, menu_rectangle_width * (13.f / 80.f),
                    display_height - menu_rectangle_height * (5.f / 11.f));

  sf::Text ali_text;
  text_init_setting(ali_text, font, 14, 0.f, roundto(stormo.get_ali(), 2),
                    sf::Color::Black, menu_rectangle_width * (33.f / 80.f),
                    display_height - menu_rectangle_height * (5.f / 11.f));

  sf::Text coh_text;
  text_init_setting(coh_text, font, 14, 0.f, roundto(stormo.get_coe(), 2),
                    sf::Color::Black, menu_rectangle_width * (53.f / 80.f),
                    display_height - menu_rectangle_height * (5.f / 11.f));

  sf::Text vis_text;
  text_init_setting(vis_text, font, 14, 0.f, roundto(dist_mult, 2),
                    sf::Color::Black, menu_rectangle_width * (73.f / 80.f),
                    display_height - menu_rectangle_height * (5.f / 11.f));

  // testi dati statistici
  sf::Text mean_dis_text;
  double mean_dis{stormo.mean_distance()};
  text_init_setting(mean_dis_text, font, 14, 0.f, roundto(mean_dis, 1),
                    sf::Color::Black,
                    display_width - stat_rectangle_width * (3.f / 5.f),
                    display_height - stat_rectangle_height * (17.f / 30.f));

  sf::Text std_dev_dis_text;
  double std_dev_dis{stormo.stnd_deviation_distance(mean_dis)};
  text_init_setting(std_dev_dis_text, font, 14, 0.f, roundto(std_dev_dis, 1),
                    sf::Color::Black,
                    display_width - stat_rectangle_width * (3.f / 5.f),
                    display_height - stat_rectangle_height * (7.f / 30.f));

  sf::Text mean_vel_text;
  double mean_vel{stormo.mean_velocity()};
  text_init_setting(mean_vel_text, font, 14, 0.f, roundto(mean_vel, 1),
                    sf::Color::Black,
                    display_width - stat_rectangle_width * (1.f / 5.f),
                    display_height - stat_rectangle_height * (17.f / 30.f));

  sf::Text std_dev_vel_text;
  double std_dev_vel{stormo.stnd_deviation_velocity(mean_vel)};
  text_init_setting(std_dev_vel_text, font, 14, 0.f, roundto(std_dev_vel, 1),
                    sf::Color::Black,
                    display_width - stat_rectangle_width * (1.f / 5.f),
                    display_height - stat_rectangle_height * (7.f / 30.f));

  // inizializzazione  di altre variabili utili nel game loop
  // il click_state sarà la variabile che daremo come parametro alle funzioni
  // che dipendono dai click del mouse
  int click_state{unclicked};

  sf::Clock animation_clock;
  sf::Clock statistics_clock;

  // l'animazione dello scoppio sarà riprodotta continuamente, ma fuori dalla
  // finestra
  boom_positionx = 10000;  // inizializzate in velocity.hpp
  boom_positiony = 10000;

  int animation_index{0};

  /*
  //Suoni
  sf::SoundBuffer boom_sound_buffer;
  boom_sound_buffer.loadFromFile("./boid_utilities/audio/boom_sound.ogg");
  sf::Sound boom_sound;
  boom_sound.setBuffer(boom_sound_buffer);
  */

  // GAME LOOP

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == sf::Event::MouseButtonReleased) {
        click_state = clicked;
      }
    }

    // if che triggera l'esplosione del petardo
    if (click_state == clicked &&
        animation_clock.getElapsedTime().asSeconds() > 1.f &&
        !(menu_rectangle.getGlobalBounds().contains(
            sf::Mouse::getPosition(window).x,
            sf::Mouse::getPosition(window).y)) &&
        !(stat_rectangle.getGlobalBounds().contains(
            sf::Mouse::getPosition(window).x,
            sf::Mouse::getPosition(window).y))) {
      sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
      // l'esplosione viene spostata in prossimità del punto clickato
      boom_positionx = mousePosition.x;
      boom_positiony = mousePosition.y;
      animation_clock.restart();
      // poiché l'animazioni (che viene eseguita di continua) viene spostata da
      // fuori la finestra al punto clickato ci si assicura che riparta
      // dall'inizio
      animation_index = 0;
      // boom_sound.play();
    }

    // dopo una certa quantità di tempo l'esplosione viene riportata fuori dallo
    // schermo e quindi anche fuori dalla portata dei boids
    if (animation_clock.getElapsedTime().asSeconds() > 1.f &&
        boom_positionx != 10000 && boom_positiony != 10000) {
      boom_positionx = 10000;
      boom_positiony = 10000;
    }

    // il rettangolo che indica allo sprite dell'esplosione quale texture
    // assumere viene spostato un frame a destra nello sprite_sheet
    rect_boom_sprite.left = animation_index * 56;
    assert(rect_boom_sprite.left <= 392);
    boom_sprite.setTextureRect(rect_boom_sprite);
    boom_sprite.setPosition(boom_positionx, boom_positiony);

    // aggiornamento dei pulsanti
    for (auto& button : sep_buttons) {
      button.update(stormo, window, click_state, dist_mult, sep_text,
                    display_height, menu_rectangle_width,
                    menu_rectangle_height);
    }
    for (auto& button : ali_buttons) {
      button.update(stormo, window, click_state, dist_mult, ali_text,
                    display_height, menu_rectangle_width,
                    menu_rectangle_height);
    }
    for (auto& button : coh_buttons) {
      button.update(stormo, window, click_state, dist_mult, coh_text,
                    display_height, menu_rectangle_width,
                    menu_rectangle_height);
    }
    for (auto& button : vis_buttons) {
      button.update(stormo, window, click_state, dist_mult, vis_text,
                    display_height, menu_rectangle_width,
                    menu_rectangle_height);
    }

    // aggiornamento del flock
    update(stormo, steps_per_evolution, dt, dist_mult);

    // aggiornamento dei dati statistici
    if (statistics_clock.getElapsedTime().asSeconds() > 3.f) {
      statistics_update(stormo, mean_dis, mean_dis_text, std_dev_dis,
                        std_dev_dis_text, mean_vel, mean_vel_text, std_dev_vel,
                        std_dev_vel_text, display_width, display_height,
                        stat_rectangle_width, stat_rectangle_height);
      statistics_clock.restart();
    }

    window.clear();

    // draw del background
    window.draw(background_sprite);

    // draw del flock
    for (auto& boid : stormo.get_flock()) {
      double angle = orientation(boid.get_vx(), boid.get_vy());
      boid_shape.setRotation(-angle);

      boid_shape.setPosition(boid.get_x(), boid.get_y());

      window.draw(boid_shape);
    }

    // draw del frame corrente dell'animazione dello scoppio
    window.draw(boom_sprite);

    // si inizia a disegnare l'interfaccia (il piccolo menù con i pulsanti)
    window.draw(menu_rectangle);
    window.draw(stat_rectangle);

    // draw dei rettangoli relativi alle caselle di testo
    // parametri
    window.draw(sep_text_rectangle);
    window.draw(ali_text_rectangle);
    window.draw(coh_text_rectangle);
    window.draw(vis_text_rectangle);
    // analisi statistica
    window.draw(mean_dis_text_rectangle);
    window.draw(std_dev_dis_text_rectangle);
    window.draw(mean_vel_text_rectangle);
    window.draw(std_dev_vel_text_rectangle);

    // draw dei testi
    // titoli parametri
    window.draw(sep_title_text);
    window.draw(ali_title_text);
    window.draw(coh_title_text);
    window.draw(vis_title_text);
    // titoli analisi statistica
    window.draw(boid_distance_title_text);
    window.draw(boid_vel_title_text);
    window.draw(mean_title_text);
    window.draw(std_dev_title_text);
    // parametri
    window.draw(sep_text);
    window.draw(ali_text);
    window.draw(coh_text);
    window.draw(vis_text);
    // analisi statistica
    window.draw(mean_dis_text);
    window.draw(std_dev_dis_text);
    window.draw(mean_vel_text);
    window.draw(std_dev_vel_text);

    // draw dei pulsanti
    for (auto& button : sep_buttons) {
      button.draw(window);
    }
    for (auto& button : ali_buttons) {
      button.draw(window);
    }
    for (auto& button : coh_buttons) {
      button.draw(window);
    }
    for (auto& button : vis_buttons) {
      button.draw(window);
    }

    window.display();

    click_state = unclicked;

    ++animation_index;
    if (animation_index == 8) {
      animation_index = 0;
    }

    fps_count.update();
    std::ostringstream ss;
    ss << fps_count.getFPS();

    window.setTitle(ss.str());
  }
}
