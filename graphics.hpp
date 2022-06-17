#ifndef GRAPHICS
#define GRAPHICS

#include "flock.hpp"
#include "r_numbers.hpp"
enum parameter_index { sep = 0, ali, coh };

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
  Button(float x, float y, sf::ConvexShape shape, sf::Color idle_color,
         sf::Color semiactive_color, sf::Color active_color, double increment)
      : x_{x},
        y_{y},
        shape_{shape},
        idle_color_{idle_color},
        semiactive_color_{semiactive_color},
        active_color_{active_color},
        increment_{increment},
        parameter_index_{sep} {
    shape.setPosition(x_, y_);
  }
  void update(Flock& flock);
  void draw(sf::RenderWindow window) { window.draw(shape_); }
};

#endif