#include <execution>

#include "flock.hpp"
#include "velocity.hpp"
// In questa TU vengono definite le due free-function che gestiscono
// l'evoluzione temporale dello stormo. La prima in particolare è la funzione
// più importante, e pesante, di tutto il codice: Update.

// Per prima cosa viene definita la distanza a cui i boids interagiscono per
// allineamento e coesione (vision) e la distanza a cui si repellono.
double const vision{200.};
double const separation{30.};
// La funzione update prende come parametri: lo stormo che si vuole far
// evolvere, in numero di step per chiamata e la durata temporale di ogni step.
// Inoltre prende anche un coefficiente moltiplicativo che scala la distanza a
// cui i boids interagiscono per coesione e allineamento.
void update(Flock& stormo, int steps_per_evolution, double delta_t,
            double dist_mult) {
  // La funzione fa un iterazione per ogni step_per_evolution
  for (int s{0}; s != steps_per_evolution; ++s) {
    // Viene fissato il vettore di Boid dello stormo su un vettore ausiliario
    auto flock{stormo.get_flock()};
    assert(stormo.get_flock().size() != 0);
    // Si trasforma il vettore tramite l'algoritmo transform
    // Nota, se il gcc in uso lo consente, è possibile (e così è
    // stato concepito il codice) passare come primo parametro la execution
    // policy: "std::execution::par_unseq". Questo renderà possibile l'uso del
    // multithreading da parte di transform. (Peggiori performce a bassi numeri
    // di boids, ma rende girabile il codice anche numeri elevati di boids).
    std::transform(flock.begin(), flock.end(), flock.begin(), [&](Boid& boid) {
      // L'operatore unario che viene applicato è un lambda che, per ogni
      // boid su cui transform si focalizza, applica una serie di
      // trasformazioni alla sua posizione e velocità.
      // Per prima cosa aggiorna la posizione in base all'attuale velocità.
      double new_x{boid.get_x() + boid.get_vx() * delta_t};
      double new_y{boid.get_y() + boid.get_vy() * delta_t};
      // Il resto della lambda è dedicata alla modifica della velocità.
      // Viene definita una velocità v_esterna, che rappresenta la
      // somma delle velocità di interazione con gli altri boids nello stormo.
      // I range di repulsione, allineamento e coesione sono tutti definiti come
      // proprozioni di "vision" e "separation".
      double const vx_e{stormo.vx_repulsive(separation, boid) +
                        stormo.vx_alignment(vision * dist_mult,
                                            vision * dist_mult / 20., boid) +
                        stormo.vx_cohesion(vision * dist_mult,
                                           vision * dist_mult / 80., boid)};
      double const vy_e{stormo.vy_repulsive(separation, boid) +
                        stormo.vy_alignment(vision * dist_mult,
                                            vision * dist_mult / 20., boid) +
                        stormo.vy_cohesion(vision * dist_mult,
                                           vision * dist_mult / 80., boid)};
      // Viene definita una nuova variabile new_v che rappresenta la nuova
      // velocità del boid, ottenuta sommando la velocità precedente e
      // v_esterna. Il coefficiente moltiplicativo di 1/8 è stato inserito per
      // moderare l'incremento della velocità. (Da notare infatti che questa
      // simulazione è slegata dalla presenza di una velocità massima per i
      // boid, tale limite avrebbe reso molto più ostico il bilancio delle
      // interazioni, che avrebbero rischiato di escludersi a vicenda).
      double new_vx = boid.get_vx() + vx_e / 8.;
      double new_vy = boid.get_vy() + vy_e / 8.;

      // Ora controlla se devono essere attivati una serie di modifiche
      // particolari sulla velocità del boid.

      // Per prima cosa applica la veloctià di slowdown se si trova nelle
      // condizioni di essere applicata (vedi la definizione di
      // p_slowdown_active in velocity.cpp). La velocità di slowdown in realtà
      // non è altro che una riduzione di un fattore moltiplicatico di 1/1.015
      // sulla nuova velocità del boid. Sembra poco, ma dato che verrà applicato
      // 200 volte al secondo, risulta effettiva. Il grande vantaggio di questa
      // soluzione sta nella poca influenza che ha la velocità del boid, quando
      // oltrepassa i limiti esterni, sul tempo che ci impiegherà ad essere
      // rallentato. Questo produrrà un effetto molto fluido indipendentemente
      // dallo stato iniziale del boid.
      if (p_slowdown_active_x(boid)) {
        new_vx = new_vx / 1.015;
      }

      if (p_slowdown_active_y(boid)) {
        new_vy = new_vy / 1.015;
      }

      // Se il boid si trova in condizioni di essere accelerato verso il centro
      // (vedi la definizione di p_velx_active) subisce una velocità aggiuntiva
      // calcolata da v_perimeter.
      if (not_in_perimeter_x(boid) && p_velx_active(boid)) {
        new_vx += v_perimeterx(0.015, boid);
      }

      if (not_in_perimeter_y(boid) && p_vely_active(boid)) {
        new_vy += v_perimetery(0.015, boid);
      }
      // Se il boid si trova nel range per essere spaventato dallo scoppio,
      // allora è capace di fuggire spaventato, grazie ad una velocità
      // aggiuntiva calcolata da expl_velocity
      if (in_explosion_range(boom_positionx, boom_positiony, 100., boid)) {
        new_vx += expl_velocity_x(10., boom_positionx, boid);
        new_vy += expl_velocity_y(10., boom_positiony, boid);
      }
      // Infine setta le nuove caratteristiche del boid
      boid.set_x(new_x);
      boid.set_y(new_y);
      boid.set_vx(new_vx);
      boid.set_vy(new_vy);
      return boid;
    });
    // Quando transform ha finito di operare, setta come nuovo vettore di Boid
    // dello stormo, quello modificato da transform.
    stormo.set_flock(flock);
  }
}

