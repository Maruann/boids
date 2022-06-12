#include "flock.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

auto evolve(Flock& flock, int steps_per_evolution, sf::Time delta_t){
    double const dt{delta_t.asSeconds()};

    for(int s{0}; s != steps_per_evolution; ++s){
        evolve(flock,dt);
    }

    return flock.get_flock(); 
}

int main(){
    Flock stormo{0.5, 0.5, 0.5};
    stormo.fill(1);


    auto const delta_t{sf::milliseconds(1)};
    int const fps = 30;
    int const steps_per_evolution{1000 / fps};

    unsigned const int display_width = 100;
    unsigned const int display_height = 100;

    sf::RenderWindow window(sf::VideoMode(display_width, display_height), "BOOOIDZZZZZ");
    
    window.setFramerateLimit(fps);

    sf::CircleShape circ{5.0f};
    circ.setFillColor(sf::Color::Blue);

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
        }

        window.clear(sf::Color::White);

        auto const state = evolve(stormo, steps_per_evolution, delta_t);

        for (auto& boid : state){
            circ.setPosition(boid.get_x(), boid.get_y());
            window.draw(circ); //disegna sull' oggetto window, ma non la "displaya" ancora
        }

     window.display();   //adesso display
    }

}
