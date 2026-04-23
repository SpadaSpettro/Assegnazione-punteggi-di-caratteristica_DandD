#define NOMINMAX // Deactivates min e max Windows macros
#include <iostream>
#include <algorithm> // For sort() function
#include <limits>
#include <string>
#include <span>
#include <format>
#include <cmath> // For floor() function
#include <array>
#include <random> // Used instead of the "cstdlib" header for better random
#include <fstream>
// Include Windows.h if compiling on Windows
#ifdef _WIN32
#include <Windows.h> // Used to change terminal's encoding
#endif

using namespace std;

// Functions declaration
int inputInt_check();   // Checks the input where integer
char inputChar_check(); // Checks the input where character
void assigningAbility(string ability, span<int> array_tot, int &ability_score);
void printArray_tot(span<int> array_tot);
int Mod(int ability_score);

int main()
{
#ifdef _WIN32
   // Forces terminal to use UTF-8 encoding
   SetConsoleOutputCP(CP_UTF8);
   SetConsoleCP(CP_UTF8);
#endif
   // Random numbers generator setting (modern C++)
   random_device rd;
   mt19937 gen(rd());
   uniform_int_distribution<> d6(1, 6); // Set up for rolling a d6

   array<int, 4> rolls;
   array<int, 6> scores;
   int strength = 0, dexterity = 0, constitution = 0, intelligence = 0, wisdom = 0, charisma = 0; // Scores variables

   cout << "This program determines the ability scores of a D&D character using the dice rolling method.\n\n";
   for (auto h = 0; h < 6; h++)
   {
      int score = 0;
      cout << format("d6 rolls results, series {}: ", h + 1); // Using "format" to create clear and clean strings
      for (auto &i : rolls)
      {
         i = d6(gen); // Roll d6
         cout << " " << i;
      }
      // C++20 Ranges: order the array in a simple way
      ranges::sort(rolls, greater());

      cout << "\nThese are the 3 highest results: ";
      for (auto i = 0; i < 3; i++)
      {
         cout << " " << rolls.at(i);
         score += rolls.at(i); // An ability score is calculated as the sum of the 3 highest roll results
      }
      cout << "\n\n";
      scores[h] = score;
   }
   cout << "Sum of the 3 highest results for each roll series:  ";
   assigningAbility("STRENGTH", scores, strength);
   assigningAbility("DEXTERITY", scores, dexterity);
   assigningAbility("CONSTITUTION", scores, constitution);
   assigningAbility("INTELLIGENCE", scores, intelligence);
   assigningAbility("WISDOM", scores, wisdom);
   assigningAbility("CHARISMA", scores, charisma);

   cout << "Final Ability Scores:" << endl;
   cout << format("STRENGTH: {} (modifier: {:+}).\n", strength, Mod(strength)); // The use of "format" allows to automatically add the plus sign (+) in front of positive numbers
   cout << format("DEXTERITY: {} (modifier: {:+}).\n", dexterity, Mod(dexterity));
   cout << format("CONSTITUTION: {} (modifier: {:+}).\n", constitution, Mod(constitution));
   cout << format("INTELLIGENCE: {} (modifier: {:+}).\n", intelligence, Mod(intelligence));
   cout << format("WISDOM: {} (modifier: {:+}).\n", wisdom, Mod(wisdom));
   cout << format("CHARISMA: {} (modifier: {:+}).\n", charisma, Mod(charisma)) << endl;

   // Creation of a text file to save the results
   cout << "Save the results to a text file? (\"y\"=\"yes\"): ";
   char confirmSave = inputChar_check();
   if (confirmSave == 'y' || confirmSave == 'Y')
   {
      cout << "Enter the filename:\n";
      string filename;
      cin >> filename;
      ofstream file(filename + ".txt");

      // Writing inside the file
      file << "Final Ability Scores:\n";
      file << format("STRENGTH: {} (modifier: {:+}).\n", strength, Mod(strength));
      file << format("DEXTERITY: {} (modifier: {:+}).\n", dexterity, Mod(dexterity));
      file << format("CONSTITUTION: {} (modifier: {:+}).\n", constitution, Mod(constitution));
      file << format("INTELLIGENCE: {} (modifier: {:+}).\n", intelligence, Mod(intelligence));
      file << format("WISDOM: {} (modifier: {:+}).\n", wisdom, Mod(wisdom));
      file << format("CHARISMA: {} (modifier: {:+}).\n", charisma, Mod(charisma));

      file.close();
      cout << format("Results saved to {}.txt, inside the project folder.", filename);
   }

   // Stalls the execution until the user presses ENTER
   cin.ignore(numeric_limits<streamsize>::max(), '\n');
   cout << "\nPress ENTER to exit the program...";
   cin.get();
   return 0;
}

// Functions definition
int inputInt_check()
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

char inputChar_check()
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

void assigningAbility(const string ability, span<int> array_tot, int &ability_score)
{
   int scorePosition = 0;
   char confirmation = ' ';
   do
   {
      printArray_tot(array_tot);
      cout << format("\n\nSpecify the position (1-6) of the score to assign to the ability {}: ", ability);
      scorePosition = inputInt_check();
      int score = array_tot[scorePosition - 1];
      while (scorePosition < 1 || scorePosition > 6 || score == -1)
      {
         cout << "Error. Inadequate or already chosen position. Please try again: ";
         scorePosition = inputInt_check();
      }
      cout << format("You have chosen position \"{}\", which corresponds to the score \"{}\".\nThe score for the ability {} will be: {}. Its modifier will be: {:+}.\nConfirm the choice? (y/n): ", scorePosition, score, ability, score, Mod(score));
      confirmation = inputChar_check();
      while (confirmation != 'y' && confirmation != 'Y' && confirmation != 'n' && confirmation != 'N') // Verifies the allowed confirmations
      {
         cout << "Invalid character. Please enter \"y\" for \"yes\" or \"n\" for \"no\": ";
         confirmation = inputChar_check();
      }
   } while (confirmation == 'n' || confirmation == 'N');
   ability_score = array_tot[scorePosition - 1]; // Assigns the selected score to the relevant variable
   array_tot[scorePosition - 1] = -1;            // Makes the chosen score unavailable
   cout << format("The score chosen for the ability {} is {} (modifier: {:+}).\n", ability, ability_score, Mod(ability_score)) << endl;
}

void printArray_tot(span<int> array_tot)
{
   static int counter = 1;                           // A counter that keeps its value across function callings
   cout << format("{}[", (counter > 1 ? "\n" : "")); // Based on the counter value, the output will terminate with a new line character each time except for the first time. This is useful for output formatting
   for (auto h = 0; const auto &i : array_tot)       // A modern foreach loop can include a counter (h)...
   {
      cout << i << (h < 5 ? ", " : ""); // ...in this case the counter is used for output formatting
      h++;
   }
   cout << "] ";
   for (auto j : array_tot) // This foreach loop ensures that the "already chosen" note appears only after the assigning of the first ability
   {
      if (j == -1)
      {
         cout << " \"-1\"=already chosen";
         break;
      }
   }
   counter++; // Increases the counter. The variable will not reset
}

int Mod(int ability_score)
{
   return static_cast<int>(floor((ability_score - 10) / 2.0)); // The floor() function ensures a correct calculation of the ability modifier
}