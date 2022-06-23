#include "flock.hpp"
#include <execution>

// In questa TU sono definiti i metodi e le free-functions responsabili della
// gestione delle interazioni dei boids. In particolare i boids possono
// interagire: tra di loro, con i bordi e con eventuali esplosioni.

// Funzione che returna true se un boid è nel raggio dell'interazione repulsiva,
// rispetto ad un altro boid. La terminologia boid e fixed_boid diventa più
// chiara in seguito, nei metodi delle velocità.
bool in_rep_range(double range, Boid const& boid, Boid const& fixed_boid)
{
  assert(range > 0);
  return std::fabs(boid.get_x() - fixed_boid.get_x()) < range
      && std::fabs(boid.get_y() - fixed_boid.get_y()) < range
      && !(boid == fixed_boid);
}
// Funzione che returna true se un boid è nel raggio dell'interazione di
// coesione o di allineamento. Da notare che, tenendo i range come parametri si
// può decidere di far attivare le due diverse interazioni a range differenti.
// Inoltre si è deciso, per questioni sia di miglioramento del risultato
// grafico, che di buon senso, di impostare una distanza minima sotto la quali
// questo tipo di interazioni si spengono.
bool in_range(double max_range, double min_range, Boid const& boid,
              Boid const& fixed_boid)
{
  return (std::fabs(boid.get_x() - fixed_boid.get_x()) < max_range
          && std::fabs(boid.get_y() - fixed_boid.get_y()) < max_range)
      && !(boid == fixed_boid)
      && (std::fabs(boid.get_x() - fixed_boid.get_x()) > min_range
          || std::fabs(boid.get_y() - fixed_boid.get_y()) > min_range);
}

// Definisco la massima velocità repulsiva in valore assoluto
double const max_v_rep{6.};
// Definisco il corpo del metodo che calcola la velocità repulsiva lungo le X.
// La funzione è chiamata su di un singolo fixed_boid che le viene passato, in
// questo modo andrà a scorrere sul vettore dei Boid calcolando l'interazione
// con ognuno degli altri boids.
// E' stata anche cambiata la meccanica dell'interazione repulsiva, rispetto a
// quella proposta. Infatti ora l'intensità dipende dall'inverso della distanza.
// Il fattore moltiplicativo di 100. è presente solo al fine di rendere migliore
// il bilancio delle intensità tra le interazioni.
double Flock::vx_repulsive(double range, Boid const& fixed_boid) const
{
  double sum{0.};
  for (Boid const& boid : flock) {
    if (in_rep_range(range, boid, fixed_boid)) {
      assert(boid.get_x() - fixed_boid.get_x() != 0);
      sum += 100. / (boid.get_x() - fixed_boid.get_x());
    }
  }
  assert(sep_ >= 0 || sep_ <= 10);
  double const vx_rep{-sum * sep_};
  // Se il valore ottenuto è più basso di quello massimo, allora può essere
  // returnato...
  if (std::fabs(vx_rep) < max_v_rep) {
    return vx_rep;
  }
  // ...altrimenti calcola il verso della velocità e returna l'intensità massima
  double const sign{std::fabs(vx_rep) / vx_rep};
  // Da notare che anche la velocità massima è scalata dal coefficiente relativo
  // all'interazione
  return sep_ * max_v_rep * sign;
}
// la logica della stessa interazione, ma proiettata sulla y, è identica
double Flock::vy_repulsive(double range, Boid const& fixed_boid) const
{
  double sum{0.};
  for (Boid const& boid : flock) {
    if (in_rep_range(range, boid, fixed_boid)) {
      assert(boid.get_y() - fixed_boid.get_y() != 0);
      sum += 1. / (boid.get_y() / 100. - fixed_boid.get_y() / 100.);
    }
  }
  assert(sep_ >= 0 || sep_ <= 10);
  double const vy_rep{-sum * sep_};
  if (std::fabs(vy_rep) < max_v_rep) {
    return vy_rep;
  }
  double const sign{std::fabs(vy_rep) / vy_rep};
  return sep_ * max_v_rep * sign;
}

