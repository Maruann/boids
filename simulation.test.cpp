#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "flock.hpp"
#include "graphics.hpp"
#include "simulation.hpp"
#include "velocity.hpp"

// Questa √® la TU su cui si basa l'eseguibile dei test. Sono state testate le
// funzionalit√† delle TU che costituiscono la struttura del programma, in ordine
// dalle basi fino alla funzione update. I metodi statistici sono stati testati
// in fondo.

// Test sulla classe Boid
TEST_CASE("Test sulle funzionalit√† dei boids") {
  Boid boid{1, 2, 3, 4};
  SUBCASE("Test dei metodi get") {
    CHECK(boid.get_x() == 1);
    CHECK(boid.get_y() == 2);
    CHECK(boid.get_vx() == 3);
    CHECK(boid.get_vy() == 4);
  }
  SUBCASE("Test dei metodi set") {
    boid.set_x(11);
    boid.set_y(12);
    boid.set_vx(13);
    boid.set_vy(14);
    CHECK(boid.get_x() == 11);
    CHECK(boid.get_y() == 12);
    CHECK(boid.get_vx() == 13);
    CHECK(boid.get_vy() == 14);
  }
  SUBCASE("Test dell'operatore ==") {
    Boid const boid2{1, 2, 3, 4};
    Boid const boid3{2, 2, 3, 4};
    CHECK(boid == boid2);
    CHECK(!(boid == boid3));
  }
};

// Verifica del corretto funzionamento delle funzioni dedite alla generazione di
// numeri pseudo-casuali. I test si basano sulla creazione di coppie di numeri
// casuali, testarne che il range sia corretto e che siano diversi tra loro.
TEST_CASE("Test delle funzioni random"){
    SUBCASE("Test della funzione random per le x"){
        double const a{r_position_x()};
double const b{r_position_x()};
CHECK(a != b);
bool const check1{a >= bound_xmin && a <= bound_xmax};
CHECK(check1);
bool const check2{b >= bound_xmin && b <= bound_xmax};
CHECK(check2);
}
SUBCASE("Test della funzione random per le y") {
  double const a{r_position_y()};
  double const b{r_position_y()};
  CHECK(a != b);
  bool const check1{a >= bound_ymin && a <= bound_ymax};
  CHECK(check1);
  bool const check2{b >= bound_ymin && b <= bound_ymax};
  CHECK(check2);
}
SUBCASE("Test della funzione segno") {
  int const a{r_sign()};
  bool const check1{a == 1 || a == -1};
  CHECK(check1);
}
SUBCASE("Test della funzione random per le velocit√† iniziali") {
  double const a{r_velocity()};
  bool const check1{std::fabs(a) <= initial_vel &&
                    std::fabs(a) >= (initial_vel - 500.)};
  CHECK(check1);
}
}
;

