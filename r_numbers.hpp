#ifndef R_NUMBERS
#define R_NUMBERS

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Audio.hpp>
#include <chrono>
#include <cmath>
#include <random>

// funzioni per la generazione random di numeri con cui
// inizializzare lo stormo
inline double bound_xmin{sf::VideoMode::getDesktopMode().width / 6.};
inline double bound_xmax{5. * sf::VideoMode::getDesktopMode().width / 6.};
inline double bound_ymin{(sf::VideoMode::getDesktopMode().height / 4.) };
inline double bound_ymax{(3. * sf::VideoMode::getDesktopMode().height / 4.) -
                         100.};

double inline r_position_x() {
  std::uniform_real_distribution<double> dist(
      bound_xmin,
      bound_xmax);  // crea una distribuzione uniforme tra i due limiti
  std::default_random_engine
      eng;  // crea un random engine da dare alla distribuzione
  eng.seed(std::chrono::system_clock::now()
               .time_since_epoch()
               .count());  // cambio il seed leggendo il tempo attuale ad ogni
                           // chiamata
  double const s = dist(eng);
  eng.seed(s);
  double const r = dist(eng);
  return r;
}

double inline r_position_y() {
  std::uniform_real_distribution<double> dist(
      bound_ymin,
      bound_ymax);  // crea una distribuzione uniforme tra i due limiti
  std::default_random_engine
      eng;  // crea un random engine da dare alla distribuzione
  eng.seed(std::chrono::system_clock::now()
               .time_since_epoch()
               .count());  // cambio il seed leggendo il tempo attuale ad ogni
                           // chiamata
  double const r = dist(eng);
  return r;
}
inline std::array<int, 2> sign{1, -1};
inline int r_sign() {
  std::uniform_int_distribution<int> dist(13, 209);
  std::default_random_engine eng;
  eng.seed(std::chrono::system_clock::now().time_since_epoch().count());
  return sign[(dist(eng) % 2)];
}

inline double initial_vel{2000.};
double inline r_velocity() {
  double lower_bound{initial_vel - 500.};
  double upper_boud{
      initial_vel};  // le velocità sono in metri/secondo per fissare le idee
  std::uniform_real_distribution<double> dist(lower_bound, upper_boud);
  std::default_random_engine eng;
  eng.seed(std::chrono::system_clock::now().time_since_epoch().count());
  double const s = dist(eng);
  eng.seed(s);
  double const r = dist(eng);
  return r * r_sign();
}

#endif