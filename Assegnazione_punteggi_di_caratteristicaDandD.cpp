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
// Include Windows.h solo se si compila su Windows
#ifdef _WIN32
#include <Windows.h> //Necessaria per cambiare la codifica del terminale
#endif

#include <fstream>

using namespace std;

// Dichiarazione funzioni
int controllaInt();
char controllaChar();
void assegnazioneCaratteristica(string caratteristica, span<int> array_totali, int &punteggio_caratteristica);
void stampaArray_totali(span<int> array_totali);
int calcoloModificatore(int punteggio_caratteristica);

int main()
{
#ifdef _WIN32
   // Forza il terminale a usare la codifica UTF-8
   SetConsoleOutputCP(CP_UTF8);
   SetConsoleCP(CP_UTF8);
#endif
   // Impostazione del generatore di numeri casuali (C++ moderno)
   random_device rd;
   mt19937 gen(rd());
   uniform_int_distribution<> d6(1, 6); // Configura un dado a 6 facce

   array<int, 4> esitoLanci;
   array<int, 6> punteggi;
   int forza = 0, destrezza = 0, costituzione = 0, intelligenza = 0, saggezza = 0, carisma = 0; // Variabili per punteggi

   cout << "Questo programma determina i punteggi di caratteristica di un personaggio utilizzando il metodo del lancio del d6.\n\n";
   for (auto h = 0; h < 6; h++)
   {
      int punteggio = 0;
      // Uso di "format" per creare stringhe chiare e pulite
      cout << format("Risultati dei tiri del d6, {}ª serie: ", h + 1);

      for (auto &i : esitoLanci)
      {
         i = d6(gen); // Lancia il d6
         cout << " " << i;
      }
      // C++20 Ranges: ordina l'array in modo molto semplice
      ranges::sort(esitoLanci, greater());

      cout << "\nQuesti sono i 3 risultati più alti: ";
      for (auto i = 0; i < 3; i++)
      {
         cout << " " << esitoLanci[i];
         punteggio += esitoLanci[i];
      }
      cout << "\n\n";
      punteggi[h] = punteggio;
   }
   cout << "Somme dei 3 risultati più alti per ogni sequenza di tiri:  ";
   assegnazioneCaratteristica("FORZA", punteggi, forza);
   assegnazioneCaratteristica("DESTREZZA", punteggi, destrezza);
   assegnazioneCaratteristica("COSTITUZIONE", punteggi, costituzione);
   assegnazioneCaratteristica("INTELLIGENZA", punteggi, intelligenza);
   assegnazioneCaratteristica("SAGGEZZA", punteggi, saggezza);
   assegnazioneCaratteristica("CARISMA", punteggi, carisma);

   cout << "Punteggi di caratteristica definitivi:" << endl;
   cout << format("FORZA: {} (modificatore: {:+}).\n", forza, calcoloModificatore(forza));
   cout << format("DESTREZZA: {} (modificatore: {:+}).\n", destrezza, calcoloModificatore(destrezza));
   cout << format("COSTITUZIONE: {} (modificatore: {:+}).\n", costituzione, calcoloModificatore(costituzione));
   cout << format("INTELLIGENZA: {} (modificatore: {:+}).\n", intelligenza, calcoloModificatore(intelligenza));
   cout << format("SAGGEZZA: {} (modificatore: {:+}).\n", saggezza, calcoloModificatore(saggezza));
   cout << format("CARISMA: {} (modificatore: {:+}).\n", carisma, calcoloModificatore(carisma)) << endl;

   // Creazione file di testo con i risultati
   cout << "Salvare i risultati in un file di testo? (\"s\"=\"sì\"): ";
   char confermaSalvataggio_file = controllaChar();
   if (confermaSalvataggio_file == 's' || confermaSalvataggio_file == 'S')
   {
      cout << "Inserisci il nome del file:\n";
      string nomeFile;
      cin >> nomeFile;
      ofstream file(nomeFile + ".txt");
      file << "Punteggi di caratteristica definitivi:\n";
      file << format("FORZA: {} (modificatore: {:+}).\n", forza, calcoloModificatore(forza));
      file << format("DESTREZZA: {} (modificatore: {:+}).\n", destrezza, calcoloModificatore(destrezza));
      file << format("COSTITUZIONE: {} (modificatore: {:+}).\n", costituzione, calcoloModificatore(costituzione));
      file << format("INTELLIGENZA: {} (modificatore: {:+}).\n", intelligenza, calcoloModificatore(intelligenza));
      file << format("SAGGEZZA: {} (modificatore: {:+}).\n", saggezza, calcoloModificatore(saggezza));
      file << format("CARISMA: {} (modificatore: {:+}).\n", carisma, calcoloModificatore(carisma));
      file.close();
      cout << format("Risultati salvati in {}.txt, nella cartella del progetto.", nomeFile);
   }

   cin.ignore(numeric_limits<streamsize>::max(), '\n');
   cout << "\nPremere Invio per uscire...";
   cin.get();
   return 0;
}

