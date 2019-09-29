#include <Arduino.h>

#include "Debugger.h"

#define kPin    13

// Prototypes:
void Allumer();
void Eteindre();
int Sum(int arg1, int arg2);

// Fonctions:
void setup()
{
    STACKLEN();
    STACKGETSTART();
    Serial.begin(9600);
    
    Serial.println(F("Bienvenue dans le debugger."));
    Serial.println(F("Tapez:"));
    Serial.println(F("- STATE pour connaitre l'etat actuel,"));
    Serial.println(F("- STOP pour arreter le prgramme à l'instruction actuelle,"));
    Serial.println(F("- STOP XXXX YYYY pour arreter le programme lorsqu'il arrive dans une plage d'adresse,"));
    Serial.println(F("- PLAY pour reprendre l'execution,"));
    Serial.println(F("- RETURN pour obtenir l'adresse de retour de la fonction actuelle (etat stoppe),"));
    Serial.println(F("- NEXT pour continuer jusqu'a la prochaine instruction."));
    
    //LASTSP();
    
    BP(); pinMode(kPin, OUTPUT);
}

void loop()
{
    STACKLEN();
    STACKGETSTART();

    BP();  Allumer();
    BP(); delay(1000);
    
    
    BP(); Eteindre();
    BP(); delay(2000);

    BP(); int test = Sum(4, 5);
    BP(); Serial.println("Ceci est un test: " + String(test)); digitalWrite(kPin, LOW);
    
}

void Allumer()
{
    STACKLEN();
    STACKGETSTART();
    BP(); digitalWrite(kPin, HIGH);
}

void Eteindre()
{
    STACKLEN();
    STACKGETSTART();
    BP(); digitalWrite(kPin, LOW);
}

int Sum(int param1, int param2)
{
    STACKLEN();
    STACKGETSTART();
    BP(); DebugPrintVariable((uint16_t)&param1, sizeof(param1));
    BP(); DebugPrintVariable((uint16_t)&param2, sizeof(param2));
    BP(); int ret = param1 + param2;
  
  return ret;
}


