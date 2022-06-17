#include "graphics.hpp"
  void Button::update(Flock& flock) {
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
          break;
        case ali:
          flock.set_ali(flock.get_ali() + increment_);
          break;
        case coh:
          flock.set_coe(flock.get_coe() + increment_);
      }
    } else {
      shape_.setFillColor(idle_color_);
    }
  }