// Definizione funzioni
int controllaInt()
{
   float input = 0.0;
   while (!(cin >> input) || input != static_cast<int>(input))
   {
      cout << "Errore. Valore non riconosciuto. Riprovare: ";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
   }
   cin.ignore(numeric_limits<streamsize>::max(), '\n');
   return static_cast<int>(input);
}

char controllaChar()
{
   char input = ' ';
   while (!(cin >> input))
   {
      cout << "Errore. Carattere non riconosciuto. Riprovare: ";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
   }
   cin.ignore(numeric_limits<streamsize>::max(), '\n');
   return input;
}

void assegnazioneCaratteristica(const string caratteristica, span<int> array_totali, int &punteggio_caratteristica)
{
   int posizionePunteggio = 0;
   char conferma = ' ';
   do
   {
      stampaArray_totali(array_totali);
      cout << format("\n\nSpecificare la posizione (1-6) del punteggio da assegnare alla caratteristica {}: ", caratteristica);
      posizionePunteggio = controllaInt();
      int punteggio = array_totali[posizionePunteggio - 1];
      while (posizionePunteggio < 1 || posizionePunteggio > 6 || punteggio == -1)
      {
         cout << "Errore (-1). Valore non adeguato o già scelto. Riprovare: ";
         posizionePunteggio = controllaInt();
      }
      cout << format("Hai scelto la posizione {}, che corrisponde al punteggio {}.\n", posizionePunteggio, punteggio);
      cout << format("Il punteggio della caratteristica {} sarà: {}. Il suo modificatore sarà: {:+}.\n\n", caratteristica, punteggio, calcoloModificatore(punteggio));
      cout << "Confermi la scelta? (s/n): ";
      conferma = controllaChar();
      while (conferma != 's' && conferma != 'S' && conferma != 'n' && conferma != 'N')
      {
         cout << "Carattere non adeguato. Inserire \"s\" per \"sì\" o \"n\" per \"no\": ";
         conferma = controllaChar();
      }
   } while (conferma == 'n' || conferma == 'N');
   punteggio_caratteristica = array_totali[posizionePunteggio - 1];
   array_totali[posizionePunteggio - 1] = -1; // Rende il punteggio scelto non più disponibile
   cout << format("Il punteggio scelto per la caratteristica {} è {} (modificatore: {:+}).\n", caratteristica, punteggio_caratteristica, calcoloModificatore(punteggio_caratteristica)) << endl;
}

void stampaArray_totali(span<int> array_totali)
{
   static int contatore = 1;
   cout << format("{}[", (contatore > 1 ? "\n" : ""));
   for (auto h = 0; const auto &i : array_totali)
   {
      cout << i << (h < 5 ? ", " : "");
      h++;
   }
   cout << "] ";
   if (contatore > 1)
   {
      cout << " \"-1\"=già scelto";
   }
   contatore++;
}

int calcoloModificatore(int punteggio_caratteristica)
{
   return static_cast<int>(floor((punteggio_caratteristica - 10) / 2.0));
}