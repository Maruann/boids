#include "flock.hpp"

double Flock::mean_distance() {
  double total_sum{0.};
  for (Boid& fixed_boid : flock) {
    double single_sum{0.};
    for (Boid& boid : flock) {
      single_sum += std::sqrt((fixed_boid.get_x() - boid.get_x()) *
                                  (fixed_boid.get_x() - boid.get_x()) +
                              (fixed_boid.get_y() - boid.get_y()) *
                                  (fixed_boid.get_y() - boid.get_y()));
    }
    total_sum += single_sum / (flock.size() - 1);
  }
  return total_sum / flock.size();
}

double Flock::mean_velocity() {
  double sum{0.};
  for (Boid& boid : flock) {
    sum += std::sqrt(boid.get_vx() * boid.get_vx() +
                     boid.get_vy() * boid.get_vy());
  }
  return sum / flock.size();
}

double Flock::stnd_deviation_distance(double mean_distance) {
  double total_sum{0.};
  for (Boid& fixed_boid : flock) {
    double single_sum{0.};
    for (Boid& boid : flock) {
      single_sum += std::sqrt((fixed_boid.get_x() - boid.get_x()) *
                                  (fixed_boid.get_x() - boid.get_x()) +
                              (fixed_boid.get_y() - boid.get_y()) *
                                  (fixed_boid.get_y() - boid.get_y()));
    }
    total_sum += (single_sum / (flock.size() - 1) - mean_distance) *
                 (single_sum / (flock.size() - 1) - mean_distance);
  }
  return std::sqrt(total_sum / (flock.size() - 1));
}

double Flock::stnd_deviation_velocity(double mean_velocity) {
  double sum{0.};
  for (Boid& boid : flock) {
    sum += (std::sqrt(boid.get_vx() * boid.get_vx() +
                      boid.get_vy() * boid.get_vy()) -
            mean_velocity) *
           (std::sqrt(boid.get_vx() * boid.get_vx() +
                      boid.get_vy() * boid.get_vy()) -
            mean_velocity);
  }
  return std::sqrt(sum/(flock.size()-1));
}
