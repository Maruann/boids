#include "boids.hpp"

bool not_in_perimeter_x(Boid& boid);
bool p_velx_active(Boid& boid);
bool p_slowdown_active_x(Boid& boid);
bool not_in_perimeter_y(Boid& boid);
bool p_vely_active(Boid& boid);
bool p_slowdown_active_y(Boid& boid);
double v_perimeterx(double m, Boid& boid);
double v_perimetery(double m, Boid& boid);

inline double boom_positionx = 100000.;
inline double boom_positiony = 100000.;
bool in_explosion_range(double expl_centerx, double expl_centery,
                        double expl_range, Boid& boid);
double expl_velocity_x(double b, double expl_centerx, Boid& boid);
double expl_velocity_y(double b, double expl_centery, Boid& boid);