#ifndef BOIDS
#define BOIDS
class Boid {
  double x_;  // posizioni nel piano
  double y_;
  double vx_;  // velocità nel piano
  double vy_;

 public:
  Boid(double x = 0, double y = 0, double vx = 0, double vy = 0)
      : x_{x}, y_{y}, vx_{vx}, vy_{vy} {}

  double get_x() const { return x_; }
  double get_y() const { return y_; }

  double get_vx() const { return vx_; }
  double get_vy() const { return vy_; }

  void set_x(double new_x) { x_ = new_x; }
  void set_y(double new_y) { y_ = new_y; }

  void set_vx(double new_vx) { vx_ = new_vx; }
  void set_vy(double new_vy) { vy_ = new_vy; }

  friend bool operator==(Boid& boid1, Boid& boid2) {
    return boid1.get_x() == boid2.get_x() && boid1.get_y() == boid2.get_y() &&
           boid1.get_vx() == boid2.get_vx() && boid1.get_vy() == boid2.get_vy();
  }
};

#endif