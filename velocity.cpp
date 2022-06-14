#include "flock.hpp"

bool in_rep_range(double range, Boid& boid, Boid& fixed_boid) {
  return std::fabs(boid.get_x() - fixed_boid.get_x()) < range &&
         std::fabs(boid.get_y() - fixed_boid.get_y()) < range;
}

bool in_range(double max_range, double min_range, Boid& boid,
              Boid& fixed_boid) {
  return (std::fabs(boid.get_x() - fixed_boid.get_x()) < max_range &&
          std::fabs(boid.get_y() - fixed_boid.get_y()) < max_range) &&
         (std::fabs(boid.get_x() - fixed_boid.get_x()) > min_range ||
          std::fabs(boid.get_y() - fixed_boid.get_y()) > min_range);
}
//////////////////////////////
double max_v_rep{20.};
double Flock::vx_repulsive(double range, Boid& fixed_boid) {
  auto lambda = [&](double sum, Boid& boid) {
    return in_rep_range(range, boid, fixed_boid)
               ? (fixed_boid == boid
                      ? sum
                      : sum + (1./(boid.get_x()/100. - fixed_boid.get_x()/100.)))
               : sum;
  };
  double vx_rep{-sep_ *
                std::accumulate(flock.begin(), flock.end(), 0., lambda)};
  double sign{std::fabs(vx_rep) / vx_rep};
  return std::fabs(vx_rep) < max_v_rep ? vx_rep : max_v_rep * sign;
}

double Flock::vy_repulsive(double range, Boid& fixed_boid) {
  auto lambda = [&](double sum, Boid& boid) {
    return in_rep_range(range, boid, fixed_boid)
               ? (fixed_boid == boid
                      ? sum
                      : sum + (1./(boid.get_y()/500. - fixed_boid.get_y()/500.)))
               : sum;
  };
  double vy_rep{-sep_ *
                std::accumulate(flock.begin(), flock.end(), 0., lambda)};
  double sign{std::fabs(vy_rep) / vy_rep};
  return std::fabs(vy_rep) < max_v_rep ? vy_rep : max_v_rep * sign;
}
//////////////////////////////
double max_v_alig{3.};
double Flock::vx_alignment(double max_range, double min_range,
                           Boid& fixed_boid) {
  auto n_lambda = [&](Boid& boid) {
    return in_range(max_range, min_range, boid, fixed_boid);
  };

  int n_boids = std::count_if(flock.begin(), flock.end(), n_lambda);
  if (n_boids == 0) {
    return 0.;
  }
  auto lambda = [&](double sum, Boid& boid) {
    return in_range(max_range, min_range, boid, fixed_boid) &&
                   !(boid == fixed_boid)
               ? sum + boid.get_vx()
               : sum;
  };
  double v_m = std::accumulate(flock.begin(), flock.end(), 0., lambda) /
               static_cast<double>(n_boids);
  double vx_align{al_ * (v_m - fixed_boid.get_vx())};
  double sign{std::fabs(vx_align) / vx_align};
  return std::fabs(vx_align) < max_v_alig ? vx_align : max_v_alig * sign;
}

double Flock::vy_alignment(double max_range, double min_range,
                           Boid& fixed_boid) {
  auto n_lambda = [&](Boid& boid) {
    return in_range(max_range, min_range, boid, fixed_boid);
  };

  int n_boids = std::count_if(flock.begin(), flock.end(), n_lambda);
  if (n_boids == 0) {
    return 0.;
  }
  auto lambda = [&](double sum, Boid& boid) {
    return in_range(max_range, min_range, boid, fixed_boid) &&
                   !(boid == fixed_boid)
               ? sum + boid.get_vy()
               : sum;
  };
  double v_m = std::accumulate(flock.begin(), flock.end(), 0., lambda) /
               static_cast<double>(n_boids);
  double vy_align{al_ * (v_m - fixed_boid.get_vy())};
  double sign{std::fabs(vy_align) / vy_align};
  return std::fabs(vy_align) < max_v_alig ? vy_align : max_v_alig * sign;
}

/////////////////////////////
double max_v_coe{10.};
double Flock::vx_coesion(double max_range, double min_range, Boid& fixed_boid) {
  auto n_lambda = [&](Boid& boid) {
    return in_range(max_range, min_range, boid, fixed_boid);
  };
  int n_boids = std::count_if(flock.begin(), flock.end(), n_lambda);
  if (n_boids == 0) {
    return 0.;
  }
  auto lambda = [&](double sum, Boid& boid) {
    return in_range(max_range, min_range, boid, fixed_boid) &&
                   !(boid == fixed_boid)
               ? sum + boid.get_x()
               : sum;
  };
  double x_m = std::accumulate(flock.begin(), flock.end(), 0., lambda) /
               static_cast<double>(n_boids);
  double vx_coe{coe_ * (x_m - fixed_boid.get_x())};
  double sign{std::fabs(vx_coe) / vx_coe};
  return std::fabs(vx_coe) < max_v_coe ? vx_coe : max_v_coe * sign;
}

double Flock::vy_coesion(double max_range, double min_range, Boid& fixed_boid) {
  auto n_lambda = [&](Boid& boid) {
    return in_range(max_range, min_range, boid, fixed_boid);
  };
  int n_boids = std::count_if(flock.begin(), flock.end(), n_lambda);
  if (n_boids == 0) {
    return 0.;
  }
  auto lambda = [&](double sum, Boid& boid) {
    return in_range(max_range, min_range, boid, fixed_boid) &&
                   !(boid == fixed_boid)
               ? sum + boid.get_y()
               : sum;
  };
  double y_m = std::accumulate(flock.begin(), flock.end(), 0., lambda) /
               static_cast<double>(n_boids);
  double vy_coe{coe_ * (y_m - fixed_boid.get_y())};
  double sign{std::fabs(vy_coe) / vy_coe};
  return std::fabs(vy_coe) < max_v_coe ? vy_coe : max_v_coe * sign;
}