// Funzione che calcola l'angolo (in gradi) di
// orientamento del singolo boid rispetto all'asse x
double orientation(double vx, double vy) {
  if (vx >= 0.) {
    return atan(-vy / vx) * (180 / M_PI) - 90;
  }
  return (atan(-vy / vx) + M_PI) * (180 / M_PI) - 90;
}

// Segue una parte di codice non necessario ed anzi, alternativo.
// La ricerca per il raggiungimento di migliori performance, ha portato alla
// tentata implementazione del multithreading. Il codice commentato che segue, è
// una versione alternativa di Update che sfrutta il multithreading al posto di
// transform. Inutile dire che le sue performance sono inferiori, un po' per le
// mancate condizioni per la necessità del multithreading, un po' per il
// tentativo, abbastanza scarso, di utilizzare qualcosa di forse troppo
// avanzato. In realtà ci si rende conto che il numero di threads creati è
// ridicolmente alto ed inefficiente, infatti l'idea iniziale era diversa, ma
// l'implementazione è risultata eccessivamente problematica. La reale
// intenzione era quella di creare un numero ragionevole di threads e di
// spezzare il vettore di Boid nello stesso numero di range. In questo modo
// sarebbe stato possibile assegnare in modo efficiente, l'aggiornamento di una
// porzione di vettore al singolo thread.
// Lasciamo comunque il codice in quanto questa tipologia di soluzione è stata
// molto interessante.

// #include <mutex>
// #include <thread>

// auto lambda = [](Boid& boid, Flock& stormo, std::vector<Boid>& n_flock,
//                  double delta_t, std::mutex& mu, double dist_mult) {
//   mu.lock();
//   double new_x = boid.get_x() + boid.get_vx() * delta_t;
//   double new_y = boid.get_y() + boid.get_vy() * delta_t;
//   double vx_e =
//       stormo.vx_repulsive(separation, boid) +
//       stormo.vx_alignment(vision * dist_mult, vision * dist_mult / 20., boid)
//       + stormo.vx_coesion(vision * dist_mult, vision * dist_mult / 80.,
//       boid);
//   double vy_e =
//       stormo.vy_repulsive(separation, boid) +
//       stormo.vy_alignment(vision * dist_mult, vision * dist_mult / 20., boid)
//       + stormo.vy_coesion(vision * dist_mult, vision * dist_mult / 80.,
//       boid);
//   double new_vx = boid.get_vx() + vx_e / 8.;
//   double new_vy = boid.get_vy() + vy_e / 8.;

//   if (p_slowdown_active_x(boid)) {
//     new_vx = new_vx / 1.01;
//   }

//   if (p_slowdown_active_y(boid)) {
//     new_vy = new_vy / 1.01;
//   }

//   if (not_in_perimeter_x(boid) && p_velx_active(boid)) {
//     new_vx += v_perimeterx(0.015, boid);
//   }

//   if (not_in_perimeter_y(boid) && p_vely_active(boid)) {
//     new_vy += v_perimetery(0.015, boid);
//   }

//   if (in_explosion_range(boom_positionx, boom_positiony, 100., boid)) {
//     new_vx += expl_velocity_x(10., boom_positionx, boid);
//     new_vy += expl_velocity_y(10., boom_positiony, boid);
//   }

//   boid.set_x(new_x);
//   boid.set_y(new_y);
//   boid.set_vx(new_vx);
//   boid.set_vy(new_vy);
//   n_flock.push_back(boid);
//   mu.unlock();
// };

// void update(Flock& stormo, int steps_per_evolution, double delta_t,
//             double dist_mult) {
//   for (int s{0}; s != steps_per_evolution; ++s) {
//     auto flock{stormo.get_flock()};
//     std::vector<Boid> n_flock;
//     std::vector<std::thread> t_vector;
//     std::mutex mu;

//     for (Boid& boid : flock) {
//       t_vector.push_back(std::thread(lambda, std::ref(boid),
//       std::ref(stormo),
//                                      std::ref(n_flock), delta_t,
//                                      std::ref(mu), dist_mult));
//     }

//     for (auto& th : t_vector) {
//       th.join();
//     }

//     stormo.set_flock(n_flock);
//   }
// }