// Sono testati i metodi base della classe Flock. I test sono basati sulla
// verifica del corretto funzionamento dei vari set e get.
TEST_CASE("Test sulle funzioni dello stormo") {
  Flock stormo{5, 5, 5};

  CHECK(stormo.get_flock().empty());

  SUBCASE("Test del fill") {
    stormo.fill(20);
    CHECK(stormo.get_flock().size() == 20);
  }
  SUBCASE("Test dei metodi get per i parametri") {
    CHECK(stormo.get_sep() == 5);
    CHECK(stormo.get_ali() == 5);
    CHECK(stormo.get_coe() == 5);
  }
  SUBCASE("Test dei metodi set per i parametri") {
    stormo.set_sep(6);
    stormo.set_ali(7);
    stormo.set_coe(8);
    CHECK(stormo.get_sep() == 6);
    CHECK(stormo.get_ali() == 7);
    CHECK(stormo.get_coe() == 8);
  }
  SUBCASE("Test del metodo set per il vettorei di boids") {
    Boid const boid1{1, 1, 1, 1};
    Boid const boid2{2, 2, 2, 2};
    Boid const boid3{3, 3, 3, 3};
    std::vector<Boid> const flock{boid1, boid2, boid3};
    stormo.set_flock(flock);
    for (int i{0}; i < 3; ++i) CHECK(flock[i] == stormo.get_flock()[i]);
  }
};
// Sono testati i metodi responsabili per calcolo delle velocit√† d'interazione
// tra i boids. Data la complessit√† delle formule sono state verificate solo
// l'attivazione e il verso, ma non il valore esatto dell'intensit√†.
TEST_CASE("Test per i metodi di calcolo per le velocit√† d'interazione") {
  Flock stormo{1, 1, 1};
  Boid const boid1{10, 10, 0, 0};
  Boid const boid2{20, 20, 1, -1};
  std::vector<Boid> const flock{boid1, boid2};
  stormo.set_flock(flock);
  SUBCASE("Test per la v di separazione") {
    // Controllo che sia presente e nel verso giusto quando √® in range e assente
    // quando no
    CHECK(stormo.vx_repulsive(100, stormo.get_flock()[0]) < 0);
    CHECK(stormo.vx_repulsive(5, stormo.get_flock()[0]) == 0);
    CHECK(stormo.vy_repulsive(100, stormo.get_flock()[0]) < 0);
    CHECK(stormo.vy_repulsive(5, stormo.get_flock()[0]) == 0);
  }
  SUBCASE("Test per la v di allineamento") {
    // Controllo che sia assente se non raggiunge il range minimo d'attivazione
    CHECK(stormo.vx_alignment(100, 20, stormo.get_flock()[0]) == 0);
    // Controllo che sia assente se eccede il range massimo d'attivazione
    CHECK(stormo.vx_alignment(8, 2, stormo.get_flock()[0]) == 0);
    // Controllo sia presente e nel verso giusto quando in range
    CHECK(stormo.vx_alignment(100, 2, stormo.get_flock()[0]) > 0);
    // Controllo che sia assente se non raggiunge il range minimo d'attivazione
    CHECK(stormo.vy_alignment(100, 20, stormo.get_flock()[0]) == 0);
    // Controllo che sia assente se eccede il range massimo d'attivazione
    CHECK(stormo.vy_alignment(8, 2, stormo.get_flock()[0]) == 0);
    // Controllo sia presente e nel verso giusto quando in range
    CHECK(stormo.vy_alignment(100, 2, stormo.get_flock()[0]) < 0);
  }
  SUBCASE("Test per la v di coesione") {
    // Controllo che sia assente se non raggiunge il range minimo d'attivazione
    CHECK(stormo.vx_cohesion(80, 30, stormo.get_flock()[0]) == 0);
    // Controllo che sia assente se eccede il range massimo d'attivazione
    CHECK(stormo.vx_cohesion(5, 2, stormo.get_flock()[0]) == 0);
    // Controllo sia presente e nel verso giusto quando in range
    CHECK(stormo.vx_cohesion(50, 2, stormo.get_flock()[0]) > 0);
    // Controllo che sia assente se non raggiunge il range minimo d'attivazione
    CHECK(stormo.vy_cohesion(80, 30, stormo.get_flock()[0]) == 0);
    // Controllo che sia assente se eccede il range massimo d'attivazione
    CHECK(stormo.vy_cohesion(5, 2, stormo.get_flock()[0]) == 0);
    // Controllo sia presente e nel verso giusto quando in range
    CHECK(stormo.vy_cohesion(50, 2, stormo.get_flock()[0]) > 0);
  }
};
// Sono testate le funzioni libere responsabili per il calcolo delle interazioni
// dei boids con gli agenti esteni allo stormo. Anche in questo caso non sono
// state verificate le precise intensit√† delle velocit√†, ma solo verso e
// corretta attivazione.
TEST_CASE("Test per le free-function relative alle velocit√† esterne") {
  Boid const boid{0, 0, -1, -1};
  SUBCASE("Test per la velocit√† di richiamo") {
    // Controllo che sia fuori perimetro
    CHECK(not_in_perimeter_x(boid));
    CHECK(not_in_perimeter_y(boid));
    // Controllo che sia applicata la velocit√† di rietro
    CHECK(p_velx_active(boid));
    CHECK(p_vely_active(boid));
    // Controllo che la velocit√† di rietro sia calcolata con il verso giusto
    CHECK(v_perimeterx(1, boid) > 0);
    CHECK(v_perimetery(1, boid) > 0);
  }
  SUBCASE("Test per la velocit√† di slowdown") {
    // Controllo che la velocit√† di slowdown sia attiva (il boid si sta
    // allontanando dal centro)
    CHECK(p_slowdown_active_x(boid));
    CHECK(p_slowdown_active_y(boid));
  }
  SUBCASE("Test per la velocit√† di fuga") {
    boom_positionx = 1;
    boom_positiony = 1;
    double explosion_range{0.5};
    // Controllo che non sia considerato in range per valori del range troppo
    // piccoli
    CHECK(!(in_explosion_range(boom_positionx, boom_positiony, explosion_range,
                               boid)));
    explosion_range = 25;
    // Controllo che il boid sia considerato in range per opportuni valori
    CHECK(in_explosion_range(boom_positionx, boom_positiony, explosion_range,
                             boid));
    // Controllo che sia calcolata la velocit√† di fuga e che abbia il verso
    // giusto
    CHECK(expl_velocity_x(1, boom_positionx, boid) < 0);
    CHECK(expl_velocity_y(1, boom_positiony, boid) < 0);
  }
};
// Viene verificato il corretto funzionamento di update e orientation. Update √®
// verificata controllando il diverso stato del vettore prima e dopo la sua
// chiamata. Orientation √® testata con una combinazione di 4 casi che si
// assicurano l'angolo sia nel quadrante giusto, a seconda dei segni delle
// velocit√† in x e y del boid.
TEST_CASE("Test per le free-function definite in simulation.cpp") {
  Flock stormo{1, 1, 1};
  SUBCASE("Test nel caso di un boid statico") {
    Boid const boid{bound_xmin + 100., bound_ymin + 100., 0, 0};
    std::vector<Boid> flock{boid};
    stormo.set_flock(flock);
    update(stormo, 1, 1, 1);
    // Controllo che nonostante l'update il singolo boid dentro alla zona neutra
    // dello schermo non si sia mosso
    CHECK(stormo.get_flock()[0] == flock[0]);
  }
  SUBCASE("Test nel caso di uno stormo non statico") {
    int const n{3};
    stormo.fill(n);
    auto flock_iniziale{stormo.get_flock()};
    update(stormo, 1, 1, 1);
    for (int i{0}; i < n; ++i)
      CHECK(!(stormo.get_flock()[i] == flock_iniziale[i]));
  }
  SUBCASE("Test per l'orientamento") {
    Boid const boid{0, 0, r_velocity(), r_velocity()};
    bool check{false};
    if (boid.get_vx() > 0 && boid.get_vy() > 0) {
      if (orientation(boid.get_vx(), boid.get_vy()) < -90 &&
          orientation(boid.get_vx(), boid.get_vy()) > -180) {
        check = true;
      }
    } else if (boid.get_vx() > 0 && boid.get_vy() < 0) {
      if (orientation(boid.get_vx(), boid.get_vy()) < 0 &&
          orientation(boid.get_vx(), boid.get_vy()) > -90)
        check = true;
    } else if (boid.get_vx() < 0 && boid.get_vy() < 0) {
      if (orientation(boid.get_vx(), boid.get_vy()) < 90 &&
          orientation(boid.get_vx(), boid.get_vy()) > 0)
        check = true;
    } else if (boid.get_vx() < 0 && boid.get_vy() > 0) {
      if (orientation(boid.get_vx(), boid.get_vy()) < 180 &&
          orientation(boid.get_vx(), boid.get_vy()) > 90)
        check = true;
    }
    CHECK(check);
  }
};
// Vengono testate le funzioni statistiche, prima nei casi particolari di 1 e 0
// boids, poi nei casi generali n>1.
TEST_CASE("Test per le funzioni di statistica") {
  Flock stormo{1, 1, 1};
  SUBCASE("Test nel caso di uno stormo vuoto") {
    CHECK(stormo.mean_distance() == 0);
    CHECK(stormo.mean_velocity() == 0);
    CHECK(stormo.stnd_deviation_distance(stormo.mean_distance()) == 0);
    CHECK(stormo.stnd_deviation_velocity(stormo.mean_velocity()) == 0);
  }
  SUBCASE("Test nel caso di un boid unico") {
    Boid const boid1{0, 0, 1, 0};
    std::vector<Boid> const flock{boid1};
    stormo.set_flock(flock);
    CHECK(stormo.mean_distance() == 0);
    CHECK(stormo.mean_velocity() == 1);
    CHECK(stormo.stnd_deviation_distance(stormo.mean_distance()) == 0);
    CHECK(stormo.stnd_deviation_velocity(stormo.mean_velocity()) == 0);
  }
  Boid const boid1{0, 0, 0, 0};
  Boid const boid2{2, 2, 2, 2};
  std::vector<Boid> const flock{boid1, boid2};
  stormo.set_flock(flock);
  SUBCASE("Test della della distanza media") {
    CHECK(stormo.mean_distance() == 2 * sqrt(2));
    CHECK(stormo.stnd_deviation_distance(stormo.mean_distance()) < 1);
  }
  SUBCASE("Test della velocit√† media") {
    CHECK(stormo.mean_velocity() == sqrt(2));
    CHECK(stormo.stnd_deviation_velocity(stormo.mean_velocity()) == 2);
  }
};

