#ifndef SIMULATION_H
#define SIMULATION_H

#include "flock.hpp"

void update(Flock& flock, int steps_per_evolution, double delta_t, double dist_mult);

double orientation(double vx, double vy);

#endif