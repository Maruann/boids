#include <execution>

#include "flock.hpp"
inline bool in_rep_range(double range, Boid& boid, Boid& fixed_boid) {
  return std::fabs(boid.get_x() - fixed_boid.get_x()) < range &&
         std::fabs(boid.get_y() - fixed_boid.get_y()) < range &&
         !(boid == fixed_boid);
}

inline bool in_range(double max_range, double min_range, Boid& boid,
                     Boid& fixed_boid) {
  return (std::fabs(boid.get_x() - fixed_boid.get_x()) < max_range &&
          std::fabs(boid.get_y() - fixed_boid.get_y()) < max_range) &&
         !(boid == fixed_boid) &&
         (std::fabs(boid.get_x() - fixed_boid.get_x()) > min_range ||
          std::fabs(boid.get_y() - fixed_boid.get_y()) > min_range);
}
//////////////////////////////
double max_v_rep{6.};
double Flock::vx_repulsive(double range, Boid& fixed_boid) {
  // double vx_rep{
  //     -sep_ * std::accumulate(
  //                 flock.begin(), flock.end(), 0., [&](double sum, Boid& boid)
  //                 {
  //                   return in_rep_range(range, boid, fixed_boid)
  //                              ? sum + (1. / (boid.get_x() / 100. -
  //                                             fixed_boid.get_x() / 100.))
  //                              : sum;
  //                 })};
  double sum{0.};
  for (Boid& boid : flock) {
    if (in_rep_range(range, boid, fixed_boid)){
      sum += 1. / (boid.get_x() / 100. - fixed_boid.get_x() / 100.);}
  }
  double vx_rep = -sum * sep_;
  if (std::fabs(vx_rep) < max_v_rep) {
    return vx_rep;
  }
  double sign{std::fabs(vx_rep) / vx_rep};
  return sep_ * max_v_rep * sign;
}

double Flock::vy_repulsive(double range, Boid& fixed_boid) {
  // double vy_rep{
  //     -sep_ * std::accumulate(
  //                 flock.begin(), flock.end(), 0., [&](double sum, Boid& boid)
  //                 {
  //                   return in_rep_range(range, boid, fixed_boid)

  //                              ? sum + (1. / (boid.get_y() / 500. -
  //                                             fixed_boid.get_y() / 500.))
  //                              : sum;
  //                 })};
  

  double sum{0.};
  for (Boid& boid : flock) {
    if (in_rep_range(range, boid, fixed_boid)){
      sum += 1. / (boid.get_y() / 100. - fixed_boid.get_y() / 100.);}
  }
  double vy_rep = -sum * sep_;
  if (std::fabs(vy_rep) < max_v_rep) {
    return vy_rep;
  }
  double sign{std::fabs(vy_rep) / vy_rep};
  return sep_ * max_v_rep * sign;
}
//////////////////////////////
double max_v_alig{1.2};
double Flock::vx_alignment(double max_range, double min_range,
                           Boid& fixed_boid) {
  double n_boids = std::count_if(flock.begin(), flock.end(), [&](Boid& boid) {
    return in_range(max_range, min_range, boid, fixed_boid);
  });
  if (n_boids == 0) {
    return 0.;
  }
  // double v_m =
  //     std::accumulate(flock.begin(), flock.end(), 0.,
  //                     [&](double sum, Boid& boid) {
  //                       return in_range(max_range, min_range, boid,
  //                       fixed_boid)
  //                                  ? sum + boid.get_vx()
  //                                  : sum;
  //                     }) /
  //     n_boids;
  // double vx_align{al_ * (v_m - fixed_boid.get_vx())};
  double sum{0.};
  for (Boid& boid : flock) {
    if (in_range(max_range, min_range, boid, fixed_boid)) {sum += boid.get_vx();}
  }
  double vx_align = al_ * ((sum / n_boids) - fixed_boid.get_vx());

  if (std::fabs(vx_align) < max_v_alig) {
    return vx_align;
  }
  double sign{std::fabs(vx_align) / vx_align};
  return al_ * max_v_alig * sign;
}

double Flock::vy_alignment(double max_range, double min_range,
                           Boid& fixed_boid) {
  double n_boids = std::count_if(flock.begin(), flock.end(), [&](Boid& boid) {
    return in_range(max_range, min_range, boid, fixed_boid);
  });
  if (n_boids == 0) {
    return 0.;
  }
  // double v_m =
  //     std::accumulate(flock.begin(), flock.end(), 0.,
  //                     [&](double sum, Boid& boid) {
  //                       return in_range(max_range, min_range, boid,
  //                       fixed_boid)
  //                                  ? sum + boid.get_vy()
  //                                  : sum;
  //                     }) /
  //     n_boids;
  // double vy_align{al_ * (v_m - fixed_boid.get_vy())};
  double sum{0.};
  for (Boid& boid : flock) {
    if (in_range(max_range, min_range, boid, fixed_boid)) {sum += boid.get_vy();}
  }
  double vy_align = al_ * ((sum / n_boids) - fixed_boid.get_vy());

  if (std::fabs(vy_align) < max_v_alig) {
    return vy_align;
  }
  double sign{std::fabs(vy_align) / vy_align};
  return al_ * max_v_alig * sign;
}