// Definisco la massima velocità di allineamento in valore assoluto
double const max_v_alig{1.2};
// Definisco il corpo della funzione che calcola la velocità di allineamento.
// La funzione per prima cosa conta quanti sono i boids nel range
// dell'interazione, e se non ce ne sono returna 0. Altrimenti calcola
// l'intensità dell'interazione subita dai boids nel range.
double Flock::vx_alignment(double max_range, double min_range,
                           Boid const& fixed_boid) const
{
  long int const n_boids{
      std::count_if(flock.begin(), flock.end(), [&](Boid const& boid) {
        return in_range(max_range, min_range, boid, fixed_boid);
      })};
  if (n_boids == 0) {
    return 0.;
  }
  assert(n_boids > 0);
  double sum{0.};
  for (Boid const& boid : flock) {
    if (in_range(max_range, min_range, boid, fixed_boid)) {
      sum += boid.get_vx();
    }
  }
  assert(al_ >= 0 || al_ <= 10);
  double const vx_align{al_ * ((sum / n_boids) - fixed_boid.get_vx())};
  // La funzione gestisce allo stesso modo della precedente il caso in cui
  // l'intensità della velocità calcolata superi il limite imposto.
  if (std::fabs(vx_align) < max_v_alig) {
    return vx_align;
  }
  double const sign{std::fabs(vx_align) / vx_align};
  return al_ * max_v_alig * sign;
}
// Anche in questo caso, come lo sarà per tutte le funzioni, la gemella sulle y
// è identica
double Flock::vy_alignment(double max_range, double min_range,
                           Boid const& fixed_boid) const
{
  long int const n_boids{
      std::count_if(flock.begin(), flock.end(), [&](Boid const& boid) {
        return in_range(max_range, min_range, boid, fixed_boid);
      })};
  if (n_boids == 0) {
    return 0.;
  }
  assert(n_boids > 0);
  double sum{0.};
  for (Boid const& boid : flock) {
    if (in_range(max_range, min_range, boid, fixed_boid)) {
      sum += boid.get_vy();
    }
  }
  assert(al_ >= 0 || al_ <= 10);
  double const vy_align{al_ * ((sum / n_boids) - fixed_boid.get_vy())};

  if (std::fabs(vy_align) < max_v_alig) {
    return vy_align;
  }
  double const sign{std::fabs(vy_align) / vy_align};
  return al_ * max_v_alig * sign;
}

// Definisco la massima velocità di coesione in valore assoluto
double const max_v_cohe{2.};
// La funzione che calcola la velocità di coesione è pressochè identica in
// logica alla funzione che calcola la velocità dell'interazione di
// allineamento. Le uniche differenze sono nel calcolo algebrico.
double Flock::vx_cohesion(double max_range, double min_range,
                          Boid const& fixed_boid) const
{
  long int const n_boids{
      std::count_if(flock.begin(), flock.end(), [&](Boid const& boid) {
        return in_range(max_range, min_range, boid, fixed_boid);
      })};
  if (n_boids == 0) {
    return 0.;
  }
  assert(n_boids > 0);
  double sum{0.};
  for (Boid const& boid : flock) {
    if (in_range(max_range, min_range, boid, fixed_boid)) {
      sum += boid.get_x();
    }
  }
  // Il fattore moltiplicativo di 1/3 è stato inserito per migliorare le
  // proprorzioni tra le intensità delle interazioni.
  assert(cohe_ >= 0 || cohe_ <= 10);
  double const vx_cohe{cohe_ * (sum / n_boids - fixed_boid.get_x()) / 3.};
  if (std::fabs(vx_cohe) < max_v_cohe) {
    return vx_cohe;
  }
  double const sign{std::fabs(vx_cohe) / vx_cohe};
  return cohe_ * max_v_cohe * sign;
}
// Funzione gemella per le y
double Flock::vy_cohesion(double max_range, double min_range,
                          Boid const& fixed_boid) const
{
  long int const n_boids{
      std::count_if(flock.begin(), flock.end(), [&](Boid const& boid) {
        return in_range(max_range, min_range, boid, fixed_boid);
      })};
  if (n_boids == 0) {
    return 0.;
  }
  assert(n_boids > 0);
  double sum{0.};
  for (Boid const& boid : flock) {
    if (in_range(max_range, min_range, boid, fixed_boid)) {
      sum += boid.get_y();
    }
  }
  assert(cohe_ >= 0 || cohe_ <= 10);
  double const vy_cohe{cohe_ * (sum / n_boids - fixed_boid.get_y()) / 3.};
  if (std::fabs(vy_cohe) < max_v_cohe) {
    return vy_cohe;
  }
  double const sign{std::fabs(vy_cohe) / vy_cohe};
  return cohe_ * max_v_cohe * sign;
}
// NOTA FINALE
// Inizialmente questi metodi implementavano l'uso dell'algoritmo accumulate,
// tuttavia è stato deciso di sostituirlo con dei cicli for. Questa decisione è
// stata presa in per migliorare le performance del programma. Infatti ognuno di
// questi metodi è chiamato 8*25*flock.size() volte al secondo, ed ogni volta
// accumulate avrebbe dovuto creare la lambda che utilizza come operatore
// binario, risultando in un calo di prestazioni (anche se molto lieve).
// L'ideale sarebbe stato definire la lambda fuori dal corpo della funzione,
// tuttavia trattandosi di un operatore binario non risulta possibile fargli
// avere, in modo non troppo artificioso, i valori che altrimenti può catturare
// dallo scope della funzione.

