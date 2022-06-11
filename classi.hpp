#include <algorithm>
#include <array>

class Boid {
  double x_;  // posizioni nel piano
  double y_;
  double vx_;  // velocità nel piano
  double vy_;
  double ax_;  // accelerazioni nel piano (forse serviranno)
  double ay_;

 public:
  Boid(double x = 0, double y = 0) : x_{x}, y_{y} {}
  void update(double n_vx, double n_vy,
              double dt) {  // update prende le due new_v e aggiorna la
                            // posizione in un dt
    vx_ = n_vx;
    vy_ = n_vy;  // la funzione update è uno schizzo

    x_ += vx_ * dt;
    y_ += vy_ * dt;     //aggiorna la posizione
  }
};

template <int n>

class Flock {
  std::array<Boid, n> flock;        //array di boids
  double separation;
  double alignment;         //parametri
  double coesion;
  double casual(){};        //funzione da scrivere per i numeri casuali

 public:
  Flock(double s, double a, double c, int n)
      : separation{s}, alignment{a}, coesion{c} {
    std::generate(flock.begin(),flock.end(), [] (){Boid boid{casual,casual}})       //algoritmo che genera i boids con cui fillare l'array
  }                                                                                 //da completare la lambda
};