#ifndef GRAPHICS
#define GRAPHICS

#include "flock.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/Time.hpp>


//enumerations usate nella implementazione grafica
enum parameter_index { sep = 0, ali, coh, vis };
enum click_state { clicked = 0, unclicked };
enum color_choice { colored = 0, uncolored };
enum rect_origin {center_origin = 0, bottom_left_origin, bottom_right_origin };

std::string roundto(double num, int n);

void shape_init_setting(sf::ConvexShape& shape_name,
                        std::vector<sf::Vector2f>& vector,
                        float outl_thickness, float button_scale,
                        int color_choice, sf::Color fill_color);

void rect_init_setting(sf::RectangleShape& rect, float width,
                       float height, float outl_thickness,
                       sf::Color fill_color, int origin_choice,
                       float posit_x, float posit_y);

void text_init_setting(sf::Text& text_name, sf::Font& font, int char_size,
                       float outl_thickness,
                       std::string text_to_display,
                       sf::Color fill_color, float posit_x,
                       float posit_y);
void statistics_update(Flock& stormo, double& mean_dis, sf::Text& mean_dis_text,
                       double& std_dev_dis, sf::Text& std_dev_dis_text,
                       double& mean_vel, sf::Text& mean_vel_text,
                       double& std_dev_vel, sf::Text& std_dev_vel_text,
                       double display_width, double display_height,
                       double stat_rectangle_width,
                       double stat_rectangle_height, sf::Clock clock);
class Button {
  // le posizioni coincideranno con il centro del bottone
  float x_;  
  float y_;
  sf::ConvexShape shape_;
  sf::Color idle_color_;
  sf::Color semiactive_color_;
  sf::Color active_color_;
  double increment_;
  int parameter_index_;

 public:
  Button(float x, float y, sf::ConvexShape shape, sf::Color idle_color,
         sf::Color semiactive_color, sf::Color active_color, double increment,
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

              int click_state, double& vision, sf::Text& text, double display_height, double menu_rectangle_width, double menu_rectangle_height); 
                             
  void draw(sf::RenderWindow& window) const;
};

void graphics(Flock& stormo);

#endif