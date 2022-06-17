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

  void update(Flock flock) {
    if (shape_.getGlobalBounds().contains(sf::Mouse::getPosition().x,
                                          sf::Mouse::getPosition().y)) {
      shape_.setFillColor(active_color_);
    } else if (shape_.getGlobalBounds().contains(sf::Mouse::getPosition().x,
                                                 sf::Mouse::getPosition().y) &&
               sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      shape_.setFillColor(semiactive_color_);
      switch (parameter_index_) {
        case sep:
          flock.set_sep(flock.get_sep() + increment_);
        case ali:
          flock.set_ali(flock.get_ali() + increment_);
        case coh:
          flock.set_coe(flock.get_coe() + increment_);
      }
    } else {
      shape_.setFillColor(idle_color_);
    }
  }
  void draw(sf::RenderWindow window) { window.draw(shape_); }
};