// D'ora in poi non vengono più definiti metodi, ma free-function.
// L'insime di free-function che vengono definite da qui in poi si occupano
// delle interazioni dei boids con agenti esterni allo stormo, ovvero i bordi e
// le esplosioni.

// Definisco due variabili che contengono il valore del centro del rettangolo in
// cui si trovano i boids. Queste serviranno poco dopo per il calcolo della
// velocità di rientro.
double const center_x{(bound_xmax + bound_xmin) / 2.};
double const center_y{(bound_ymax + bound_ymin) / 2.};

// Funzione che returna true se i boids escono dal range x del rettangolo
bool not_in_perimeter_x(Boid const& boid)
{
  return boid.get_x() < bound_xmin || boid.get_x() > bound_xmax;
}

// Ora inizia la parte di logica dietro alla velocità che fa rietrare i boids
// nel perimetro del rettangolo.
// Per prima cosa definisco un secondo limite, un limite esterno se vogliamo.
// La differenza tra i due limiti è visualizzabile come "quello interno è
// oltrepassabile, ma rappresenza una zona di spazio speciale", mentre "quello
// esterno è un muro pressochè invalicabile". La loro esistenza e distinzione
// sarà resa più chiara dalla prossima funzione.
double const margin{100.};
// La funzione perimeter_velocityx_active returna true se sul boid deve essere
// applicata una velocità di rientro(ATTENZIONE, QUESTA VELOCITA' E' DIVERSA DA
// QUELLA CHE FRENA IL BOID FUORI DAL RETTANGOLO). Ma quando deve essere
// applicata? Deve essere attivata in due casi: il boid ha sorpassato le colonne
// d'Ercole, ovvero il secondo limite del rettangolo, oppure quando il boid si
// trova nell'area compresa tra il primo limite ed il secondo, e
// contemporanemente, è direzionato verso il centro. Questo secondo caso è
// responsabile di mantenere lo stormo vivo e reattivo, facendolo accelerare
// verso il centro qualora provasse ad evadere dal rettangolo. La presenza di
// primo e secondo limite garantisce così una specie di pista di accelerazione
// per i boid, senza però rischiare che vengano accelerati verso l'esterno.
// Chiaramente esiste la gemella sulle y di questa funzione.
bool p_velx_active(Boid const& boid)
{
  if (boid.get_x() < bound_xmin) {
    bool const is_center_oriented{boid.get_vx() > 0.};
    return boid.get_x() < (bound_xmin - margin) || is_center_oriented;
  } else if (boid.get_x() > bound_xmax) {
    bool const is_center_oriented{boid.get_vx() < 0.};
    return (boid.get_x() > bound_xmax + margin) || is_center_oriented;
  }
  return false;
}
// La funzione perimeter_slowdown_active_x returna true se il boid deve essere
// rallentato, cioè se si trova fuori dai bordi esterni del rettangolo ed è
// orientato verso l'esterno.
bool p_slowdown_active_x(Boid const& boid)
{
  if (boid.get_x() < bound_xmin - margin) {
    bool const is_exterior_oriented{boid.get_vx() < 0.};
    return is_exterior_oriented;
  } else if (boid.get_x() > bound_xmax + margin) {
    bool const is_exterior_oriented{boid.get_vx() > 0.};
    return is_exterior_oriented;
  }
  assert(boid.get_x() > bound_xmin - margin
         && boid.get_x() < bound_xmax + margin);
  return false;
}
// esattamente le stesse funzioni, ma applicate sulle y.
bool not_in_perimeter_y(Boid const& boid)
{
  return boid.get_y() < bound_ymin || boid.get_y() > bound_ymax;
}

