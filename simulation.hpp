#ifndef SIMULATION_H
#define SIMULATION_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

#include "flock.hpp"

void update(Flock& flock, int steps_per_evolution, sf::Time delta_t);

void evolve(Flock& stormo, float delta_t);

Boid solve(Flock& stormo, double delta_t, Boid& boid);

double orientation(double vx, double vy);

#endif