//Vengono testale le funzioni utilizzate nella parte grafica
TEST_CASE("Test per la funzione di arrotondamento") {
  double num1 = 100.000;
  double num2 = 0;
  SUBCASE("Test arrotondamento ad un numero di decimali minore") {
    CHECK(roundto(num1, 2) == "100.00");
  }
  SUBCASE("Test arrotondamento ad un numero di decimali maggiore") {
    CHECK(roundto(num1, 4) == "100.0000");
  }
  SUBCASE("Test arrotondamento di un numero definito senza il punto") {
    CHECK(roundto(num2, 4) == "0.0000");
  }
};

TEST_CASE("Test per le funzioni di modifica degli oggetti grafici") {
  sf::ConvexShape boid_shape;
  float const boid_scale{0.6f};
  std::vector<sf::Vector2f> boid_shape_vector{
      sf::Vector2f(5, 12), sf::Vector2f(1, 14),  sf::Vector2f(0, 10),
      sf::Vector2f(5, 0),  sf::Vector2f(10, 10), sf::Vector2f(9, 14)};
  shape_init_setting(boid_shape, boid_shape_vector, 0.f, boid_scale, colored,
                     sf::Color::Black);
  SUBCASE("Test per il controllo dell'origine") {
    CHECK(boid_shape.getOrigin() == sf::Vector2f(5, 7));
  }
  SUBCASE("Test per il controllo del colore") {
    CHECK(boid_shape.getFillColor() == sf::Color::Black);
  }
  SUBCASE("Test controllo contorno della formma") {
    CHECK(boid_shape.getOutlineThickness() == 0.f);
  }

  sf::Color color(149, 149, 149, 255);
  float const display_width = 1280;
  float const display_height = 720;
  float const rectangle_width{display_width * (3.f / 5.f)};
  float const rectangle_height{display_height / 10.f};
  sf::RectangleShape rectangle;
  rect_init_setting(rectangle, rectangle_width, rectangle_height, -5.f, color,
                    bottom_left_origin, 0.f, display_height);
  SUBCASE("Test per il controllo del colore") {
    CHECK(rectangle.getFillColor() == color);
  }
  SUBCASE("Test per il controllo dell'origine") {
    CHECK(rectangle.getOrigin() == sf::Vector2f(0, 72));
  }
  SUBCASE("Test per il controllo della posizione") {
    CHECK(rectangle.getPosition() == sf::Vector2f(0, 720));
  }
  rect_init_setting(rectangle, rectangle_width, rectangle_height, -5.f, color,
                    bottom_right_origin, 0.f, display_height);
  SUBCASE("Test per il controllo dell'origine") {
    CHECK(rectangle.getOrigin() == sf::Vector2f(768, 72));
  }
  rect_init_setting(rectangle, rectangle_width, rectangle_height, -5.f, color,
                    center_origin, 0.f, display_height);
  SUBCASE("Test per il controllo dell'origine") {
    CHECK(rectangle.getOrigin() == sf::Vector2f(384, 36));
  }
  

  sf::Font font;
  font.loadFromFile("./boid_utilities/fonts/arial.ttf");
  sf::Text text;
  text_init_setting(text, font, 16, 1.f, "Testo", sf::Color::Black,
                    rectangle_width * (13.f / 80.f),
                    display_height - rectangle_height * (9.f / 12.f));
  SUBCASE("Test per il controllo della size del font") {
    CHECK(text.getCharacterSize() == 16);
  }
  SUBCASE("Test per il controllo del testo") {
    CHECK(text.getString() == "Testo");
  }
  SUBCASE("Test per il controllo del contorno del testo") {
    CHECK(text.getOutlineThickness() == 1.f);
    CHECK(text.getOutlineColor() == sf::Color::Black);
  }
};