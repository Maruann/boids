#ifndef FLOCK
#define FLOCK

#include <chrono>
#include <cmath>
#include <random>

#include "boids.hpp"

// funzioni per la generazione random di numeri con cui
// inizializzare lo stormo
double inline r_position() {
  double lower_bound{0};
  double upper_boud{
      1000};  // le dimesioni sono in metri, quindi stiamo considerando la
              // simulazione come un quadrato 100x100
  std::uniform_real_distribution<double> dist(
      lower_bound,

      upper_boud);  // crea una distribuzione uniforme tra i due limiti
  std::default_random_engine
      eng;  // crea un random engine da dare alla distribuzione
  eng.seed(std::chrono::system_clock::now()
               .time_since_epoch()
               .count());  // cambio il seed leggendo il tempo attuale ad ogni
                           // chiamata
  double const r = dist(eng);
  return r;
}

double inline r_velocity() {
  double lower_bound{-1};
  double upper_boud{
      1};  // le velocità sono in metri/secondo per fissare le idee
  std::uniform_real_distribution<double> dist(lower_bound, upper_boud);
  std::default_random_engine eng;
  eng.seed(std::chrono::system_clock::now().time_since_epoch().count());
  double r = dist(eng);
  return r;
}

class Flock {
  std::vector<Boid> flock;  // vettore di boids
  double sep_;              // separation
  double al_;               // alignment
  double coe_;              // coesion

 public:
  Flock(double s, double a, double c) : sep_{s}, al_{a}, coe_{c} {}

  void fill(int n) {  // metodo che filla il vettore stormo(flock) di un numero
                      // arbitrario di boids generati random
    for (int i{0}; i < n; ++i) {
      Boid boid{r_position(), r_position(), r_velocity(), r_velocity()};
      flock.push_back(boid);
    }
  }
  int size() { return flock.size(); }

  auto get_flock()
      const {      // metodo get per il vettore flock, serve principalmente
    return flock;  // per i test
  }

  void set_flock(std::vector<Boid> new_flock) { flock = new_flock; }

  /* metodo di calcolo per le velocità repulsive: prende in entrata dx_x che è
  la distanza minima sulle x per avere una repulsione e i che è il numero del
  boid che si vuole fissare.
  lambda che fa da operatore binario all'accumulate, prende in entrata sum
  ed il boid sul quale è arrivato l'iteratore, cattura flock e ne prende
  l'i-esimo boid, ne fa la differenza sulle x e: se in modulo è minore di
  dx_s ne returna il termine della sommatoria se è maggiore returna 0*/

  /*double vx_repulsive(double dx_s, Boid& fixed_boid) {
    auto lambda = [&](double sum, Boid boid) {
      return (std::fabs(boid.get_x() - fixed_boid.get_x()) < dx_s)
                 ? sum + fixed_boid.get_x() - boid.get_x()
                 : 0.;
    };
    return -sep_ * std::accumulate(flock.begin(), flock.end(), 0., lambda);
  }*/
  // questo accumulate somma le differenze sulle x tra tutti i boids e
  // quello fissato da "i"

  /*double vy_repulsive(double dx_s, Boid& fixed_boid) {  // stessa cosa sulle y
    auto lambda = [&](double sum, Boid boid) {
      return (std::fabs(boid.get_y() - fixed_boid.get_y()) < dx_s)
                 ? sum + fixed_boid.get_y() - boid.get_y()
                 : 0.;
    };
    return -sep_ * std::accumulate(flock.begin(), flock.end(), 0., lambda);
  }*/
  // la parte commentata qua sotto è la versione della funzione che fa dipendere
  // dall'inverso della distanza, se la vuoi provare scommentala e sostituiscila
  // alla corrispettiva v_repulsiva, ma non ha avuto grandi effetti
  double vx_repulsive(double dx_s, Boid& fixed_boid) {
    auto lambda = [&](double sum, Boid boid) {
      return (std::fabs(boid.get_x() - fixed_boid.get_x()) < dx_s)
                 ? (fixed_boid == boid
                        ? sum
                        : sum + (1. / fixed_boid.get_x() - boid.get_x()))
                 : 0.;
    };
    return -sep_ * std::accumulate(flock.begin(), flock.end(), 0., lambda);
  }

  double vy_repulsive(double dx_s, Boid& fixed_boid) {
    auto lambda = [&](double sum, Boid boid) {
      return (std::fabs(boid.get_y() - fixed_boid.get_y()) < dx_s)
                 ? (fixed_boid == boid
                        ? sum
                        : sum + (1. / fixed_boid.get_y() - boid.get_y()))
                 : 0.;
    };
    return -sep_ * std::accumulate(flock.begin(), flock.end(), 0., lambda);
  }

  // funzione che calcola la velocità di allineamento
  // la lambda poteva essere fatta anche senza dover fare l'overload
  // dell'operatore == per i Boid, ma credo che ci servirà prima o poi
  double vx_alignment(Boid fixed_boid) {
    auto lambda = [&](double sum, Boid boid) {
      return boid == fixed_boid ? sum : sum + boid.get_vx();
    };
    double v_m = std::accumulate(flock.begin(), flock.end(), 0., lambda) /
                 (static_cast<double>(flock.size()) - 1);  // mean velocity
    return al_ * (v_m - fixed_boid.get_vx());
  }

  double vy_alignment(Boid& fixed_boid) {
    auto lambda = [&](double sum, Boid boid) {
      return boid == fixed_boid ? sum : sum + boid.get_vy();
    };
    double v_m = std::accumulate(flock.begin(), flock.end(), 0., lambda) /
                 (static_cast<double>(flock.size()) - 1);  // mean velocity
    return al_ * (v_m - fixed_boid.get_vy());
  }

  // funzione per la velocità di coesione
  double vx_coesion(Boid& fixed_boid) {
    auto lambda = [&](double sum, Boid boid) {
      return boid == fixed_boid ? sum : sum + boid.get_x();
    };
    double x_m = std::accumulate(flock.begin(), flock.end(), 0., lambda) /
                 (static_cast<double>(flock.size()) - 1);
    return coe_ * (x_m - fixed_boid.get_x());
  }

  double vy_coesion(Boid& fixed_boid) {
    auto lambda = [&](double sum, Boid boid) {
      return boid == fixed_boid ? sum : sum + boid.get_y();
    };
    double y_m = std::accumulate(flock.begin(), flock.end(), 0., lambda) /
                 (static_cast<double>(flock.size()) - 1);
    return coe_ * (y_m - fixed_boid.get_y());
  }
};


#endif