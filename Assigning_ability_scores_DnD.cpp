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
// Include Windows.h if compiling on Windows
#ifdef _WIN32
#include <Windows.h> // Necessary to change terminal's encoding
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
   // Force UTF-8 encoding in terminal
   SetConsoleOutputCP(CP_UTF8);
   SetConsoleCP(CP_UTF8);
#endif
   // Random numbers generator setting (modern C++)
   random_device rd;
   mt19937 gen(rd());
   uniform_int_distribution<> d6(1, 6); // Set up for rolling a d6

   array<int, 4> esitoLanci;
   array<int, 6> punteggi;
   int forza = 0, destrezza = 0, costituzione = 0, intelligenza = 0, saggezza = 0, carisma = 0; // Variabili per punteggi

   cout << "This program determines the ability scores of a D&D character using the dice rolling method.\n\n";
   for (auto h = 0; h < 6; h++)
   {
      int punteggio = 0;
      // Using "format" to create clear and clean strings
      cout << format("Results of the d6 rolls, {} series: ", h + 1);

      for (auto &i : esitoLanci)
      {
         i = d6(gen); // Roll d6
         cout << " " << i;
      }
      // C++20 Ranges: order the array in a simple way
      ranges::sort(esitoLanci, greater());

      cout << "\nThese are the 3 highest results: ";
      for (auto i = 0; i < 3; i++)
      {
         cout << " " << esitoLanci[i];
         punteggio += esitoLanci[i];
      }
      cout << "\n\n";
      punteggi[h] = punteggio;
   }
   cout << "Sums of the 3 highest results for each roll series:  ";
   assegnazioneCaratteristica("STRENGTH", punteggi, forza);
   assegnazioneCaratteristica("DEXTERITY", punteggi, destrezza);
   assegnazioneCaratteristica("CONSTITUTION", punteggi, costituzione);
   assegnazioneCaratteristica("INTELLIGENCE", punteggi, intelligenza);
   assegnazioneCaratteristica("WISDOM", punteggi, saggezza);
   assegnazioneCaratteristica("CHARISMA", punteggi, carisma);

   cout << "Final Ability Scores:" << endl;
   cout << format("STRENGTH: {} (modifier: {:+}).\n", forza, calcoloModificatore(forza));
   cout << format("DEXTERITY: {} (modifier: {:+}).\n", destrezza, calcoloModificatore(destrezza));
   cout << format("CONSTITUTION: {} (modifier: {:+}).\n", costituzione, calcoloModificatore(costituzione));
   cout << format("INTELLIGENCE: {} (modifier: {:+}).\n", intelligenza, calcoloModificatore(intelligenza));
   cout << format("WISDOM: {} (modifier: {:+}).\n", saggezza, calcoloModificatore(saggezza));
   cout << format("CHARISMA: {} (modifier: {:+}).\n", carisma, calcoloModificatore(carisma)) << endl;

   // Creation of a text file for the results
   cout << "Save the results to a text file? (\"y\"=\"yes\"): ";
   char confermaSalvataggio_file = controllaChar();
   if (confermaSalvataggio_file == 'y' || confermaSalvataggio_file == 'Y')
   {
      cout << "Enter the filename:\n";
      string nomeFile;
      cin >> nomeFile;
      ofstream file(nomeFile + ".txt");
      file << "Final Ability Scores:\n";
      file << format("STRENGTH: {} (modifier: {:+}).\n", forza, calcoloModificatore(forza));
      file << format("DEXTERITY: {} (modifier: {:+}).\n", destrezza, calcoloModificatore(destrezza));
      file << format("CONSTITUTION: {} (modifier: {:+}).\n", costituzione, calcoloModificatore(costituzione));
      file << format("INTELLIGENCE: {} (modifier: {:+}).\n", intelligenza, calcoloModificatore(intelligenza));
      file << format("WISDOM: {} (modifier: {:+}).\n", saggezza, calcoloModificatore(saggezza));
      file << format("CHARISMA: {} (modifier: {:+}).\n", carisma, calcoloModificatore(carisma));
      file.close();
      cout << format("Results saved to {}.txt, inside the project folder.", nomeFile);
   }

   cin.ignore(numeric_limits<streamsize>::max(), '\n');
   cout << "\nPress Enter to exit...";
   cin.get();
   return 0;
}

// Definizione funzioni
int controllaInt()
{
   float input = 0.0;
   while (!(cin >> input) || input != static_cast<int>(input))
   {
      cout << "Error. Unknown value. Please try again: ";
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
      cout << "Error. Unknown character. Please try again: ";
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
      cout << format("\n\nSpecify the position (1-6) of the score to assign to the ability {}: ", caratteristica);
      posizionePunteggio = controllaInt();
      int punteggio = array_totali[posizionePunteggio - 1];
      while (posizionePunteggio < 1 || posizionePunteggio > 6 || punteggio == -1)
      {
         cout << "Error (-1). Inadequate or already chosen position. Please try again: ";
         posizionePunteggio = controllaInt();
      }
      cout << format("You have chosen position \"{}\", which corresponds to the score \"{}\".\n", posizionePunteggio, punteggio);
      cout << format("The score for the ability {} will be: {}. Its modifier will be: {:+}.\n", caratteristica, punteggio, calcoloModificatore(punteggio));
      cout << "Confirm the choice? (y/n): ";
      conferma = controllaChar();
      while (conferma != 'y' && conferma != 'Y' && conferma != 'n' && conferma != 'N')
      {
         cout << "Invalid character. Please enter \"y\" for \"yes\" or \"n\" for \"no\": ";
         conferma = controllaChar();
      }
   } while (conferma == 'n' || conferma == 'N');
   punteggio_caratteristica = array_totali[posizionePunteggio - 1];
   array_totali[posizionePunteggio - 1] = -1; // Makes the chosen score unavailable
   cout << format("The score chosen for the ability {} is {} (modifier: {:+}).\n", caratteristica, punteggio_caratteristica, calcoloModificatore(punteggio_caratteristica)) << endl;
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
   for (auto j : array_totali)
   {
      if (j == -1)
      {
         cout << " \"-1\"=already chosen";
         break;
      }
   }
   contatore++;
}

int calcoloModificatore(int punteggio_caratteristica)
{
   return static_cast<int>(floor((punteggio_caratteristica - 10) / 2.0));
}