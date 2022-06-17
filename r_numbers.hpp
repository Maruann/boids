#ifndef R_NUMBERS
#define R_NUMBERS

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <chrono>
#include <cmath>
#include <random>

// funzioni per la generazione random di numeri con cui
// inizializzare lo stormo
inline double bound_xmin{sf::VideoMode::getDesktopMode().width* (15./100.)};
inline double bound_xmax{sf::VideoMode::getDesktopMode().width*(68./100.)};
inline double bound_ymin{(sf::VideoMode::getDesktopMode().height*(25./100.))};
inline double bound_ymax{(sf::VideoMode::getDesktopMode().height*(50./100.))
                         };
inline std::uniform_real_distribution<double> dist_x(bound_xmin, bound_xmax);
inline std::default_random_engine eng;
double inline r_position_x() {
  eng.seed(std::chrono::system_clock::now()
               .time_since_epoch()
               .count());  // cambio il seed leggendo il tempo attuale ad ogni
                           // chiamata
  return dist_x(eng);
}
inline std::uniform_real_distribution<double> dist_y(bound_ymin, bound_ymax);
double inline r_position_y() {
  eng.seed(std::chrono::system_clock::now()
               .time_since_epoch()
               .count());  // cambio il seed leggendo il tempo attuale ad ogni
                           // chiamata
  return dist_y(eng);
}
inline std::array<int, 2> sign{1, -1};
inline std::uniform_int_distribution<int> int_dist(13, 209);
inline int r_sign() {
  eng.seed(std::chrono::system_clock::now().time_since_epoch().count());
  return sign[(int_dist(eng) % 2)];
}

inline double initial_vel{2000.};
inline std::uniform_real_distribution<double> vel_dist(initial_vel-500, initial_vel);

double inline r_velocity() {
  eng.seed(std::chrono::system_clock::now().time_since_epoch().count());
  return vel_dist(eng) * r_sign();
}

#endif