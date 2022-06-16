

#include <iostream>

#include "flock.hpp"
#include "velocity.hpp"

double vision{200.};
double separation{30.};
void update(Flock& stormo, int steps_per_evolution, double delta_t) {
  for (int s{0}; s != steps_per_evolution; ++s) {
    auto n_flock{stormo.get_flock()};
    int j{0};
    for (Boid& boid : stormo.get_flock()) {
      double new_x = boid.get_x() + boid.get_vx() * delta_t;
      double new_y = boid.get_y() + boid.get_vy() * delta_t;

      double vx_e = stormo.vx_repulsive(separation, boid) +
                    stormo.vx_alignment(vision, vision / 20., boid) +
                    stormo.vx_coesion(vision, vision / 80., boid);
      double vy_e = stormo.vy_repulsive(separation, boid) +
                    stormo.vy_alignment(vision, vision / 20., boid) +
                    stormo.vy_coesion(vision, vision / 80., boid);
      double new_vx = boid.get_vx() + vx_e / 8.;
      double new_vy = boid.get_vy() + vy_e / 8.;

      if (p_slowdown_active_x(boid)) {
        new_vx = new_vx / 1.01;
      }

      if (p_slowdown_active_y(boid)) {
        new_vy = new_vy / 1.01;
      }

      if (not_in_perimeter_x(boid) && p_velx_active(boid)) {
        new_vx += v_perimeterx(0.015, boid);
      }

      if (not_in_perimeter_y(boid) && p_vely_active(boid)) {
        new_vy += v_perimetery(0.015, boid);
      }

      if (in_explosion_range(boom_positionx, boom_positiony, 100., boid)) {
        new_vx += expl_velocity_x(10., boom_positionx, boid);
        new_vy += expl_velocity_y(10., boom_positiony, boid);
      }

      n_flock[j].set_x(new_x);
      n_flock[j].set_y(new_y);
      n_flock[j].set_vx(new_vx);
      n_flock[j].set_vy(new_vy);
      ++j;
    }
    stormo.set_flock(n_flock);
  }
}

// void update(Flock& stormo, int steps_per_evolution, double delta_t) {
//   for (int s{0}; s != steps_per_evolution; ++s) {
//     std::vector<Boid> n_flock;
//     int j{0};

//     auto lambda = [&](Boid& boid){};

//     std::transform(stormo.get_flock().begin(), stormo.get_flock().end(),n_flock.begin(), lambda )

//     for (Boid& boid : stormo.get_flock()) {
//       double new_x = boid.get_x() + boid.get_vx() * delta_t;
//       double new_y = boid.get_y() + boid.get_vy() * delta_t;

//       double vx_e = stormo.vx_repulsive(separation, boid) +
//                     stormo.vx_alignment(vision, vision / 20., boid) +
//                     stormo.vx_coesion(vision, vision / 80., boid);
//       double vy_e = stormo.vy_repulsive(separation, boid) +
//                     stormo.vy_alignment(vision, vision / 20., boid) +
//                     stormo.vy_coesion(vision, vision / 80., boid);
//       double new_vx = boid.get_vx() + vx_e / 8.;
//       double new_vy = boid.get_vy() + vy_e / 8.;

//       if (p_slowdown_active_x(boid)) {
//         new_vx = new_vx / 1.01;
//       }

//       if (p_slowdown_active_y(boid)) {
//         new_vy = new_vy / 1.01;
//       }

//       if (not_in_perimeter_x(boid) && p_velx_active(boid)) {
//         new_vx += v_perimeterx(0.015, boid);
//       }

//       if (not_in_perimeter_y(boid) && p_vely_active(boid)) {
//         new_vy += v_perimetery(0.015, boid);
//       }

//       if (in_explosion_range(boom_positionx, boom_positiony, 100., boid)) {
//         new_vx += expl_velocity_x(10., boom_positionx, boid);
//         new_vy += expl_velocity_y(10., boom_positiony, boid);
//       }

//       boid.set_x(new_x);
//       boid.set_y(new_y);
//       boid.set_vx(new_vx);
//       boid.set_vy(new_vy);
//       ++j;
//     }
//     stormo.set_flock(n_flock);
//   }
// }

double orientation(double vx, double vy) {
  if (vx >= 0.) {
    return atan(-vy / vx) * (180 / M_PI) - 90;
  }
  return (atan(-vy / vx) + M_PI) * (180 / M_PI) - 90;
}  // funzione che calcola l'angolo (in gradi) di
   // orientamento del singolo boid rispetto all'asse x