/////////////////////////////
double max_v_coe{2.};
double Flock::vx_coesion(double max_range, double min_range, Boid& fixed_boid) {
  double n_boids = std::count_if(flock.begin(), flock.end(), [&](Boid& boid) {
    return in_range(max_range, min_range, boid, fixed_boid);
  });
  if (n_boids == 0) {
    return 0.;
  }
  // double x_m =
  //     std::accumulate(flock.begin(), flock.end(), 0.,
  //                     [&](double sum, Boid& boid) {
  //                       return in_range(max_range, min_range, boid,
  //                       fixed_boid)
  //                                  ? sum + boid.get_x()
  //                                  : sum;
  //                     }) /
  //     n_boids;
  // double vx_coe{coe_ * (x_m - fixed_boid.get_x()) / 3.};
  double sum{0.};
  for (Boid& boid : flock) {
    if (in_range(max_range, min_range, boid, fixed_boid)) {sum += boid.get_x();}
  }
  double vx_coe = coe_ * (sum / n_boids - fixed_boid.get_x()) / 3.;
  if (std::fabs(vx_coe) < max_v_coe) {
    return vx_coe;
  }
  double sign{std::fabs(vx_coe) / vx_coe};
  return coe_ * max_v_coe * sign;
}

double Flock::vy_coesion(double max_range, double min_range, Boid& fixed_boid) {
  int n_boids = std::count_if(flock.begin(), flock.end(), [&](Boid& boid) {
    return in_range(max_range, min_range, boid, fixed_boid);
  });
  if (n_boids == 0) {
    return 0.;
  }
  // double y_m =
  //     std::accumulate(flock.begin(), flock.end(), 0.,
  //                     [&](double sum, Boid& boid) {
  //                       return in_range(max_range, min_range, boid,
  //                       fixed_boid)
  //                                  ? sum + boid.get_y()
  //                                  : sum;
  //                     }) /
  //     n_boids;
  // double vy_coe{coe_ * (y_m - fixed_boid.get_y()) / 3.};
  double sum{0.};
  for (Boid& boid : flock) {
    if (in_range(max_range, min_range, boid, fixed_boid)) {sum += boid.get_y();}
  }
  double vy_coe = coe_ * (sum / n_boids - fixed_boid.get_y()) / 3.;
  if (std::fabs(vy_coe) < max_v_coe) {
    return vy_coe;
  }
  double sign{std::fabs(vy_coe) / vy_coe};
  return coe_ * max_v_coe * sign;
}
/////////
double center_x{(bound_xmax + bound_xmin) / 2.};
double center_y{(bound_ymax + bound_ymin) / 2.};

bool not_in_perimeter_x(Boid& boid) {
  return boid.get_x() < bound_xmin || boid.get_x() > bound_xmax;
}
double margin{100.};
bool p_velx_active(Boid& boid) {
  if (boid.get_x() < bound_xmin) {
    bool is_center_oriented = boid.get_vx() > 0.;
    return boid.get_x() < (bound_xmin - margin) ||
           (boid.get_x() > bound_xmax + margin) || is_center_oriented;
  } else if (boid.get_x() > bound_xmax) {
    bool is_center_oriented = boid.get_vx() < 0.;
    return boid.get_x() < (bound_xmin - margin) ||
           (boid.get_x() > bound_xmax + margin) || is_center_oriented;
  }
  return false;
}

bool p_slowdown_active_x(Boid& boid) {
  if (boid.get_x() < bound_xmin - margin) {
    bool is_exterior_oriented = boid.get_vx() < 0.;
    return is_exterior_oriented;
  } else if (boid.get_x() > bound_xmax + margin) {
    bool is_exterior_oriented = boid.get_vx() > 0.;
    return is_exterior_oriented;
  }
  return false;
}

bool not_in_perimeter_y(Boid& boid) {
  return boid.get_y() < bound_ymin || boid.get_y() > bound_ymax;
}

bool p_vely_active(Boid& boid) {
  if (boid.get_y() < bound_ymin) {
    bool is_center_oriented = boid.get_vy() > 0.;
    return boid.get_y() < (bound_ymin - margin) ||
           (boid.get_y() > bound_ymax + margin) || is_center_oriented;
  } else if (boid.get_y() > bound_ymax) {
    bool is_center_oriented = boid.get_vy() < 0.;
    return boid.get_y() < (bound_ymin - margin) ||
           (boid.get_y() > bound_ymax + margin) || is_center_oriented;
  }
  return false;
}

bool p_slowdown_active_y(Boid& boid) {
  if (boid.get_y() < bound_ymin - margin) {
    bool is_exterior_oriented = boid.get_vy() < 0.;
    return is_exterior_oriented;
  } else if (boid.get_y() > bound_ymax + margin) {
    bool is_exterior_oriented = boid.get_vy() > 0.;
    return is_exterior_oriented;
  }
  return false;
}

double v_perimeterx(double m, Boid& boid) {
  return m * (center_x - boid.get_x());
}

double v_perimetery(double m, Boid& boid) {
  return m * (center_y - boid.get_y());
}

bool in_explosion_range(double expl_centerx, double expl_centery,
                        double expl_range, Boid& boid) {
  return (std::fabs(boid.get_x() - expl_centerx) < expl_range &&
          std::fabs(boid.get_y() - expl_centery) < expl_range);
}
double max_expl_vel{35.};
double expl_velocity_x(double b, double expl_centerx, Boid& boid) {
  double expl_velx = b / (boid.get_x() / 300. - expl_centerx / 300.);
  if (std::fabs(expl_velx) < max_expl_vel) return expl_velx;
  double sign = std::fabs(expl_velx) / expl_velx;
  return max_expl_vel * sign;
}

double expl_velocity_y(double b, double expl_centery, Boid& boid) {
  double expl_vely = b / (boid.get_y() / 300. - expl_centery / 300.);
  if (std::fabs(expl_vely) < max_expl_vel) return expl_vely;
  double sign = std::fabs(expl_vely) / expl_vely;
  return max_expl_vel * sign;
}
