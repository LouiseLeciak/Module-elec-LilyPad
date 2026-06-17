
// https://passionelectronique.fr/encodeur-rotatif-incremental-mecanique/

#define pinArduinoRaccordementSignalSW  2 
#define pinArduinoRaccordementSignalCLK 3 
#define pinArduinoRaccordementSignalDT  4 

// Variables
int etatPrecedentLigneSW;         
int compteur = 0;                  
                                   

void setup() {

    Serial.begin(9600);
    Serial.println(F("=========================================================================="));
    Serial.println(F("Exemple 2 (programme de test Arduino Nano <-> module KY-040, utilisant les"));
    Serial.println(F("           interruptions Arduino INT0 et INT1, avec affichage du nombre de"));
    Serial.println(F("           crans parcourus sur l'encodeur, ainsi que le sens de rotation)"));
    Serial.println(F("========================================================================="));
    Serial.println("");

    
    pinMode(pinArduinoRaccordementSignalSW, INPUT);        
                                                          
    pinMode(pinArduinoRaccordementSignalCLK, INPUT);

    delay(200);

    etatPrecedentLigneSW  = digitalRead(pinArduinoRaccordementSignalSW);

    Serial.print(F("Valeur initiale du compteur = "));
    Serial.println(compteur);

    attachInterrupt(digitalPinToInterrupt(pinArduinoRaccordementSignalCLK), changementDetecteSurLigneCLK, FALLING); // FALLING => détecte tout front descendant
    attachInterrupt(digitalPinToInterrupt(pinArduinoRaccordementSignalSW), changementDetecteSurLigneSW, CHANGE);    // CHANGE => détecte tout changement d'état

}

void loop() {


}

void changementDetecteSurLigneCLK() {

    int etatActuelDeLaLigneDT  = digitalRead(pinArduinoRaccordementSignalDT);
    int etatActuelDeLaLigneCLK = LOW;
      
    if(etatActuelDeLaLigneCLK != etatActuelDeLaLigneDT) {
        compteur++;

        Serial.print(F("Sens = horaire | Valeur du compteur = "));
        Serial.println(compteur);
    }
    else {
        compteur--;
        Serial.print(F("Sens = antihoraire | Valeur du compteur = "));
        Serial.println(compteur);
    }
    
}


void changementDetecteSurLigneSW() {
    int etatActuelDeLaLigneSW = digitalRead(pinArduinoRaccordementSignalSW);

    etatPrecedentLigneSW = etatActuelDeLaLigneSW;

    if(etatActuelDeLaLigneSW == LOW)
        Serial.println(F("Bouton SW appuyé"));
    else
        Serial.println(F("Bouton SW relâché"));

}

