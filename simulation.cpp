#include <execution>
#include <mutex>
#include <thread>

#include "flock.hpp"
#include "velocity.hpp"
double vision{400.};
double separation{30.};

// auto lambda = [](Boid& boid, Flock& stormo, std::vector<Boid>& n_flock,
//                  double delta_t, std::mutex& mu, double dist_mult) {
//   mu.lock();
//   double new_x = boid.get_x() + boid.get_vx() * delta_t;
//   double new_y = boid.get_y() + boid.get_vy() * delta_t;
//   double vx_e =
//       stormo.vx_repulsive(separation, boid) +
//       stormo.vx_alignment(vision * dist_mult, vision * dist_mult / 20., boid)
//       + stormo.vx_coesion(vision * dist_mult, vision * dist_mult / 80.,
//       boid);
//   double vy_e =
//       stormo.vy_repulsive(separation, boid) +
//       stormo.vy_alignment(vision * dist_mult, vision * dist_mult / 20., boid)
//       + stormo.vy_coesion(vision * dist_mult, vision * dist_mult / 80.,
//       boid);
//   double new_vx = boid.get_vx() + vx_e / 8.;
//   double new_vy = boid.get_vy() + vy_e / 8.;

//   if (p_slowdown_active_x(boid)) {
//     new_vx = new_vx / 1.01;
//   }

//   if (p_slowdown_active_y(boid)) {
//     new_vy = new_vy / 1.01;
//   }

//   if (not_in_perimeter_x(boid) && p_velx_active(boid)) {
//     new_vx += v_perimeterx(0.015, boid);
//   }

//   if (not_in_perimeter_y(boid) && p_vely_active(boid)) {
//     new_vy += v_perimetery(0.015, boid);
//   }

//   if (in_explosion_range(boom_positionx, boom_positiony, 100., boid)) {
//     new_vx += expl_velocity_x(10., boom_positionx, boid);
//     new_vy += expl_velocity_y(10., boom_positiony, boid);
//   }

//   boid.set_x(new_x);
//   boid.set_y(new_y);
//   boid.set_vx(new_vx);
//   boid.set_vy(new_vy);
//   n_flock.push_back(boid);
//   mu.unlock();
// };

// void update(Flock& stormo, int steps_per_evolution, double delta_t,
//             double dist_mult) {
//   for (int s{0}; s != steps_per_evolution; ++s) {
//     auto flock{stormo.get_flock()};
//     std::vector<Boid> n_flock;
//     std::vector<std::thread> t_vector;
//     std::mutex mu;

//     for (Boid& boid : flock) {
//       t_vector.push_back(std::thread(lambda, std::ref(boid),
//       std::ref(stormo),
//                                      std::ref(n_flock), delta_t,
//                                      std::ref(mu), dist_mult));
//     }

//     for (auto& th : t_vector) {
//       th.join();
//     }

//     stormo.set_flock(n_flock);
//   }
// }

void update(Flock& stormo, int steps_per_evolution, double delta_t,
            double dist_mult) {
  for (int s{0}; s != steps_per_evolution; ++s) {
    auto flock{stormo.get_flock()};
    std::transform(
        std::execution::par_unseq, flock.begin(), flock.end(), flock.begin(),
        [&](Boid& boid) {
          double new_x = boid.get_x() + boid.get_vx() * delta_t;
          double new_y = boid.get_y() + boid.get_vy() * delta_t;
          double vx_e = stormo.vx_repulsive(separation, boid) +
                        stormo.vx_alignment(vision * dist_mult,
                                            vision * dist_mult / 20., boid) +
                        stormo.vx_coesion(vision * dist_mult,
                                          vision * dist_mult / 80., boid);
          double vy_e = stormo.vy_repulsive(separation, boid) +
                        stormo.vy_alignment(vision * dist_mult,
                                            vision * dist_mult / 20., boid) +
                        stormo.vy_coesion(vision * dist_mult,
                                          vision * dist_mult / 80., boid);
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

          if (in_explosion_range(boom_positionx, boom_positiony, 100., boid))
          {
            new_vx += expl_velocity_x(10., boom_positionx, boid);
            new_vy += expl_velocity_y(10., boom_positiony, boid);
          }

          boid.set_x(new_x);
          boid.set_y(new_y);
          boid.set_vx(new_vx);
          boid.set_vy(new_vy);
          return boid;
        });
    stormo.set_flock(flock);
  }
}


double orientation(double vx, double vy) {
  if (vx >= 0.) {
    return atan(-vy / vx) * (180 / M_PI) - 90;
  }
  return (atan(-vy / vx) + M_PI) * (180 / M_PI) - 90;
}  // funzione che calcola l'angolo (in gradi) di
   // orientamento del singolo boid rispetto all'asse x