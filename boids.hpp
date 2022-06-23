#ifndef BOIDS
#define BOIDS
#include <cassert>
// Definizione della classe Boid, elemento strutturale del codice

class Boid
{
  // Posizioni e velocità nel piano
  // La definizione di una coppia di valori per posizione e velocità, cioè delle
  // loro proiezioni sulle x e y, anzi che l'impiego di un unico valore
  // vettoriale, rappresenta la nostra scela di lavorare sempre e solo in forma
  // scalare
  double x_;
  double y_;
  double vx_;
  double vy_;
  // Costruttore
 public:
  Boid(double x = 0, double y = 0, double vx = 0, double vy = 0)
      : x_{x}
      , y_{y}
      , vx_{vx}
      , vy_{vy}
  {}
  // Metodi get e set per i dati membro
  double get_x() const
  {
    return x_;
  }
  double get_y() const
  {
    return y_;
  }

  double get_vx() const
  {
    return vx_;
  }
  double get_vy() const
  {
    return vy_;
  }

  void set_x(double new_x)
  {
    x_ = new_x;
  }
  void set_y(double new_y)
  {
    y_ = new_y;
  }

  void set_vx(double new_vx)
  {
    vx_ = new_vx;
  }
  void set_vy(double new_vy)
  {
    vy_ = new_vy;
  }

  // operatore == che sarà usato in seguito per fare in modo che un boid non
  // subisca l'influenza di se stesso. Inoltre è stato usato l'operatore == e
  // non =! perchè mi azzarderei a dire che è più efficiente, infatti essendo
  // molto più frequente che i due boids non siano lo stesso, raramente deve
  // valutare tutte le condizioni.
  friend bool operator==(Boid const& boid1, Boid const& boid2)
  {
    return boid1.get_x() == boid2.get_x() && boid1.get_y() == boid2.get_y()
        && boid1.get_vx() == boid2.get_vx() && boid1.get_vy() == boid2.get_vy();
  }
};

#endif