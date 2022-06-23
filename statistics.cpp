#include "flock.hpp"

// In questa TU vado a definire i metodi statistici dichiarati nella classe
// Flock. Alcune accortezze generali sono state: evitare l'utilizzo della
// funzione pow() per questioni di performance e evitare il nesting di algoritmi
// accumulate per non rendere il codice inguardabile.

// Funzione che calcola la distanza media di tutti i boids. La distanza media
// golbale è stata considerata come la media delle distanze medie dei singoli
// fixed_boid dagli altri boids.
double Flock::mean_distance() const
{
  
  double total_sum{0.};
  // Per ogni boid calcolo la distanza media e la sommo
  for (Boid const& fixed_boid : flock) {
    double const single_sum{std::accumulate(
        flock.begin(), flock.end(), 0., [&](double sum, Boid const& boid) {
          return sum
               // Come distanza è stata usata quella definita rispetto alla
               // norma euclidea
               + std::sqrt((fixed_boid.get_x() - boid.get_x())
                               * (fixed_boid.get_x() - boid.get_x())
                           + (fixed_boid.get_y() - boid.get_y())
                                 * (fixed_boid.get_y() - boid.get_y()));
        })};
    assert(single_sum >= 0);
    assert(flock.size() != 0 && flock.size() != 1);
    total_sum += single_sum / (flock.size() - 1);
    assert(total_sum >= 0);
    // Dalla somma totale faccio la media delle distanze medie
  }
  return total_sum / flock.size();
  // Da notare come la media globlale sia una somma di flock.size() termini,
  // mentre quella del singolo boid chiaramente di flock.size()-1
}

// Definisco il corpo del metodo che calcola l'intensità media dei moduli delle
// velocità
double Flock::mean_velocity() const
{
  if (flock.size() == 0)
    return 0.;
  double const sum_{std::accumulate(
      flock.begin(), flock.end(), 0., [&](double sum, Boid const& boid) {
        return sum
             + std::sqrt(boid.get_vx() * boid.get_vx()
                         + boid.get_vy() * boid.get_vy());
      })};
  assert(flock.size() != 0);
  assert(sum_ >= 0);

  return sum_ / flock.size();
}

// Definisco il corpo del metodo che calcola la deviazione standard della
// distanza media
double Flock::stnd_deviation_distance(double mean_distance) const
{
  if (flock.size() == 1 || flock.size() == 0)
    return 0;
  double total_sum{0.};
  for (Boid const& fixed_boid : flock) {
    double const single_sum{std::accumulate(
        flock.begin(), flock.end(), 0., [&](double sum, Boid const& boid) {
          return sum
               + std::sqrt((fixed_boid.get_x() - boid.get_x())
                               * (fixed_boid.get_x() - boid.get_x())
                           + (fixed_boid.get_y() - boid.get_y())
                                 * (fixed_boid.get_y() - boid.get_y()));
        })};
    assert(single_sum >= 0);
    assert(flock.size() != 0 && flock.size() != 1);
    total_sum += (single_sum / (flock.size() - 1) - mean_distance)
               * (single_sum / (flock.size() - 1) - mean_distance);
    assert(total_sum >= 0);
  }
  return std::sqrt(total_sum / (flock.size() - 1));
}

// Definisco il corpo della deviazione standard della media dei moduli delle
// velocità
double Flock::stnd_deviation_velocity(double mean_velocity) const
{
  if (flock.size() == 1 || flock.size() == 0)
    return 0;
  double const sum_{std::accumulate(
      flock.begin(), flock.end(), 0., [&](double sum, Boid const& boid) {
        return sum
             + (std::sqrt(boid.get_vx() * boid.get_vx()
                          + boid.get_vy() * boid.get_vy())
                - mean_velocity)
                   * (std::sqrt(boid.get_vx() * boid.get_vx()
                                + boid.get_vy() * boid.get_vy())
                      - mean_velocity);
      })};
  assert(sum_ >= 0);
  assert(flock.size() != 0 && flock.size() != 1);
  return std::sqrt(sum_ / (flock.size() - 1));
}
