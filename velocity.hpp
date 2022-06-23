#ifndef VELOCITY
#define VELOCITY

#include "boids.hpp"

// Header File con le dichiarazioni delle funzioni definite nella TU di
// velocity.cpp

bool not_in_perimeter_x(Boid const& boid);
bool p_velx_active(Boid const& boid);
bool p_slowdown_active_x(Boid const& boid);
bool not_in_perimeter_y(Boid const& boid);
bool p_vely_active(Boid const& boid);
bool p_slowdown_active_y(Boid const& boid);
double v_perimeterx(double m, Boid const& boid);
double v_perimetery(double m, Boid const& boid);

inline double boom_positionx{10000.};
inline double boom_positiony{10000.};
bool in_explosion_range(double expl_centerx, double expl_centery,
                        double expl_range, Boid const& boid);
double expl_velocity_x(double b, double expl_centerx, Boid const& boid);
double expl_velocity_y(double b, double expl_centery, Boid const& boid);

#endif