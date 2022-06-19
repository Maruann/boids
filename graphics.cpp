#include "graphics.hpp"
#include "velocity.hpp"
#include "simulation.hpp"
#include <sstream>

class FPS
{ 
public:
	/// @brief Constructor with initialization.
	///
	FPS() : mFrame(0), mFps(0) {}

	/// @brief Update the frame count.
	/// 


	/// @brief Get the current FPS count.
	/// @return FPS count.
	const unsigned int getFPS() const { return mFps; }

private:
	unsigned int mFrame;
	unsigned int mFps;
	sf::Clock mClock;

public:
	void update()
	{
		if(mClock.getElapsedTime().asSeconds() >= 1.f)
		{
			mFps = mFrame;
			mFrame = 0;
			mClock.restart();
		}
 
		++mFrame;
	}
};

std::string roundto(double num, int n){
  std::string string{std::to_string(num)};
  int i{1};
  for(char& character : string){
  if(character == '.'){
      break;}
  else{++i;}
  }
  return (string.erase(i + n));
  }

void Button::update(Flock& flock, sf::RenderWindow& window, int click_state, double& vision, sf::Text& text, double display_height, double menu_rectangle_width, double menu_rectangle_height) {
  if ((shape_.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,
                                         sf::Mouse::getPosition(window).y)) &&
      click_state == clicked) {
    shape_.setFillColor(active_color_);
    switch (parameter_index_) {
      case sep:
        flock.set_sep(flock.get_sep() + increment_);
        if(flock.get_sep() > 10.){flock.set_sep(10.);}
        else if(flock.get_sep() < 0.){flock.set_sep(0.);}
        text.setString(roundto(flock.get_sep(), 2));
        text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
        text.setPosition(menu_rectangle_width * (13.f / 80.f), (display_height - menu_rectangle_height * (5.f / 11.f)));
        break;
      case ali:
        flock.set_ali(flock.get_ali() + increment_);
        if(flock.get_ali() > 10.){flock.set_ali(10.);}
        else if(flock.get_ali() < 0.){flock.set_ali(0.);}
        text.setString(roundto(flock.get_ali(), 2));
        text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
        text.setPosition(menu_rectangle_width * (33.f / 80.f), (display_height - menu_rectangle_height * (5.f / 11.f)));
        break;
      case coh:
        flock.set_coe(flock.get_coe() + increment_);
        if(flock.get_coe() > 10.){flock.set_coe(10.);}
        else if(flock.get_coe() < 0.){flock.set_coe(0.);}
        text.setString(roundto(flock.get_coe(), 2));
        text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
        text.setPosition(menu_rectangle_width * (53.f / 80.f), (display_height - menu_rectangle_height * (5.f / 11.f)));
        break;
      case vis:
        vision += increment_;
        if(vision > 2.){vision = 2.;}
        else if(vision < 0.){vision = 0.;}
        text.setString(roundto(vision, 2));
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

void statistics_update(Flock stormo, double& mean_dis, sf::Text& mean_dis_text, double& std_dis, sf::Text& std_dis_text, double& mean_vel, sf::Text& mean_vel_text, double& std_vel, sf::Text& std_vel_text, double display_width, double display_height, double stat_rectangle_width, double stat_rectangle_height){
      mean_dis = stormo.mean_distance();
      mean_dis_text.setString(roundto(mean_dis, 1));
      mean_dis_text.setOrigin(mean_dis_text.getGlobalBounds().width / 2.f, mean_dis_text.getGlobalBounds().height / 2.f);
      mean_dis_text.setPosition(display_width - stat_rectangle_width * (3.f / 5.f),(display_height - stat_rectangle_height * (17.f / 30.f)));
      std_dis = stormo.stnd_deviation_distance(mean_dis);
      std_dis_text.setString(roundto(std_dis, 1));
      std_dis_text.setOrigin(std_dis_text.getGlobalBounds().width / 2.f, std_dis_text.getGlobalBounds().height / 2.f);
      std_dis_text.setPosition(display_width - stat_rectangle_width * (3.f / 5.f),(display_height - stat_rectangle_height * (7.f / 30.f)));

      mean_vel = stormo.mean_velocity();
      mean_vel_text.setString(roundto(mean_vel, 1));
      mean_vel_text.setOrigin(mean_vel_text.getGlobalBounds().width / 2.f, mean_vel_text.getGlobalBounds().height / 2.f);
      mean_vel_text.setPosition(display_width - stat_rectangle_width * (1.f / 5.f),(display_height - stat_rectangle_height * (17.f / 30.f)));
      std_vel = stormo.stnd_deviation_velocity(mean_vel);
      std_vel_text.setString(roundto(std_vel, 1));
      std_vel_text.setOrigin(std_vel_text.getGlobalBounds().width / 2.f, std_vel_text.getGlobalBounds().height / 2.f);
      std_vel_text.setPosition(display_width - stat_rectangle_width * (1.f / 5.f),(display_height - stat_rectangle_height * (7.f / 30.f)));
    }

void graphics(Flock& stormo){
auto const delta_t{sf::milliseconds(1)};
  double const dt{delta_t.asSeconds()};
  int const fps = 25;
  int const steps_per_evolution{200 / fps};

  const int display_width = 1280;  
  const int display_height = 720;

  double dist_mult = 1.;

  FPS fps_count;

  sf::RenderWindow window(sf::VideoMode(display_width, display_height),
                          "Flock Simulation", sf::Style::Titlebar);
                          
  window.setFramerateLimit(fps);
  sf::ConvexShape convex;  // genero una forma geometrica come modello del
                           // singolo boid (unisco i 6 punti che genero sotto)
  convex.setFillColor(sf::Color::Black);
  convex.setPointCount(6);

  convex.setPoint(0, sf::Vector2f(30, 60));
  convex.setPoint(1, sf::Vector2f(10, 70));
  convex.setPoint(2, sf::Vector2f(5, 50));
  convex.setPoint(3, sf::Vector2f(30, 0));
  convex.setPoint(4, sf::Vector2f(55, 50));
  convex.setPoint(5, sf::Vector2f(50, 70));

  convex.setScale(0.1f, 0.1f);

  convex.setOrigin(
      sf::Vector2f(30, 35));  // setto l'origine locale (punto attorno a cui
                              // ruota il singolo boid)

  sf::Texture background;
  background.loadFromFile("./boid_utilities/img/background.png");
  sf::Sprite background_sprite(background);
  background_sprite.setScale(0.68, 0.68);

  sf::Texture boom_texture;
  boom_texture.loadFromFile(
      "./boid_utilities/sheet_sprites/boom_sheetsprite.png");

  sf::IntRect rect_boom_sprite(0, 0, 448 / 8, 56);
  sf::Sprite boom_sprite(boom_texture, rect_boom_sprite);
  boom_sprite.setOrigin(448 / 16, 28);

  sf::SoundBuffer boom_sound_buffer;
  boom_sound_buffer.loadFromFile("./boid_utilities/audio/boom_sound.ogg");
  sf::Sound boom_sound;
  boom_sound.setBuffer(boom_sound_buffer);

  ///////////////// PARTE DEGLI "SLIDER"

  // rettangolo del menu
  sf::Color menu_color(149, 149, 149, 255);//colore dei rettangoli del menu
  float menu_rectangle_width = static_cast<float>(display_width * (3.f / 5.f)); //SEMBRA UNA SCELTA ILLOGICA MA HA SENSO PER SOTTO
  float menu_rectangle_height = static_cast<float>(display_height / 10);
  sf::RectangleShape menu_rectangle(sf::Vector2f(menu_rectangle_width,  menu_rectangle_height));
  menu_rectangle.setOrigin(0.f, menu_rectangle.getSize().y);
  menu_rectangle.setFillColor(menu_color);
  menu_rectangle.setOutlineThickness(-5.f);
  menu_rectangle.setOutlineColor(sf::Color::White);
  menu_rectangle.setPosition(0.f, static_cast<float>(display_height));

  //rettangolo per il display statistico
  float stat_rectangle_width = menu_rectangle_width * (3.f / 5.f); //SEMBRA UNA SCELTA ILLOGICA MA HA SENSO PER SOTTO
  float stat_rectangle_height = menu_rectangle_height * (3.f / 2.f);
  sf::RectangleShape stat_rectangle(sf::Vector2f(stat_rectangle_width,  stat_rectangle_height));
  stat_rectangle.setOrigin(stat_rectangle_width, stat_rectangle_height);
  stat_rectangle.setFillColor(menu_color);
  stat_rectangle.setOutlineThickness(-5.f);
  stat_rectangle.setOutlineColor(sf::Color::White);
  stat_rectangle.setPosition(static_cast<float>(display_width), static_cast<float>(display_height));
  
  //rettangoli per il testo dei parametri
  sf::Color text_color(sf::Color::White);
  float text_rectangle_width = menu_rectangle_width * (8.f / 60.f); 
  float text_rectangle_height = menu_rectangle_height * (1.f / 3.f);
  sf::RectangleShape sep_text_rectangle(sf::Vector2f(text_rectangle_width,  text_rectangle_height));
  sep_text_rectangle.setOrigin(text_rectangle_width / 2.f, text_rectangle_height / 2.f);
  sep_text_rectangle.setPosition(menu_rectangle_width * (13.f / 80.f),(display_height - menu_rectangle_height * (2.f / 5.f)));
  sep_text_rectangle.setFillColor(text_color);
  sf::RectangleShape ali_text_rectangle(sf::Vector2f(text_rectangle_width,  text_rectangle_height));
  ali_text_rectangle.setOrigin(text_rectangle_width / 2.f, text_rectangle_height / 2.f);
  ali_text_rectangle.setPosition(menu_rectangle_width * (33.f / 80.f),(display_height - menu_rectangle_height * (2.f / 5.f)));
  ali_text_rectangle.setFillColor(text_color);
  sf::RectangleShape coh_text_rectangle(sf::Vector2f(text_rectangle_width,  text_rectangle_height));
  coh_text_rectangle.setOrigin(text_rectangle_width / 2.f, text_rectangle_height / 2.f);
  coh_text_rectangle.setPosition(menu_rectangle_width * (53.f / 80.f),(display_height - menu_rectangle_height * (2.f / 5.f)));
  coh_text_rectangle.setFillColor(text_color);
  sf::RectangleShape vis_text_rectangle(sf::Vector2f(text_rectangle_width,  text_rectangle_height));
  vis_text_rectangle.setOrigin(text_rectangle_width / 2.f, text_rectangle_height / 2.f);
  vis_text_rectangle.setPosition(menu_rectangle_width * (73.f / 80.f),(display_height - menu_rectangle_height * (2.f / 5.f)));
  vis_text_rectangle.setFillColor(text_color);
  
  //rettangoli per il testo dei dati statistici
  sf::RectangleShape mean_dis_text_rectangle(sf::Vector2f(text_rectangle_width,  text_rectangle_height));
  mean_dis_text_rectangle.setOrigin(text_rectangle_width / 2.f, text_rectangle_height / 2.f);
  mean_dis_text_rectangle.setPosition(display_width - stat_rectangle_width * (3.f / 5.f),(display_height - stat_rectangle_height * (3.f / 15.f)));
  mean_dis_text_rectangle.setFillColor(text_color);
  sf::RectangleShape std_dis_text_rectangle(sf::Vector2f(text_rectangle_width,  text_rectangle_height));
  std_dis_text_rectangle.setOrigin(text_rectangle_width / 2.f, text_rectangle_height / 2.f);
  std_dis_text_rectangle.setPosition(display_width - stat_rectangle_width * (3.f / 5.f),(display_height - stat_rectangle_height * (8.f / 15.f)));
  std_dis_text_rectangle.setFillColor(text_color);
  sf::RectangleShape mean_vel_text_rectangle(sf::Vector2f(text_rectangle_width,  text_rectangle_height));
  mean_vel_text_rectangle.setOrigin(text_rectangle_width / 2.f, text_rectangle_height / 2.f);
  mean_vel_text_rectangle.setPosition(display_width - stat_rectangle_width * (1.f / 5.f),(display_height - stat_rectangle_height * (3.f / 15.f)));
  mean_vel_text_rectangle.setFillColor(text_color);
  sf::RectangleShape std_vel_text_rectangle(sf::Vector2f(text_rectangle_width,  text_rectangle_height));
  std_vel_text_rectangle.setOrigin(text_rectangle_width / 2.f, text_rectangle_height / 2.f);
  std_vel_text_rectangle.setPosition(display_width - stat_rectangle_width * (1.f / 5.f),(display_height - stat_rectangle_height * (8.f / 15.f)));
  std_vel_text_rectangle.setFillColor(text_color);
  
  
  //bottoni
  float button_scale = 10.f;
  float button_outline_thickness = -0.2f;
  sf::ConvexShape up;
  up.setPointCount(3);
  up.setOutlineThickness(button_outline_thickness);
  up.setOutlineColor(sf::Color::Black);
  up.setPoint(0, sf::Vector2f(1, 0));
  up.setPoint(1, sf::Vector2f(2, 2));
  up.setPoint(2, sf::Vector2f(0, 2));
  up.setScale(button_scale, button_scale);
  up.setOrigin(sf::Vector2f(1, 1));

  sf::ConvexShape down;
  down.setPointCount(3);
  down.setOutlineThickness(button_outline_thickness);
  down.setOutlineColor(sf::Color::Black);
  down.setPoint(0, sf::Vector2f(0, 0));
  down.setPoint(1, sf::Vector2f(2, 0));
  down.setPoint(2, sf::Vector2f(1, 2));
  down.setScale(button_scale, button_scale);
  down.setOrigin(sf::Vector2f(1, 1));

  sf::ConvexShape double_up;
  double_up.setPointCount(7);
  double_up.setOutlineThickness(button_outline_thickness);
  double_up.setOutlineColor(sf::Color::Black);
  double_up.setPoint(0, sf::Vector2f(1, 0));
  double_up.setPoint(1, sf::Vector2f(2, 1));
  double_up.setPoint(2, sf::Vector2f(1, 1));
  double_up.setPoint(3, sf::Vector2f(2, 2));
  double_up.setPoint(4, sf::Vector2f(0, 2));
  double_up.setPoint(5, sf::Vector2f(1, 1));
  double_up.setPoint(6, sf::Vector2f(0, 1));
  double_up.setScale(button_scale, button_scale);
  double_up.setOrigin(sf::Vector2f(1, 1));

  sf::ConvexShape double_down;
  double_down.setPointCount(7);
  double_down.setOutlineThickness(button_outline_thickness);
  double_down.setOutlineColor(sf::Color::Black);
  double_down.setPoint(0, sf::Vector2f(0, 0));
  double_down.setPoint(1, sf::Vector2f(2, 0));
  double_down.setPoint(2, sf::Vector2f(1, 1));
  double_down.setPoint(3, sf::Vector2f(2, 1));
  double_down.setPoint(4, sf::Vector2f(1, 2));
  double_down.setPoint(5, sf::Vector2f(0, 1));
  double_down.setPoint(6, sf::Vector2f(1, 1));
  double_down.setScale(button_scale, button_scale);
  double_down.setOrigin(sf::Vector2f(1,1));

  sf::Color sep_idle{240, 67, 67, 255};//ROSSO
  sf::Color sep_semiactive{158, 24, 24, 255};
  sf::Color sep_active{94, 9, 9, 255};
  sf::Color ali_idle{82, 219, 61, 255};//VERDE
  sf::Color ali_semiactive{34, 143, 17, 255};
  sf::Color ali_active{17, 87, 6, 255};
  sf::Color coh_idle{77, 102, 232, 255};//BLU
  sf::Color coh_semiactive{17, 38, 143, 255};
  sf::Color coh_active{11, 26, 102, 255};
  sf::Color vis_idle{231, 237, 62, 255};//GIALLO
  sf::Color vis_semiactive{176, 181, 29, 255};
  sf::Color vis_active{119, 122, 9, 255};


  Button sep_up{menu_rectangle_width * (3.f/80.f), (display_height - menu_rectangle_height * (3.f / 4.f)), up, sep_idle, sep_semiactive, sep_active, 0.25, sep};
  Button sep_down{menu_rectangle_width * (3.f/80.f), (display_height - menu_rectangle_height * (1.f / 4.f)), down, sep_idle, sep_semiactive, sep_active, -0.25, sep};
  Button sep_dou_up{menu_rectangle_width * (6.f/80.f), (display_height - menu_rectangle_height * (3.f / 4.f)), double_up, sep_idle, sep_semiactive, sep_active, 2, sep};
  Button sep_dou_down{menu_rectangle_width * (6.f/80.f), (display_height - menu_rectangle_height * (1.f / 4.f)), double_down, sep_idle, sep_semiactive, sep_active, -2, sep};

  Button ali_up{menu_rectangle_width * (23.f/80.f), (display_height - menu_rectangle_height * (3.f / 4.f)), up, ali_idle, ali_semiactive, ali_active, 0.25, ali};
  Button ali_down{menu_rectangle_width * (23.f/80.f), (display_height - menu_rectangle_height * (1.f / 4.f)), down, ali_idle, ali_semiactive, ali_active, -0.25, ali};
  Button ali_dou_up{menu_rectangle_width * (26.f/80.f), (display_height - menu_rectangle_height * (3.f / 4.f)), double_up, ali_idle, ali_semiactive, ali_active, 2, ali};
  Button ali_dou_down{menu_rectangle_width * (26.f/80.f), (display_height - menu_rectangle_height * (1.f / 4.f)), double_down, ali_idle, ali_semiactive, ali_active, -2, ali};

  Button coh_up{menu_rectangle_width * (43.f/80.f), (display_height - menu_rectangle_height * (3.f / 4.f)), up, coh_idle, coh_semiactive, coh_active, 0.25, coh};
  Button coh_down{menu_rectangle_width * (43.f/80.f), (display_height - menu_rectangle_height * (1.f / 4.f)), down, coh_idle, coh_semiactive, coh_active, -0.25, coh};
  Button coh_dou_up{menu_rectangle_width * (46.f/80.f), (display_height - menu_rectangle_height * (3.f / 4.f)), double_up, coh_idle, coh_semiactive, coh_active, 2, coh};
  Button coh_dou_down{menu_rectangle_width * (46.f/80.f), (display_height - menu_rectangle_height * (1.f / 4.f)), double_down, coh_idle, coh_semiactive, coh_active, -2, coh};

  Button vis_up{menu_rectangle_width * (63.f/80.f), (display_height - menu_rectangle_height * (3.f / 4.f)), up, vis_idle, vis_semiactive, vis_active, 0.05, vis};
  Button vis_down{menu_rectangle_width * (63.f/80.f), (display_height - menu_rectangle_height * (1.f / 4.f)), down, vis_idle, vis_semiactive, vis_active, -0.05, vis};
  Button vis_dou_up{menu_rectangle_width * (66.f/80.f), (display_height - menu_rectangle_height * (3.f / 4.f)), double_up, vis_idle, vis_semiactive, vis_active, 0.2, vis};
  Button vis_dou_down{menu_rectangle_width * (66.f/80.f), (display_height - menu_rectangle_height * (1.f / 4.f)), double_down, vis_idle, vis_semiactive, vis_active, -0.2, vis};

  std::array<Button, 4> sep_buttons{sep_up, sep_down, sep_dou_up, sep_dou_down};
  std::array<Button, 4> ali_buttons{ali_up, ali_down, ali_dou_up, ali_dou_down};
  std::array<Button, 4> coh_buttons{coh_up, coh_down, coh_dou_up, coh_dou_down};
  std::array<Button, 4> vis_buttons{vis_up, vis_down, vis_dou_up, vis_dou_down};

  //testi
  sf::Font font;
  font.loadFromFile("./boid_utilities/fonts/arial.ttf");

      //testi titoli parametri
  sf::Text sep_title_text;
  sep_title_text.setFont(font);
  sep_title_text.setCharacterSize(16);
  sep_title_text.setOutlineThickness(1.f);
  sep_title_text.setOutlineColor(sf::Color::Black);
  sep_title_text.setString("Separation");
  sep_title_text.setFillColor(sep_idle);
  sep_title_text.setOrigin(sep_title_text.getGlobalBounds().width / 2.f, sep_title_text.getGlobalBounds().height / 2.f);
  sep_title_text.setPosition(menu_rectangle_width * (13.f / 80.f), (display_height - menu_rectangle_height * (9.f / 11.f)));
  sf::Text ali_title_text;
  ali_title_text.setFont(font);
  ali_title_text.setCharacterSize(16);
  ali_title_text.setOutlineThickness(1.f);
  ali_title_text.setOutlineColor(sf::Color::Black);
  ali_title_text.setString("Alignment");
  ali_title_text.setFillColor(ali_idle);
  ali_title_text.setOrigin(ali_title_text.getGlobalBounds().width / 2.f, ali_title_text.getGlobalBounds().height / 2.f);
  ali_title_text.setPosition(menu_rectangle_width * (33.f / 80.f), (display_height - menu_rectangle_height * (9.f / 11.f)));
  sf::Text coh_title_text;
  coh_title_text.setFont(font);
  coh_title_text.setCharacterSize(16);
  coh_title_text.setOutlineThickness(1.f);
  coh_title_text.setOutlineColor(sf::Color::Black);
  coh_title_text.setString("Cohesion");
  coh_title_text.setFillColor(coh_idle);
  coh_title_text.setOrigin(coh_title_text.getGlobalBounds().width / 2.f, coh_title_text.getGlobalBounds().height / 2.f);
  coh_title_text.setPosition(menu_rectangle_width * (53.f / 80.f), (display_height - menu_rectangle_height * (9.f / 11.f)));
  sf::Text vis_title_text;
  vis_title_text.setFont(font);
  vis_title_text.setCharacterSize(16);
  vis_title_text.setOutlineThickness(1.f);
  vis_title_text.setOutlineColor(sf::Color::Black);
  vis_title_text.setString("Vision");
  vis_title_text.setFillColor(vis_idle);
  vis_title_text.setOrigin(vis_title_text.getGlobalBounds().width / 2.f, vis_title_text.getGlobalBounds().height / 2.f);
  vis_title_text.setPosition(menu_rectangle_width * (73.f / 80.f), (display_height - menu_rectangle_height * (9.f / 11.f)));

      //testi titoli dati statistici
  sf::Text boid_distance_title_text;
  boid_distance_title_text.setFont(font);
  boid_distance_title_text.setCharacterSize(16);
  boid_distance_title_text.setString("Boids Distance");
  boid_distance_title_text.setFillColor(sf::Color::Black);
  boid_distance_title_text.setOrigin(boid_distance_title_text.getGlobalBounds().width / 2.f, boid_distance_title_text.getGlobalBounds().height / 2.f);
  boid_distance_title_text.setPosition(display_width - stat_rectangle_width * (3.f / 5.f),(display_height - stat_rectangle_height * (13.f / 15.f)));
  sf::Text boid_vel_title_text;
  boid_vel_title_text.setFont(font);
  boid_vel_title_text.setCharacterSize(16);
  boid_vel_title_text.setString("Boids Velocity");
  boid_vel_title_text.setFillColor(sf::Color::Black);
  boid_vel_title_text.setOrigin(boid_vel_title_text.getGlobalBounds().width / 2.f, boid_vel_title_text.getGlobalBounds().height / 2.f);
  boid_vel_title_text.setPosition(display_width - stat_rectangle_width * (1.f / 5.f),(display_height - stat_rectangle_height * (13.f / 15.f)));
  sf::Text mean_title_text;
  mean_title_text.setFont(font);
  mean_title_text.setCharacterSize(16);
  mean_title_text.setString("Mean:");
  mean_title_text.setFillColor(sf::Color::Black);
  mean_title_text.setOrigin(0.f, mean_title_text.getGlobalBounds().height / 2.f);
  mean_title_text.setPosition(display_width - stat_rectangle_width * (19.f / 20.f),(display_height - stat_rectangle_height * (17.f / 30.f)));
  sf::Text std_title_text;
  std_title_text.setFont(font);
  std_title_text.setCharacterSize(16);
  std_title_text.setString("Stnd. Dev:");
  std_title_text.setFillColor(sf::Color::Black);
  std_title_text.setOrigin(0.f, std_title_text.getGlobalBounds().height / 2.f);
  std_title_text.setPosition(display_width - stat_rectangle_width * (19.f / 20.f),(display_height - stat_rectangle_height * (7.f / 30.f)));
  
      //testi parametri
  sf::Text sep_text;
  sep_text.setFont(font);
  sep_text.setCharacterSize(14);
  sep_text.setFillColor(sf::Color::Black);
  sep_text.setString(roundto(stormo.get_sep(), 2));
  sep_text.setOrigin(sep_text.getGlobalBounds().width / 2.f, sep_text.getGlobalBounds().height / 2.f);
  sep_text.setPosition(menu_rectangle_width * (13.f / 80.f), (display_height - menu_rectangle_height * (5.f / 11.f)));
  sf::Text ali_text;
  ali_text.setFont(font);
  ali_text.setCharacterSize(14);
  ali_text.setFillColor(sf::Color::Black);
  ali_text.setString(roundto(stormo.get_ali(), 2));
  ali_text.setOrigin(ali_text.getGlobalBounds().width / 2.f, ali_text.getGlobalBounds().height / 2.f);
  ali_text.setPosition(menu_rectangle_width * (33.f / 80.f), (display_height - menu_rectangle_height * (5.f / 11.f)));
  sf::Text coh_text;
  coh_text.setFont(font);
  coh_text.setCharacterSize(14);
  coh_text.setFillColor(sf::Color::Black);
  coh_text.setString(roundto(stormo.get_coe(), 2));
  coh_text.setOrigin(coh_text.getGlobalBounds().width / 2.f, coh_text.getGlobalBounds().height / 2.f);
  coh_text.setPosition(menu_rectangle_width * (53.f / 80.f), (display_height - menu_rectangle_height * (5.f / 11.f)));
  sf::Text vis_text;
  vis_text.setFont(font);
  vis_text.setCharacterSize(14);
  vis_text.setFillColor(sf::Color::Black);
  vis_text.setString(roundto(dist_mult, 2));
  vis_text.setOrigin(vis_text.getGlobalBounds().width / 2.f, vis_text.getGlobalBounds().height / 2.f);
  vis_text.setPosition(menu_rectangle_width * (73.f / 80.f), (display_height - menu_rectangle_height * (5.f / 11.f)));

      //testi dati statistici
  double mean_dis{stormo.mean_distance()};
  double std_dis{stormo.stnd_deviation_distance(mean_dis)};
  double mean_vel{stormo.mean_velocity()};
  double std_vel{stormo.stnd_deviation_velocity(mean_vel)};

  sf::Text mean_dis_text;
  mean_dis_text.setFont(font);
  mean_dis_text.setCharacterSize(14);
  mean_dis_text.setFillColor(sf::Color::Black);
  mean_dis_text.setString(roundto(mean_dis, 1));
  mean_dis_text.setOrigin(mean_dis_text.getGlobalBounds().width / 2.f, mean_dis_text.getGlobalBounds().height / 2.f);
  mean_dis_text.setPosition(display_width - stat_rectangle_width * (3.f / 5.f),(display_height - stat_rectangle_height * (17.f / 30.f)));
  sf::Text std_dis_text;
  std_dis_text.setFont(font);
  std_dis_text.setCharacterSize(14);
  std_dis_text.setFillColor(sf::Color::Black);
  std_dis_text.setString(roundto(std_dis, 1));
  std_dis_text.setOrigin(std_dis_text.getGlobalBounds().width / 2.f, std_dis_text.getGlobalBounds().height / 2.f);
  std_dis_text.setPosition(display_width - stat_rectangle_width * (3.f / 5.f),(display_height - stat_rectangle_height * (7.f / 30.f)));
  sf::Text mean_vel_text;
  mean_vel_text.setFont(font);
  mean_vel_text.setCharacterSize(14);
  mean_vel_text.setFillColor(sf::Color::Black);
  mean_vel_text.setString(roundto(mean_vel, 1));
  mean_vel_text.setOrigin(mean_vel_text.getGlobalBounds().width / 2.f, mean_vel_text.getGlobalBounds().height / 2.f);
  mean_vel_text.setPosition(display_width - stat_rectangle_width * (1.f / 5.f),(display_height - stat_rectangle_height * (17.f / 30.f)));
  sf::Text std_vel_text;
  std_vel_text.setFont(font);
  std_vel_text.setCharacterSize(14);
  std_vel_text.setFillColor(sf::Color::Black);
  std_vel_text.setString(roundto(std_vel, 1));
  std_vel_text.setOrigin(std_vel_text.getGlobalBounds().width / 2.f, std_vel_text.getGlobalBounds().height / 2.f);
  std_vel_text.setPosition(display_width - stat_rectangle_width * (1.f / 5.f),(display_height - stat_rectangle_height * (7.f / 30.f)));

  //inizializzazione  di altre variabili utili nel game loop
  int click_state = unclicked;
  sf::Clock sound_clock;
  sf::Clock statistics_clock;

  boom_positionx = 10000;//in realtà inizializzate in velocity.hpp
  boom_positiony = 10000;

  int animation_index{0};

  //GAME LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOP

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type ==
          sf::Event::Closed) {  // if che serve per chiudere la finestre quando
                                /// si preme sulla "x" in alto a destra
        window.close();
      }
      else if (event.type == sf::Event::MouseButtonReleased &&
          sound_clock.getElapsedTime().asSeconds() > 1.f && !(menu_rectangle.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,
                                        sf::Mouse::getPosition(window).y)) && !(stat_rectangle.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,
                                        sf::Mouse::getPosition(window).y))) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        boom_positionx = static_cast<double>(mousePosition.x);
        boom_positiony = static_cast<double>(mousePosition.y);
        boom_sound.play();
        sound_clock.restart();
        animation_index = 0;
      }
      else if (event.type == sf::Event::MouseButtonReleased){
        click_state = clicked;
      }
    }
    
    if (sound_clock.getElapsedTime().asSeconds() > 1.f && boom_positionx != 10000 && boom_positiony != 10000) {
      boom_positionx = 10000;
      boom_positiony = 10000;
    }

    rect_boom_sprite.left = animation_index * 56;
    boom_sprite.setTextureRect(rect_boom_sprite);
    boom_sprite.setPosition(boom_positionx, boom_positiony);//DA RIVEDERE 

    //aggiorno i bottoni
    for(auto& button : sep_buttons){
      button.update(stormo, window, click_state, dist_mult, sep_text, display_height, menu_rectangle_width, menu_rectangle_height);
    }
    for(auto& button : ali_buttons){
      button.update(stormo, window, click_state, dist_mult, ali_text, display_height, menu_rectangle_width, menu_rectangle_height);
    }
    for(auto& button : coh_buttons){
      button.update(stormo, window, click_state, dist_mult, coh_text, display_height, menu_rectangle_width, menu_rectangle_height);
    }
    for(auto& button : vis_buttons){
      button.update(stormo, window, click_state, dist_mult, vis_text, display_height, menu_rectangle_width, menu_rectangle_height);
    }

    //aggiorno il flock
    update(stormo, steps_per_evolution, dt, dist_mult);
    
    //aggiorno i dati statistici
    
    if (statistics_clock.getElapsedTime().asSeconds() > 3.f) {
      statistics_update(stormo, mean_dis, mean_dis_text, std_dis, std_dis_text, mean_vel, mean_vel_text, std_vel, std_vel_text, display_width, display_height, stat_rectangle_width, stat_rectangle_height);
      statistics_clock.restart();
    }

    window.clear();

    //drawo il background
    window.draw(background_sprite);

    //drawo il flock
    for (auto& boid : stormo.get_flock()) {
      double angle = orientation(boid.get_vx(), boid.get_vy());
      convex.setRotation(-angle);

      convex.setPosition(boid.get_x(), boid.get_y());

      window.draw(convex);  // disegna sull' oggetto window, ma non "displaya"
                            // ancora la window
    }

    //drawo il frame corrente dell'animazione dello scoppio
    window.draw(boom_sprite);

    //inizio a draware l'interfaccia
    window.draw(menu_rectangle);
    window.draw(stat_rectangle);

    //drawo i rettangoli relativi alle caselle di testo
        //parametri 
    window.draw(sep_text_rectangle);
    window.draw(ali_text_rectangle);
    window.draw(coh_text_rectangle);
    window.draw(vis_text_rectangle);
        //analisi statistica
    window.draw(mean_dis_text_rectangle);
    window.draw(std_dis_text_rectangle);
    window.draw(mean_vel_text_rectangle);
    window.draw(std_vel_text_rectangle);
    
    //drawo i testi
        //titoli parametri
    window.draw(sep_title_text);
    window.draw(ali_title_text);
    window.draw(coh_title_text);
    window.draw(vis_title_text);
        //titoli analisi statistica
    window.draw(boid_distance_title_text);
    window.draw(boid_vel_title_text);
    window.draw(mean_title_text);
    window.draw(std_title_text);
        //parametri
    window.draw(sep_text);
    window.draw(ali_text);
    window.draw(coh_text);
    window.draw(vis_text);
        //analisi statistica
    window.draw(mean_dis_text);
    window.draw(std_dis_text);
    window.draw(mean_vel_text);
    window.draw(std_vel_text);

    //drawo i bottoni
    for(auto& button : sep_buttons){
      button.draw(window);
    }
    for(auto& button : ali_buttons){
      button.draw(window);
    }
    for(auto& button : coh_buttons){
      button.draw(window);
    }
    for(auto& button : vis_buttons){
      button.draw(window);
    }

    window.display();  // adesso displaya
    
    click_state = unclicked;

    ++animation_index;
    if (animation_index == 8) {
      animation_index = 0;
    }
  
  fps_count.update();
	   std::ostringstream ss;
	   ss << fps_count.getFPS();
		
	   window.setTitle(ss.str());
  
  }
}

