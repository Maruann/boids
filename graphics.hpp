#ifndef GRAPHICS
#define GRAPHICS

#include "flock.hpp"
#include "r_numbers.hpp"
enum parameter_index { sep = 0, ali, coh };
enum click_state { clicked = 0, unclicked };

class Button {
  float x_;  // LE POSIZIONI COINCIDERANNO CON IL CENTRO DEL BOTTONE
  float y_;
  sf::ConvexShape shape_;
  sf::Color idle_color_;
  sf::Color semiactive_color_;
  sf::Color active_color_;
  double increment_;
  int parameter_index_;

 public:
  Button(float x, float y, sf::ConvexShape& shape, sf::Color& idle_color,
         sf::Color& semiactive_color, sf::Color& active_color, double increment,
         int parameter_index)
      : x_{x},
        y_{y},
        shape_{shape},
        idle_color_{idle_color},
        semiactive_color_{semiactive_color},
        active_color_{active_color},
        increment_{increment},
        parameter_index_{parameter_index} {
    shape_.setPosition(x, y);
  }
  void update(Flock& flock, sf::RenderWindow& window,
              int& click_state);  // IMPORCIANCE: la finestra in input serve per
                                 // prendere la posizione del mouse rispetto alla
                                 // finestra (e non al desktop)
  void draw(sf::RenderWindow& window) { window.draw(shape_); }
};

void graphics(Flock stormo);

#endif