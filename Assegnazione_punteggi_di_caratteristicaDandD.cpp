#define NOMINMAX
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>
#include <string>
#include <span>
#include <format>
#include <cmath>
#include <array>
#include <random>
// Include Windows.h solo se compiliamo su Windows
#ifdef _WIN32
#include <Windows.h> //Necessaria per cambiare la codifica del terminale
#endif

#include <fstream>

using namespace std;

// Dichiarazione funzioni
int controllaScelta();
char controllaConferma();
void sceltaCaratteristica(string caratteristica, span<int> vettore, int &CARATTERISTICA);
void leggitotali(span<int> vettore);
int modificatore(int CARATTERISTICA);

int main()
{
#ifdef _WIN32
   // Forza il terminale a usare la codifica UTF-8
   SetConsoleOutputCP(CP_UTF8);
   SetConsoleCP(CP_UTF8);
#endif
   // Setup del generatore di numeri casuali (C++ moderno)
   random_device rd;
   mt19937 gen(rd());
   uniform_int_distribution<> d6(1, 6); // Configura un dado da 6 facce

   array<int, 4> tiri;
   array<int, 6> totali;
   int FORZA = 0, DESTREZZA = 0, COSTITUZIONE = 0, INTELLIGENZA = 0, SAGGEZZA = 0, CARISMA = 0; // Variabili

   cout << "Questo programma determina i punteggi di caratteristica di un personaggio utilizzando il metodo del lancio del d6.\n\n";
   for (auto j = 0; j < 6; j++)
   {
      int tot = 0;
      // Uso di format per creare stringhe chiare e pulite
      cout << format("Risultati dei tiri del d6, {}ª serie: ", j + 1);

      for (auto &i : tiri)
      {
         i = d6(gen); // Tira il d6
         cout << " " << i;
      }
      // C++20 Ranges: ordina l'array in modo molto semplice
      ranges::sort(tiri, greater());

      cout << "\nQuesti sono i 3 risultati più alti: ";
      for (auto i = 0; i < 3; i++)
      {
         cout << " " << tiri[i];
         tot += tiri[i];
      }
      cout << "\n\n";
      totali[j] = tot;
   }
   cout << "Somme dei 3 risultati più alti per ogni sequenza di tiri:  ";
   sceltaCaratteristica("FORZA", totali, FORZA);
   sceltaCaratteristica("DESTREZZA", totali, DESTREZZA);
   sceltaCaratteristica("COSTITUZIONE", totali, COSTITUZIONE);
   sceltaCaratteristica("INTELLIGENZA", totali, INTELLIGENZA);
   sceltaCaratteristica("SAGGEZZA", totali, SAGGEZZA);
   sceltaCaratteristica("CARISMA", totali, CARISMA);

   cout << "Punteggi di caratteristica definitivi:" << endl;
   cout << format("FORZA: {} (modificatore: {:+}).\n", FORZA, modificatore(FORZA));
   cout << format("DESTREZZA: {} (modificatore: {:+}).\n", DESTREZZA, modificatore(DESTREZZA));
   cout << format("COSTITUZIONE: {} (modificatore: {:+}).\n", COSTITUZIONE, modificatore(COSTITUZIONE));
   cout << format("INTELLIGENZA: {} (modificatore: {:+}).\n", INTELLIGENZA, modificatore(INTELLIGENZA));
   cout << format("SAGGEZZA: {} (modificatore: {:+}).\n", SAGGEZZA, modificatore(SAGGEZZA));
   cout << format("CARISMA: {} (modificatore: {:+}).\n", CARISMA, modificatore(CARISMA)) << endl;

   // Creazione file di testo con i risultati
   cout << "Salvare i risultati in un file di testo? ('s'='sì'): ";
   char salvaFile = controllaConferma();
   if (salvaFile == 's' || salvaFile == 'S')
   {
      cout << "Inserisci il nome del file:\n";
      string nomeFile;
      cin >> nomeFile;
      ofstream file(nomeFile + ".txt");
      file << "Punteggi di caratteristica definitivi:\n";
      file << format("FORZA: {} (modificatore: {:+}).\n", FORZA, modificatore(FORZA));
      file << format("DESTREZZA: {} (modificatore: {:+}).\n", DESTREZZA, modificatore(DESTREZZA));
      file << format("COSTITUZIONE: {} (modificatore: {:+}).\n", COSTITUZIONE, modificatore(COSTITUZIONE));
      file << format("INTELLIGENZA: {} (modificatore: {:+}).\n", INTELLIGENZA, modificatore(INTELLIGENZA));
      file << format("SAGGEZZA: {} (modificatore: {:+}).\n", SAGGEZZA, modificatore(SAGGEZZA));
      file << format("CARISMA: {} (modificatore: {:+}).\n", CARISMA, modificatore(CARISMA));
      file.close();
      cout << format("Risultati salvati in {}.txt, nella cartella del progetto.", nomeFile);
   }

   cin.ignore(numeric_limits<streamsize>::max(), '\n');
   cout << "\nPremere Invio per uscire...";
   cin.get();
   return 0;
}

