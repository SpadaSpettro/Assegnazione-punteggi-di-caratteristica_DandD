#define NOMINMAX
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <Windows.h> //Necessaria per cambiare la codifica del terminale
#include <limits>
#include <string>
#include <span>

using namespace std;

//Dichiarazione funzioni
int controllaScelta();
char controllaConferma();
void sceltaCaratteristica(string caratteristica, span<int> vettore, int &CARATTERISTICA);
void elencoDefinitivo(string caratteristica, int CARATTERISTICA);
void leggitotali(span<int> vettore);

int main() {
   // Forza il terminale a usare la codifica UTF-8
   SetConsoleOutputCP(CP_UTF8);
   SetConsoleCP(CP_UTF8);
   srand(time(0));

   int tiri[4], totali[6], FORZA, DESTREZZA, COSTITUZIONE, INTELLIGENZA, SAGGEZZA, CARISMA; //Variabili

   cout<<"Questo programma determina i punteggi di caratteristica di un personaggio utilizzando il metodo del lancio del d6.\n"<<endl;
   for (auto j=0;j<6;j++){
      int tot=0;
      cout<<"Risultati dei tiri del d6, "<<j+1<<"ª serie: ";
      for (auto &i : tiri){
         i=(rand()%6)+1;
         cout<<" "<<i;
      }
      sort(begin(tiri), end(tiri), greater<int>());
      cout<<"\nQuesti sono i 3 risultati più alti: ";
      for (auto i=0; i<3; i++){
         cout<<" "<<tiri[i];
         tot+=tiri[i];
      }
      cout<<"\n"<<endl;
      totali[j]=tot;
   }
   cout<<"Somme dei 3 risultati più alti per ogni sequenza di tiri:  ";
   sceltaCaratteristica("FORZA", totali, FORZA);
   sceltaCaratteristica("DESTREZZA", totali, DESTREZZA);
   sceltaCaratteristica("COSTITUZIONE", totali, COSTITUZIONE);
   sceltaCaratteristica("INTELLIGENZA", totali, INTELLIGENZA);
   sceltaCaratteristica("SAGGEZZA", totali, SAGGEZZA);
   sceltaCaratteristica("CARISMA", totali, CARISMA);
   cout<<"\nPunteggi di caratteristica definitivi:"<<endl;
   elencoDefinitivo("FORZA", FORZA);
   elencoDefinitivo("DESTREZZA", DESTREZZA);
   elencoDefinitivo("COSTITUZIONE", COSTITUZIONE);
   elencoDefinitivo("INTELLIGENZA", INTELLIGENZA);
   elencoDefinitivo("SAGGEZZA", SAGGEZZA);
   elencoDefinitivo("CARISMA", CARISMA);
   
   cin.ignore(numeric_limits<streamsize>::max(), '\n');
   cin.get();
   return 0;
}

//Definizione funzioni
int controllaScelta() {
   int valore;
   while (!(cin >> valore) || valore!=static_cast<int>(valore)) {
      cout << "Errore. Valore non riconosciuto. Riprovare: ";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
   }
   cin.ignore(numeric_limits<streamsize>::max(), '\n');
   return valore;
}

char controllaConferma() {
   char valore;
   while (!(cin >> valore)) {
      cout << "Errore. Carattere non riconosciuto. Riprovare: ";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
   }
   cin.ignore(numeric_limits<streamsize>::max(), '\n');
   return valore;
}

void sceltaCaratteristica(string caratteristica, span<int> vettore, int &CARATTERISTICA) {
   int scelta;
   char conferma;
   leggitotali(vettore);
   do {
      cout<<"\n\nSpecificare la posizione (1-6) del punteggio da assegnare alla caratteristica "<<caratteristica<<": ";
      scelta=controllaScelta();
      while (scelta<1 || scelta>6 || vettore[scelta-1]==-1){
         cout<<"Errore (-1). Valore non adeguato o già scelto. Riprovare: ";
         scelta=controllaScelta();
      }
      cout<<"\nHai scelto la posizione "<<scelta<<", che corrisponde al punteggio "<<vettore[scelta-1]<<endl;
      cout<<"Il punteggio della caratteristica "<<caratteristica<<" sarà: "<<vettore[scelta-1]<<". Il suo modificatore sarà: "<<(((vettore[scelta-1]-10)/2)>0 ? "+" : "")<<(vettore[scelta-1]-10)/2<<endl;
      cout<<"Confermi la scelta? (s/n): ";
      conferma = controllaConferma();
      while (conferma!='s' && conferma!='S' && conferma!='n' && conferma!='N'){
         cout<<"Carattere non adeguato. Inserire 's' per 'sì' o 'n' per 'no': ";
         conferma = controllaConferma();
      }
   }
   while (conferma=='n' || conferma=='N');
   CARATTERISTICA=vettore[scelta-1];
   vettore[scelta-1]=-1; //Rende il punteggio scelto non più disponibile
   cout<<"Il punteggio scelto per la caratteristica "<<caratteristica<<" è: "<<CARATTERISTICA<<" (modificatore: "<<(((CARATTERISTICA-10)/2)>0 ? "+" : "")<<(CARATTERISTICA-10)/2<<")\n"<<endl;
}

void elencoDefinitivo(string caratteristica, int CARATTERISTICA) {
   cout<<caratteristica<<": "<<CARATTERISTICA<<" (modificatore: "<<(((CARATTERISTICA-10)/2)>0 ? "+" : "")<<(CARATTERISTICA-10)/2<<")"<<endl;
}

void leggitotali(span<int> vettore) {
   static int contatore=1;
   cout<<"[";
   for (auto indice=0; const auto& i : vettore){
      cout<<i<<(indice<5 ? ", " : "");
      indice++;
   }
   cout<<"] ";
   if (contatore>=2){
      cout<<" '-1'=già scelto";
   }
   contatore++;
}