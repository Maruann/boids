#include "graphics.hpp"
#include "simulation.hpp"
#include <iostream>
// Prima del main sono definite due free-function che gestiscono l'input da
// terminale delle dimensioni dello stormo.

// La funzione is_integer controlla se nella stringa che le passo è
// rappresentato un numero intero. Più precisamente returna true se e solo se la
// stringa è formata da una serie ininterrotta di cifre. I casi in cui la serie
// è anticipata o posticipata da un solo spazio, sono accettati.
bool is_integer(std::string const& str)
{
  if (str.empty())
    return false;

  assert(!str.empty());
  int const lenght{static_cast<int>(str.length())};
  assert(lenght > 0);
  for (int i{0}; i < lenght; i++) {
    if (i == 0 || i == (lenght - 1)) {
      if (std::isdigit(str[i]) == false && std::isspace(str[i]) == false
          && str[i] != *"-" && str[i] != *"+") {
        return false;
      }
    } else {
      if (std::isdigit(str[i]) == false || std::isspace(str[i]) == true) {
        return false;
      }
    }
  }
  return true;
}

// Funzione ricorsiva che gestisce l'input da terminale della size dello stormo.
// La funzione legge da terminale una riga e determina, tramite la precedente
// funzione, se è accettabile o meno. Se lo è converte la stringa in intero e lo
// returna. Se la stringa non è valida (contiene caratteri che non sono cifre o
// la cifra che se ne deduce è 0), allora lancia un'eccezione. L'eccezione viene
// catchata sempre all'interno del corpo della funzione e stampa a schermo la
// causa dell'errore. Infine, se la lettura non è andata a buon fine, chiama se
// stessa e ricomincia.
int input_reader()
{
  std::string input;
  try {
    std::cout << "Inserire il numero di Boid da generare:" << '\n';
    std::getline(std::cin, input);
    if (is_integer(input)) {
      int const value{std::stoi(input)};
      if (value > 0) {
        std::cout << "CARICAMENTO..." << '\n';
        return value;
      } else if (value == 0)
        throw std::runtime_error("Il valore inserito è 0!");
      else
        throw std::runtime_error("Il valore inserito è minore di 0!");
    } else {
      throw std::runtime_error("Il valore inserito non è un intero!");
    }
  } catch (std::runtime_error const& e) {
    std::cout << "Errore: " << e.what() << '\n';
    return input_reader();
  }
}

int main()
{
  Flock stormo{5., 5., 5.};
  int const n{input_reader()};
  assert(n > 0);
  stormo.fill(n);
  assert(!stormo.get_flock().empty());
  graphics(stormo);
}