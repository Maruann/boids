#ifndef FLOCK
#define FLOCK

#include <iostream>

#include "boids.hpp"
#include "r_numbers.hpp"

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
      Boid boid{r_position_x(), r_position_y(), r_velocity(), r_velocity()};
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
  double max_v_rep{100.};
  double vx_repulsive(double dx_s, Boid& fixed_boid) {
    auto lambda = [&](double sum, Boid boid) {
      return (std::fabs(boid.get_x() - fixed_boid.get_x()) < dx_s &&
              std::fabs(boid.get_y() - fixed_boid.get_y()) < dx_s)
                 ? (fixed_boid == boid
                        ? sum
                        : sum + ((boid.get_x() - fixed_boid.get_x())))
                 : sum;
    };
    double vx_rep{-sep_ *
                  std::accumulate(flock.begin(), flock.end(), 0., lambda)};
    double sign{std::fabs(vx_rep) / vx_rep};
    return std::fabs(vx_rep) < max_v_rep ? vx_rep : max_v_rep * sign;
  }

  double vy_repulsive(double dx_s, Boid& fixed_boid) {
    auto lambda = [&](double sum, Boid boid) {
      return (std::fabs(boid.get_x() - fixed_boid.get_x()) < dx_s &&
              std::fabs(boid.get_y() - fixed_boid.get_y()) < dx_s)
                 ? (fixed_boid == boid
                        ? sum
                        : sum + ((boid.get_y() - fixed_boid.get_y())))
                 : sum;
    };
    double vy_rep{-sep_ *
                  std::accumulate(flock.begin(), flock.end(), 0., lambda)};
    double sign{std::fabs(vy_rep) / vy_rep};
    return std::fabs(vy_rep) < max_v_rep ? vy_rep : max_v_rep * sign;
  }

  // funzione che calcola la velocità di allineamento
  // la lambda poteva essere fatta anche senza dover fare l'overload
  // dell'operatore == per i Boid, ma credo che ci servirà prima o poi
  double max_v_alig{10.};
  double vx_alignment(double dx_a, double mx_a, Boid fixed_boid) {
    auto lambda = [&](double sum, Boid boid) {
      return ((std::fabs(boid.get_x() - fixed_boid.get_x()) < dx_a ||
               std::fabs(boid.get_y() - fixed_boid.get_y()) < dx_a) &&
              (std::fabs(boid.get_x() - fixed_boid.get_x()) > mx_a ||
               std::fabs(boid.get_y() - fixed_boid.get_y()) > mx_a))
                 ? (boid == fixed_boid ? sum : sum + boid.get_vx())
                 : sum;
    };
    double v_m = std::accumulate(flock.begin(), flock.end(), 0., lambda) /
                 (static_cast<double>(flock.size()) - 1);  // mean velocity
    double vx_align{al_ * (v_m - fixed_boid.get_vx())};
    double sign{std::fabs(vx_align) / vx_align};
    return std::fabs(vx_align) < max_v_alig ? vx_align : max_v_alig * sign;
  }

  double vy_alignment(double dx_a, double mx_a, Boid fixed_boid) {
    auto lambda = [&](double sum, Boid boid) {
      return ((std::fabs(boid.get_x() - fixed_boid.get_x()) < dx_a ||
               std::fabs(boid.get_y() - fixed_boid.get_y()) < dx_a) &&
              (std::fabs(boid.get_x() - fixed_boid.get_x()) > mx_a ||
               std::fabs(boid.get_y() - fixed_boid.get_y()) > mx_a))
                 ? (boid == fixed_boid ? sum : sum + boid.get_vy())
                 : sum;
    };
    double v_m = std::accumulate(flock.begin(), flock.end(), 0., lambda) /
                 (static_cast<double>(flock.size()) - 1);  // mean velocity
    double vy_align{al_ * (v_m - fixed_boid.get_vy())};
    double sign{std::fabs(vy_align) / vy_align};
    return std::fabs(vy_align) < max_v_alig ? vy_align : max_v_alig * sign;
  }

  // funzione per la velocità di coesione
  double max_v_coe{20.};
  double vx_coesion(double dx_c, double mx_c, Boid& fixed_boid) {
    auto lambda = [&](double sum, Boid boid) {
      return ((std::fabs(boid.get_x() - fixed_boid.get_x()) < dx_c ||
               std::fabs(boid.get_y() - fixed_boid.get_y()) < dx_c) &&
              (std::fabs(boid.get_x() - fixed_boid.get_x()) > mx_c ||
               std::fabs(boid.get_y() - fixed_boid.get_y()) > mx_c))
                 ? (boid == fixed_boid ? sum : sum + boid.get_x())
                 : sum;
    };
    double x_m = std::accumulate(flock.begin(), flock.end(), 0., lambda) /
                 (static_cast<double>(flock.size()) - 1);
    double vx_coe{coe_ * (x_m - fixed_boid.get_x())};
    double sign{std::fabs(vx_coe) / vx_coe};
    return std::fabs(vx_coe) < max_v_coe ? vx_coe : max_v_coe * sign;
  }

  double vy_coesion(double dx_c, double mx_c, Boid& fixed_boid) {
    auto lambda = [&](double sum, Boid boid) {
      return ((std::fabs(boid.get_x() - fixed_boid.get_x()) < dx_c ||
               std::fabs(boid.get_y() - fixed_boid.get_y()) < dx_c) &&
              (std::fabs(boid.get_x() - fixed_boid.get_x()) > mx_c ||
               std::fabs(boid.get_y() - fixed_boid.get_y()) > mx_c))
                 ? (boid == fixed_boid ? sum : sum + boid.get_y())
                 : sum;
    };
    double y_m = std::accumulate(flock.begin(), flock.end(), 0., lambda) /
                 (static_cast<double>(flock.size()) - 1);
    double vy_coe{coe_ * (y_m - fixed_boid.get_x())};
    double sign{std::fabs(vy_coe) / vy_coe};
    return std::fabs(vy_coe) < max_v_coe ? vy_coe : max_v_coe * sign;
  }
};

#endif