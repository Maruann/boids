#include <random>

#include "boids.hpp"

double r_position() {
  double lower_bound{0};
  double upper_boud{
      100};  // le dimesioni sono in metri, quindi stiamo considerando la
             // simulazione come un quadrato 100x100
  std::uniform_real_distribution<double> dist(
      lower_bound,
      upper_boud);  // crea una distribuzione uniforme tra i due limiti
  std::default_random_engine
      eng;  // crea un random engine da dare alla distribuzione
  return dist(eng);
}

double r_velocity() {
  double lower_bound{-15};
  double upper_boud{
      15};  // le velocità sono in metri/secondo per fissare le idee
  std::uniform_real_distribution<double> dist(lower_bound, upper_boud);
  std::default_random_engine eng;
  return dist(eng);
}

class Flock {
  std::vector<Boid> flock;  // vettore di boids
  double sep_;              // separation
  double al_;               // alignment
  double coe_;              // coesion
  double xc_;               // x del centro di massa
  double yc_;               // y del centro di massa

 public:
  Flock(double s, double a, double c) : sep_{s}, al_{a}, coe_{c} {}

  void fill(int n) {  // metodo che filla il vettore stormo(flock) di un numero
                      // arbitrario di boids generati random
    for (int i{0}; i < n; ++i) {
      Boid boid{r_position(), r_position(), r_velocity(), r_velocity()};
      flock.push_back(boid);
    }
  }

  void upt_xc_() {
    xc_ = std::accumulate(
              flock.begin(), flock.end(), 0.,
              [](double sum, Boid boid) { return sum + boid.get_x(); }) /
          flock.size();
  }
  void upt_yc_() {
    yc_ = std::accumulate(
              flock.begin(), flock.end(), 0.,
              [](double sum, Boid boid) { return sum + boid.get_y(); }) /
          flock.size();
  }

  double vx_repulsive(double dx_s, int i) {   //prende in entrata dx_x che è la distanza minima sulle x per avere una repulsione e i che è il numero del boid che si vuole fissare
    auto lambda = [&](double sum, Boid boid) {      //lambda che fa da operatore binario all'accumulate, prende in entrata sum ed il boid sul quale
      return (fabs(boid.get_x() - flock[i].get_x()) > dx_s)   //è arrivato l'iteratore, mentre cattura by reference flock e ne prende l'i-esimo boid
                 ? sum + boid.get_x() - flock[i].get_x()      //ne fa la differenza sulle x e: se in modulo è minore di dx_s ne returna il termine della sommatoria
                 : 0;                                         //se è maggiore returna 0
    };
    return -sep_ * std::accumulate(flock.begin(), flock.end(), 0, lambda);
  } //questo accumulate somma le differenze sulle x tra tutti i boids e quello fissato da i
};

/*  LASCIO QUI IL METODO NEL CASO SI DECIDESSE DI USARE UN ARRAY
std::generate_n(flock.begin(),n , []() {
    Boid boid{casual(), casual(), casual(), casual()};
});*/