#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

#include "flock.hpp"
double v_max_x{80.};
double v_max_y{80.};
Boid solve(Flock& stormo, double delta_t, Boid& boid) {
  // ho tolto la creazione di un nuovo boid ed invece glielo passo
  /* std::cout << boid.get_x() << '\n';
   boid.set_x(boid.get_x() + boid.get_vx() * delta_t);
   boid.set_y(boid.get_y() + boid.get_vy() * delta_t);
   // ATTENZIONE HO FISSATO IL PARAMETRO d (50.)
   double vx_e = stormo.vx_repulsive(50., boid) + stormo.vx_alignment(boid) +
                 stormo.vx_coesion(boid);
   boid.set_vx(boid.get_vx() + vx_e);

   double vy_e = stormo.vy_repulsive(50., boid) + stormo.vy_alignment(boid) +
                 stormo.vy_coesion(boid);  // ANCHE QUI
   boid.set_vy(boid.get_vy() + vy_e);
   std::cout << boid.get_x() << '\n';

   return boid;
 */
  double new_x = boid.get_x() + boid.get_vx() * delta_t;
  double new_y = boid.get_y() + boid.get_vy() * delta_t;

  double vx_e = stormo.vx_repulsive(10., boid) +
                stormo.vx_alignment(30., boid) + stormo.vx_coesion(4., boid);
  double vy_e = stormo.vy_repulsive(10., boid) +
                stormo.vy_alignment(30., boid) + stormo.vy_coesion(4., boid);
  double new_vx = boid.get_vx() + vx_e;
  double new_vy = boid.get_vy() + vy_e;
  if (std::fabs(new_vx) >= v_max_x) {
    if (std::fabs(new_vy) > v_max_y) {
      Boid new_boid{new_x, new_y, boid.get_vx(), boid.get_vy()};
      return new_boid;
    } else {
      Boid new_boid{new_x, new_y, boid.get_vx(), new_vy};
      return new_boid;
    }
  } else {
    if (std::fabs(new_vy) >= v_max_y) {
      Boid new_boid{new_x, new_y, new_vx, boid.get_vx()};
      return new_boid;
    } else {
      Boid new_boid{new_x, new_y, new_vx, new_vy};
      return new_boid;
    }
  }

}  // in pratica aggiorna il singolo boide, prima aggiornando la posizione
   // tramite la velocità "vecchia", e poi aggiornando le velocità

void evolve(Flock& stormo, double delta_t) {
  std::vector<Boid> new_flock;
  for (Boid boid : stormo.get_flock()) {
    new_flock.push_back(solve(stormo, delta_t, boid));
  }
  stormo.set_flock(new_flock);
};  // creo un nuovo vettore di boids che vado a riempire con i "vecchi" boids
    // aggiornati e poi una volta riempito lo vado a sostituire tramite il
    // metodo appositamente creato set flock

// provvisoriamente ho usato il nome italiano stormo perché se avessi chiamato
// flock l'oggetto di tipo Flock, ci sarebbe stata confusione sia con il nome
// della classe che con il membro flock

void update(Flock& flock, int steps_per_evolution, sf::Time delta_t) {
  double const dt{delta_t.asSeconds()};

  for (int s{0}; s != steps_per_evolution; ++s) {
    evolve(flock, dt);
  }
}

double orientation(double vx, double vy) {
  double pi = acos(-1);
  double result = atan(-vy / vx);
  if (vx >= 0.) {
  } else if (vx < 0.) {
    result += pi;
  }

  result = result * (180 / pi);
  return result;
}  // funzione che calcola l'angolo (in gradi) di
   // orientamento del singolo boid rispetto all'asse x
