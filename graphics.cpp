#include "graphics.hpp"
void Button::update(Flock& flock, sf::RenderWindow& window, int& click_state) {
  if ((shape_.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,
                                         sf::Mouse::getPosition(window).y)) &&
      click_state == clicked) {
    shape_.setFillColor(active_color_);
    switch (parameter_index_) {
      case sep:
        flock.set_sep(flock.get_sep() + increment_);
        break;
      case ali:
        flock.set_ali(flock.get_ali() + increment_);
        break;
      case coh:
        flock.set_coe(flock.get_coe() + increment_);
        break;
      default:
        break;
    }
  } else if (shape_.getGlobalBounds().contains(
                 sf::Mouse::getPosition(window).x,
                 sf::Mouse::getPosition(window).y)) {
    shape_.setFillColor(semiactive_color_);
  } else {
    shape_.setFillColor(idle_color_);
  }
}