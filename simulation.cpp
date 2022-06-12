#include "flock.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

auto evolve(Flock& flock, int steps_per_evolution, sf::Time delta_t){
    double const dt{delta_t.asSeconds()};

    for(int i{0}; i != steps_per_evolution; ++i){
        flock.evolve(dt);
    }

    return flock.state(); //dobbiamo definire una funzione che ritorni lo stato dello stormo
}