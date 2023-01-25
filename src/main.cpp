/***
 * Auteur : Grégory BERTINI
 * Date : 25/01/2023
 * Description : Tests d'affichage et de modes
 * Version : 1.0
 * 
 * GitHub : https://github.com/SyFizz
 * Twitter : https://twitter.com/SyFizz_
 * Discord : SyFizz_#0001
 * E-Mail : gbertini.lpo@gmail.com
 * Tél : 06.84.15.85.17
*/

// Inclusions
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>

// Définitions de broches
#define greenLedPin 9
#define groupButtonPin 2
#define modeButtonPin 3

// Définitions de nombres
#define buttonsDebounceDelay 200
#define screenContrast 50

// Instanciation des classes
LiquidCrystal lcd(12, 11, 5, 4, 8, 7);

// Définitions de variables
unsigned long groupButtonLastPressed = 0;
unsigned long modeButtonLastPressed = 0;
int selectedGroup = 1;
int selectedMode = 0;
char* infos[4] = {"Vit. vent", "Dir. vent", "Temp.    ", "Pluvio.  "}; //Tableau de correspondance entre les ID de mode et leur texte
int modeForGroup[7] = {0, 0, 0, 0, 0, 0, 0}; //Tableau de stockage du mode pour chaque groupe

// Fonction d'interruption déclenchée par le bouton de groupe
void groupButtonPressed() {
  if(groupButtonLastPressed + buttonsDebounceDelay < millis()) { //Si le délai de rebond est dépassé
    selectedGroup++; //On passe au groupe suivant
      if (selectedGroup > 6) {
        selectedGroup = 1; //On revient au groupe 1 si on dépasse le groupe 6
      }
    groupButtonLastPressed = millis(); //On met à jour le dernier temps de pression du bouton
  }
}

// Fonction d'interruption déclenchée par le bouton d'info
void modeButtonPressed() {
  if(modeButtonLastPressed + buttonsDebounceDelay < millis()) { //Si le délai de rebond est dépassé
  selectedMode = modeForGroup[selectedGroup]; //On récupère le mode du groupe sélectionné
    selectedMode++; //On passe au mode suivant
    if (selectedMode > 3) {
      selectedMode = 0; //On revient au mode 0 si on dépasse le mode 3
    }
    modeButtonLastPressed = millis(); //On met à jour le dernier temps de pression du bouton
    modeForGroup[selectedGroup] = selectedMode; //On met à jour le mode pour le groupe sélectionné
  }
}

// Initialisation du programme
void setup() {
  // Initialisation des broches des boutons (résistance de pull-up)
  pinMode(groupButtonPin, INPUT_PULLUP);
  pinMode(modeButtonPin, INPUT_PULLUP);

  // Initialisation des broches des LEDs
  pinMode(greenLedPin, OUTPUT);

  // Création des interruptions pour les boutons
  attachInterrupt(digitalPinToInterrupt(groupButtonPin), groupButtonPressed, FALLING);
  attachInterrupt(digitalPinToInterrupt(modeButtonPin), modeButtonPressed, FALLING);

  // Initialisation de l'écran LCD
  analogWrite(6, screenContrast); //Contraste
  lcd.begin(16, 2); //Taille
  lcd.leftToRight();  //Lecture de gauche à droite
  lcd.clear(); //Effacement de l'écran

  // Initialisation des LEDs
  analogWrite(greenLedPin, 128);
}

void loop() {
  lcd.setCursor(0, 0); //Positionnement du curseur à la première ligne
  lcd.print("Groupe: "); //Affichage du texte
  lcd.print(selectedGroup);
  lcd.setCursor(0, 1); //Positionnement du curseur à la deuxième ligne
  lcd.print("Info: "); //Affichage du texte
  lcd.print(infos[modeForGroup[selectedGroup]]); 
}