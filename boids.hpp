class Boid {
  double x_;  // posizioni nel piano
  double y_;
  double vx_;  // velocità nel piano
  double vy_;

 public:
  Boid(double x = 0, double y = 0, double vx = 0, double vy = 0)
      : x_{x}, y_{y}, vx_{vx}, vy_{vy} {}

  void update(double dt, double vx_e, double vy_e) {
    x_ += vx_ * dt;
    y_ += vy_ * dt;
    vx_ += vx_e;      //vx_e sta per velocità x esterna
    vy_ += vy_e;
  }
  double get_x(){
    return x_;
  }
  double get_y(){
    return y_;
  }

  double get_vx(){
    return vx_;
  }
  double get_vy(){
    return vy_;
  }

  friend bool operator==(Boid boid1, Boid boid2){
    return boid1.get_x()==boid2.get_x() && boid1.get_y()==boid2.get_y() && boid1.get_vx()==boid2.get_vx() && boid1.get_vy()==boid2.get_vy();
  }
};
