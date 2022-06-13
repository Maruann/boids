#ifndef SIMULATION_H
#define SIMULATION_H

#include "flock.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

void update(Flock& flock, int steps_per_evolution, sf::Time delta_t);

void evolve(Flock& stormo, double delta_t);

Boid solve(Flock& stormo, double delta_t, Boid& boid);


#endif