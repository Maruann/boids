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

int main(){
    Flock flock{1., 1., 1.};
    flock.fill(10);


    auto const delta_t{sf::milliseconds(1)};
    int const fps = 30;
    int const steps_per_evolution{1000 / fps};

    unsigned const display_width = 100;
    unsigned const display_height = 100;

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

        auto const state = evolve(flock, steps_per_evolution, delta_t);

        for (auto& boid : state){
            circ.setPosition(boid.get_x, boid.get_y);
            window.draw(circ); //disegna sull' oggetto window, ma non la "displaya" ancora
        }

     window.display();   //adesso display
    }

}
