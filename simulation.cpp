

#include <iostream>

#include "flock.hpp"

double center_x{sf::VideoMode::getDesktopMode().width / 2.};
double center_y{sf::VideoMode::getDesktopMode().height / 2.};

bool not_in_perimeter_x(Boid& boid) {
  return boid.get_x() < bound_xmin || boid.get_x() > bound_xmax;
}

bool not_in_perimeter_y(Boid& boid) {
  return boid.get_y() < bound_ymin || boid.get_y() > bound_ymax;
}

double v_perimeterx(double m, Boid& boid) {
  return m * (center_x - boid.get_x());
}

double v_perimetery(double m, Boid& boid) {
  return m * (center_y - boid.get_y());
}


double vision{100.};
double separation{20.};

Boid solve(Flock& stormo, double delta_t, Boid& boid) {
  double new_x = boid.get_x() + boid.get_vx() * delta_t;
  double new_y = boid.get_y() + boid.get_vy() * delta_t;



  double vx_e = stormo.vx_repulsive(separation, boid) +
                stormo.vx_alignment(vision, vision / 20., boid) +
                stormo.vx_coesion(vision, vision / 80., boid);
  double vy_e = stormo.vy_repulsive(separation, boid) +
                stormo.vy_alignment(vision, vision / 20., boid) +
                stormo.vy_coesion(vision, vision / 80., boid);
  double new_vx = boid.get_vx() + vx_e;
  double new_vy = boid.get_vy() + vy_e;

  if (not_in_perimeter_x(boid)) {
    new_vx += v_perimeterx(0.1, boid);
  }

  if (not_in_perimeter_y(boid)) {
    new_vy += v_perimetery(0.1, boid);
  }
  Boid new_boid{new_x, new_y, new_vx, new_vy};
  return new_boid;
  /*
    double sign_x{std::fabs(boid.get_vx())/boid.get_vx()};
    double sign_y{std::fabs(boid.get_vy())/boid.get_vy()};
      if (std::fabs(new_vx) >= v_max_x) {
        std::cout << "velocità massima raggiunta    " << boid.get_vx() << '\n';
      if (std::fabs(new_vy) > v_max_y) {
        Boid new_boid{new_x, new_y, v_max_x*sign_x + v_perimeterx(1.,boid),
    v_max_y*sign_y+v_perimetery(1.,boid)}; return new_boid; } else { Boid
    new_boid{new_x, new_y, v_max_x*sign_x+ v_perimeterx(1.,boid), new_vy};
        return new_boid;
      }
    } else {
      if (std::fabs(new_vy) >= v_max_y) {
        Boid new_boid{new_x, new_y, new_vx,
    v_max_y*sign_y+v_perimetery(1.,boid)}; std::cout << "velocità massima
    raggiunta    " << boid.get_vy()<< '\n'; return new_boid; } else { Boid
    new_boid{new_x, new_y, new_vx, new_vy}; return new_boid;
      }
    }*/
}
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
