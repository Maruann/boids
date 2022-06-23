#ifndef SIMULATION_H
#define SIMULATION_H

#include "flock.hpp"

// Header File che dichiara le funzioni definite nella TU di simulation.cpp

void update(Flock& flock, int steps_per_evolution, double delta_t,
            double dist_mult);

double orientation(double vx, double vy);
int input_reader();

#endif