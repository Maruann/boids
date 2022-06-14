#ifndef R_NUMBERS
#define R_NUMBERS

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <chrono>
#include <cmath>
#include <random>

// funzioni per la generazione random di numeri con cui
// inizializzare lo stormo
inline double bound_xmin{sf::VideoMode::getDesktopMode().width / 4.};
inline double bound_xmax{3. * sf::VideoMode::getDesktopMode().width / 4.};
inline double bound_ymin{sf::VideoMode::getDesktopMode().height / 4.};
inline double bound_ymax{3. * sf::VideoMode::getDesktopMode().height / 4.};

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
inline double initial_vel{1000.};
double inline r_velocity() {
  double lower_bound{-initial_vel};
  double upper_boud{
      initial_vel};  // le velocità sono in metri/secondo per fissare le idee
  std::uniform_real_distribution<double> dist(lower_bound, upper_boud);
  std::default_random_engine eng;
  eng.seed(std::chrono::system_clock::now().time_since_epoch().count());
  double const s = dist(eng);
  eng.seed(s);
  double const r = dist(eng);
  return r;
}

#endif