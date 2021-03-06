//  LIBRAIRIES UTILISEES  //

#include <Servo.h>
#include "IRremote.h"

//************************//


// DECLARATION DES 2 SERVOMOTEURS //

Servo servDir;
Servo servFrein;

//********************************//


// DECLARATION DU PONT EN H CONTROLANT LE MOTEUR CC //

#define ENABLE 5
#define DIRA 3
#define DIRB 4

//*************************************************//


// DECLARATION DU JOYSTICK  //

const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output

//*************************//


// DECLARATION DU POTENTIOMETRE  //

int pot = 5;//Déclaration

//*************************//


// DECLARATION DU MODULE INFRAROUGE  //

int receiver = 11; // Signal Pin of IR receiver to Arduino Digital Pin 11

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

//*************************//

// DECLARATION DES VARIABLES //

int valUtilisation;// Prend la valeur de l'état du potentiometre (1024 valeurs)
boolean utilisation;// Valeur binaire

int valFor;
int valTurn;
int valFrein;
int degres;

//*************************//

//*FIN DES DECLARATIONS*//




//####################################################################################################################################//



// FONCTION MARCHE/ARRET DU FREIN A MAIN //

void frein (){

  valFrein = digitalRead(SW_pin);
    degres = servFrein.read();
  
    switch(valFrein){
      case HIGH : servFrein.write(degres); 
        break;
      
      case LOW : 
      
        if (degres <=80){
          servFrein.write(180);
          }
        else{
          servFrein.write(0);          
          }
        break;
        }
  
}



//*************************//



// FONCTION DE TRANSMITION D'INFORMATION SELON LES TOUCHES DE LA TELECOMMANDE IR //

void translateIR(){ // takes action based on IR code received
  




// FIN DE LA FONCTION TRANSMITION IR //


  switch(results.value)

  {
  
  case 0xFF629D: Serial.println("VOL+");
                 analogWrite(ENABLE,200); //enable on
                 digitalWrite(DIRA,HIGH); //one way
                 digitalWrite(DIRB,LOW);
  
                 break;


                 
  case 0xFF22DD: Serial.println("FAST BACK");   
                 servDir.write(0);
  
                 break;


                 
  case 0xFF02FD: Serial.println("PAUSE");  
                 servDir.write(90);

                 break;
                 
  case 0xFFC23D: Serial.println("FAST FORWARD");  
                 servDir.write(180);
  
                 break;


                 
  case 0xFFA857: Serial.println("VOL-"); 
                 analogWrite(ENABLE,200); //enable on
                 digitalWrite(DIRA,LOW); //one way
                 digitalWrite(DIRB,HIGH);
  
                 break;

  case 0xFFE21D: Serial.println("FUNC/STOP");
                 analogWrite(ENABLE,0); //enable on
                 digitalWrite(DIRA,LOW); //one way
                 digitalWrite(DIRB,HIGH);
                 
                 break;
                 
  
  case 0xFFFFFFFF: Serial.println("Repeat"); 
  
                   break;  
  case 0xFF9867: Serial.println("EQ");   

                 frein();
  
                 break;
  
  default: 
    Serial.println("Touche inconnue ");

  }// End Case

  delay(500);
}


//####################################################################################################################################//




// DEBUT DU SETUP //

void setup() {

  Serial.begin(9600); // Vitesse de la carte à 9600


  irrecv.enableIRIn(); // Démarrer le capteur IR

  // INIT DES 2 SERVOMOTEURS //
  servDir.attach(9);
  servDir.write(90);

  servFrein.attach(13);
  servFrein.write(0);
  //************************//
  
  // METTRE LES PINS DU PONT EN H EN SORTIE //
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  //************************//

  // INIT DES ENTREES DU JOYSTICK //
  pinMode(SW_pin, INPUT); /*Déclaration du joystick comme une entrée*/
  digitalWrite(SW_pin, HIGH); /*Initialisation du bouton poussoir du joystick sur HAUT*/
  //***********************//

  
  
  
  }

// FIN DU SETUP //








//####################################################################################################################################//







// DEBUT DE LA BOUCLE LOOP  //









void loop() {


delay(50);

valUtilisation = analogRead(pot); //valUtilisateur prend la valeur du potentiometre

if (valUtilisation < 512){
    utilisation = true;
    }                            //Tansforme la valeur analogique du potentiometre en une valeur binaire
  else {
    utilisation = false;  
  }






switch (utilisation) { // Choix du mode d'utilisation : soit une utlisation avec le joystick, soit avec la télécommande IR
  case true : // Utilisation en mode joystick

    // CONTROLE DU MOTEUR CC //
    valFor = analogRead(X_pin);
    valFor = map (valFor, 0, 1023, 0, 255);
    
    if (valFor>= 126){
      valFor = valFor - 126;
      }
    else{
      valFor = 0;
       }

    analogWrite(ENABLE,valFor); 
    digitalWrite(DIRA,HIGH); 
    digitalWrite(DIRB,LOW);
    //*********************//

    // CONTROLE DU SERVOMOTEUR DIRECTIONELLE //

    valTurn = analogRead(Y_pin);
    valTurn = map (valTurn, 0, 1023, 0, 179);
    servDir.write(valTurn);

    //*********************//

    // CONTROLE DU SERVOMOTEUR FREIN A MAIN //

    frein();

    //*********************//
    Serial.println(valUtilisation);
    
    break; //Fin du cas : contrôle joystick




    case false : 
      if (irrecv.decode(&results)){ // have we received an IR signal?
        translateIR(); 
        irrecv.resume(); // receive the next value
        } 
    break;
    }
}


