// Definizione funzioni
int controllaScelta()
{
   float valore = 0.0;
   while (!(cin >> valore) || valore != static_cast<int>(valore))
   {
      cout << "Errore. Valore non riconosciuto. Riprovare: ";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
   }
   cin.ignore(numeric_limits<streamsize>::max(), '\n');
   return static_cast<int>(valore);
}

char controllaConferma()
{
   char valore = ' ';
   while (!(cin >> valore))
   {
      cout << "Errore. Carattere non riconosciuto. Riprovare: ";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
   }
   cin.ignore(numeric_limits<streamsize>::max(), '\n');
   return valore;
}

void sceltaCaratteristica(const string caratteristica, span<int> vettore, int &CARATTERISTICA)
{
   int scelta = 0;
   char conferma = ' ';
   do
   {
      leggitotali(vettore);
      cout << format("\n\nSpecificare la posizione (1-6) del punteggio da assegnare alla caratteristica {}: ", caratteristica);
      scelta = controllaScelta();
      int punteggioScelto = vettore[scelta - 1];
      while (scelta < 1 || scelta > 6 || punteggioScelto == -1)
      {
         cout << "Errore (-1). Valore non adeguato o già scelto. Riprovare: ";
         scelta = controllaScelta();
      }
      cout << format("Hai scelto la posizione {}, che corrisponde al punteggio {}.\n", scelta, punteggioScelto);
      cout << format("Il punteggio della caratteristica {} sarà: {}. Il suo modificatore sarà: {:+}.\n\n", caratteristica, punteggioScelto, modificatore(punteggioScelto));
      cout << "Confermi la scelta? (s/n): ";
      conferma = controllaConferma();
      while (conferma != 's' && conferma != 'S' && conferma != 'n' && conferma != 'N')
      {
         cout << "Carattere non adeguato. Inserire 's' per 'sì' o 'n' per 'no': ";
         conferma = controllaConferma();
      }
   } while (conferma == 'n' || conferma == 'N');
   CARATTERISTICA = vettore[scelta - 1];
   vettore[scelta - 1] = -1; // Rende il punteggio scelto non più disponibile
   cout << format("Il punteggio scelto per la caratteristica {} è {} (modificatore: {:+}).\n", caratteristica, CARATTERISTICA, modificatore(CARATTERISTICA)) << endl;
}

void leggitotali(span<int> vettore)
{
   static int contatore = 1;
   cout << format("{}[", (contatore > 1 ? "\n" : ""));
   for (auto indice = 0; const auto &i : vettore)
   {
      cout << i << (indice < 5 ? ", " : "");
      indice++;
   }
   cout << "] ";
   if (contatore > 1)
   {
      cout << " '-1'=già scelto";
   }
   contatore++;
}

int modificatore(int CARATTERISTICA)
{
   return static_cast<int>(floor((CARATTERISTICA - 10) / 2.0));
}