bool p_vely_active(Boid const& boid)
{
  if (boid.get_y() < bound_ymin) {
    bool const is_center_oriented{boid.get_vy() > 0.};
    return boid.get_y() < (bound_ymin - margin)
        || (boid.get_y() > bound_ymax + margin) || is_center_oriented;
  } else if (boid.get_y() > bound_ymax) {
    bool const is_center_oriented{boid.get_vy() < 0.};
    return boid.get_y() < (bound_ymin - margin)
        || (boid.get_y() > bound_ymax + margin) || is_center_oriented;
  }
  return false;
}

bool p_slowdown_active_y(Boid const& boid)
{
  if (boid.get_y() < bound_ymin - margin) {
    bool const is_exterior_oriented{boid.get_vy() < 0.};
    return is_exterior_oriented;
  } else if (boid.get_y() > bound_ymax + margin) {
    bool const is_exterior_oriented{boid.get_vy() > 0.};
    return is_exterior_oriented;
  }
  return false;
}

// Definisco la funzione returna la velocità di rientro del boid, questa è
// direttamente proporzionale alla distanza ed è regolata da una distanza
// moltiplicativa.
double v_perimeterx(double m, Boid const& boid)
{
  assert(m > 0);
  return m * (center_x - boid.get_x());
}

double v_perimetery(double m, Boid const& boid)
{
  assert(m > 0);
  return m * (center_y - boid.get_y());
}

// Ora vengono definite le funzioni che gestiscono l'interazione dei boids con
// le esplosioni.

// La funzione in_explosion_range determina se un boid è nel range per subire
// l'effetto di spavento causato dallo scoppio.
bool in_explosion_range(double expl_centerx, double expl_centery,
                        double expl_range, Boid const& boid)
{
  assert(expl_range > 0);
  return (std::fabs(boid.get_x() - expl_centerx) < expl_range
          && std::fabs(boid.get_y() - expl_centery) < expl_range);
}
// Definisco la massima velocità applicabile al boid a causa dello spavento
double const max_expl_vel{30.};
// Definisco la funzione che calcola la velocità con cui scappa il boid
// spaventato. La sua logica è simile alla funzione per la velocità repulsiva,
// ma in questo caso saranno applicati coefficienti moltiplicativi maggiori.
double expl_velocity_x(double b, double expl_centerx, Boid const& boid)
{
  assert(boid.get_x() - expl_centerx != 0);
  double const expl_velx{300. * b / (boid.get_x() - expl_centerx)};
  if (std::fabs(expl_velx) < max_expl_vel)
    return expl_velx;
  double const sign{std::fabs(expl_velx) / expl_velx};
  return max_expl_vel * sign;
}
// Equivalente sulle y.
double expl_velocity_y(double b, double expl_centery, Boid const& boid)
{
  assert(boid.get_y() - expl_centery != 0);
  double const expl_vely{b / (boid.get_y() / 300. - expl_centery / 300.)};
  if (std::fabs(expl_vely) < max_expl_vel)
    return expl_vely;
  double const sign{std::fabs(expl_vely) / expl_vely};
  return max_expl_vel * sign;
}
