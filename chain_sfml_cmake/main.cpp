#include "chain.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

auto evolve(Chain& chain, int steps_per_evolution, sf::Time delta_t) //è diversa dalla evolve che avevamo implementato, ma implementata usando l'evolution del chain
{
  double const dt{delta_t.asSeconds()}; //converti delta_ in un double perchè evolution di chain funziona con i double

  for (int i{0}; i != steps_per_evolution; ++i) {
    chain.evolve(dt); //usi la vecchia evoluzione tot volte
  }

  return chain.state(); //restituisci il valore che chain ha dopo tot iterazioni
}

int main()
{
  // Molla con:
  // k = 1 N/m
  // l = 1 cm
  Hooke hooke{1.0, 0.01};

  // Catena con 10 punti materiali:
  // m: 50 g per ogni punto
  // x: un punto ogni 1.1 cm (partendo da 0.0)
  // v: tutti i punti hanno v = 0 m/s
  Chain chain(hooke);

  double const p_m{0.05};

  double p_x{0.0};
  double delta_x{0.011};

  for (auto const p_v : {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}) {
    chain.push_back({p_m, p_x, p_v});
    p_x += delta_x;
  };

  auto const delta_t{sf::milliseconds(1)}; //1 gennaio 1970 è il tempo standard da cui si inizia a contare
  int const fps = 30;
  int const steps_per_evolution{1000 / fps};

  unsigned const display_width = .9 * sf::VideoMode::getDesktopMode().width; //dici che la larghezza è il novanta /% dello schermo
  unsigned const display_height{300};

  auto const min_x{-0.025}; //sono parametri per dire dove vuoi il centro del sist di riferimento dei pallini (per sfml centro è in alto a sx ma tu non lo vuoi lì perchè le posizioni dei tuoi punti sono rispetto a un "centro che è al centro".
  auto const max_x{0.125};
  auto const scale_x = display_width / (max_x - min_x);

  sf::RenderWindow window(sf::VideoMode(display_width, display_height),
                          "Hooke Chain Evolution"); //tiolo
  window.setFramerateLimit(fps); //indica il limite oltre il quale evolve non può amdare

  sf::CircleShape circ{18.0f};
  circ.setFillColor(sf::Color::Blue);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear(sf::Color::White); //cancelli(sfondo bianco)

    auto const state = evolve(chain, steps_per_evolution, delta_t);

    for (auto& pps : state) {
      circ.setPosition((pps.x - min_x) * scale_x, display_height * 0.5); //tralsazione e scaling (lo zero in SFML è in alto a sx di default), min x è traslazione a dx
      window.draw(circ);
    }

    window.display();
  }
}
