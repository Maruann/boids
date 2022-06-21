#ifndef FLOCK
#define FLOCK

#include "boids.hpp"
#include "iterator"
#include "r_numbers.hpp"

// Classe flock, rappresenta l'insieme di boids che interagiscono tra di loro.
//  La maggior parte dei metodi di questa classe sono definiti in altre TU, in
//  particolare il calcolo delle velocità è assegnato a velocity.cpp, mentre il
//  calcolo statistico a statistics.cpp
class Flock
{
  // i dati membro sono un vettore che conterrà gli oggeti Boid, ovvero i boids
  // dello stormo, e i valori dei parametri di repulsione, allineamento e
  // coesione
  std::vector<Boid> flock;
  double sep_;
  double al_;
  double cohe_;
  // costruttore
 public:
  Flock(double s, double a, double c)
      : sep_{s}
      , al_{a}
      , cohe_{c}
  {}
  // metodo che riempe lo stormo in base al numero intero di boids che si decide
  // di generare
  void fill(int n)
  {
    std::generate_n(std::back_inserter(flock), n, []() {
      Boid boid{r_position_x(), r_position_y(), r_velocity(), r_velocity()};
      return boid;
    });
  }
  // metodo che returna il numero di boids nello stormo
  int size()
  {
    return flock.size();
  }
  // metodi get e set necessari per interfacciarsi con i dati membro
  auto get_flock() const
  {
    return flock;
  }

  void set_flock(std::vector<Boid>& new_flock)
  {
    flock = new_flock;
  }

  double get_ali()
  {
    return al_;
  }

  double get_sep()
  {
    return sep_;
  }

  double get_coe()
  {
    return cohe_;
  }

  void set_ali(double new_al)
  {
    al_ = new_al;
  }

  void set_sep(double new_sep)
  {
    sep_ = new_sep;
  }

  void set_coe(double new_coe)
  {
    cohe_ = new_coe;
  }
  // Dichiarazione dei metodi che calcolano le velocità di interazione tra i
  // boids dello stormo. La decisione di farle funzioni membro è stata presa per
  // via dei vantaggi a cui porta, specialmente l'accesso ai dati membro.
  // I corpi dei metodi sono definiti in velocity.cpp
  double vx_repulsive(double dx_s, Boid& fixed_boid);
  double vy_repulsive(double dx_s, Boid& fixed_boid);

  double vx_alignment(double dx_a, double mx_a, Boid& fixed_boid);
  double vy_alignment(double dx_a, double mx_a, Boid& fixed_boid);

  double vx_cohesion(double dx_c, double mx_c, Boid& fixed_boid);
  double vy_cohesion(double dx_c, double mx_c, Boid& fixed_boid);

  // dichiarazione dei metodi per il calcolo statistico, il corpo è definito in
  // statistics.cpp
  double mean_distance();
  double mean_velocity();
  double stnd_deviation_distance(double mean_distance);
  double stnd_deviation_velocity(double mean_velocity);
};

#endif