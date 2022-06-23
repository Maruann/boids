#ifndef R_NUMBERS
#define R_NUMBERS

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <cassert>
#include <chrono>
#include <cmath>
#include <random>

// In questo Header File sono definite le free-function responsabili della
// generazione pseudo-casuale dei numeri necessari a inizializzare i boids.

// spigoli del rettangolo in cui sono contenuti i boid senza essere accelerati
// verso il centro
inline double const bound_xmin{sf::VideoMode::getDesktopMode().width * (15. / 100.)};
inline double const bound_xmax{sf::VideoMode::getDesktopMode().width * (68. / 100.)};
inline double const bound_ymin{
    (sf::VideoMode::getDesktopMode().height * (25. / 100.))};
inline double const bound_ymax{
    (sf::VideoMode::getDesktopMode().height * (50. / 100.))};

// random engine e distribuzione necessari per generare numeri casuali sulle X
// nel range del rettangolo
inline std::uniform_real_distribution<double> dist_x(bound_xmin, bound_xmax);
inline std::default_random_engine eng;

// funzione che returna una coordinata X random
double inline r_position_x()
{
  eng.seed(std::chrono::system_clock::now().time_since_epoch().count());
  return dist_x(eng);
}
// distribuzione per generare numeri casuali sulle Y nel range del rettangolo
inline std::uniform_real_distribution<double> dist_y(bound_ymin, bound_ymax);
// funzione che returna una coordinata Y random
double inline r_position_y()
{
  eng.seed(std::chrono::system_clock::now().time_since_epoch().count());
  return dist_y(eng);
}

// qui mi sono generato a mano un generatore casuale che returna 1 o -1 per
// rendere casuale il segno delle velocità, 13 e 209 sono solo numeri arbitrari
inline std::array<int, 2> const sign{1, -1};
inline std::uniform_int_distribution<int> int_dist(13, 209);
// la funzione returna 1 o -1 richiamando l'i-esimo elemento dell'array, dove i
// è determitata dal resto della divisione per 2 dell'intero generato dalla
// distribuzione
inline int r_sign()
{
  eng.seed(std::chrono::system_clock::now().time_since_epoch().count());
  return sign[(int_dist(eng) % 2)];
}
// fisso i limiti della velocità iniziale e li uso come limiti di una
// distribuzione
inline double const initial_vel{2000.};
inline std::uniform_real_distribution<double> vel_dist(initial_vel - 500,
                                                       initial_vel);
// funzione che returna una velocità iniziale con intensità e segno random
double inline r_velocity()
{
  eng.seed(std::chrono::system_clock::now().time_since_epoch().count());
  return vel_dist(eng) * r_sign();
}

#endif