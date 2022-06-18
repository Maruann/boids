#ifndef FLOCK
#define FLOCK

#include <iostream>

#include "boids.hpp"
#include "r_numbers.hpp"
#include "iterator"

class Flock {
  std::vector<Boid> flock;  // vettore di boids
  double sep_;              // separation
  double al_;               // alignment
  double coe_;              // coesion

 public:
  Flock(double s, double a, double c) : sep_{s}, al_{a}, coe_{c} {}

  void fill(int n) {  // metodo che filla il vettore stormo(flock) di un numero
                      // arbitrario di boids generati random
                      // for (int i{0}; i < n; ++i) {
    //   Boid boid{r_position_x(), r_position_y(), r_velocity(), r_velocity()};
    //   flock.push_back(boid);
    // }
    std::generate_n(std::back_inserter(flock), n, [](){Boid boid{r_position_x(), r_position_y(), r_velocity(), r_velocity()};
      return boid;});
  }
  int size() { return flock.size(); }

  auto get_flock() const { return flock; }

  void set_flock(std::vector<Boid>& new_flock) { flock = new_flock; }

  double get_ali() { return al_; }

  double get_sep() { return sep_; }

  double get_coe() { return coe_; }

  void set_ali(double new_al) { al_ = new_al; }

  void set_sep(double new_sep) { sep_ = new_sep; }

  void set_coe(double new_coe) { coe_ = new_coe; }

  double vx_repulsive(double dx_s, Boid& fixed_boid);
  double vy_repulsive(double dx_s, Boid& fixed_boid);

  double vx_alignment(double dx_a, double mx_a, Boid& fixed_boid);
  double vy_alignment(double dx_a, double mx_a, Boid& fixed_boid);

  double vx_coesion(double dx_c, double mx_c, Boid& fixed_boid);
  double vy_coesion(double dx_c, double mx_c, Boid& fixed_boid);
};

#endif