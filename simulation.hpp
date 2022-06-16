#ifndef SIMULATION_H
#define SIMULATION_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

#include "flock.hpp"

void update(Flock& flock, int steps_per_evolution, double delta_t);

double orientation(double vx, double vy);

#endif