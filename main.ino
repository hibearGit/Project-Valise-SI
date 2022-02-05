#include <Servo.h>
#include "IRremote.h"

int receiver = 11; // Signal Pin of IR receiver to Arduino Digital Pin 11

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'


Servo myservo;
Servo myservo2;

const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output

#define ENABLE 5
#define DIRA 3
#define DIRB 4

int valFor;
int valTurn;
int valFrein;
int degres;
 



void translateIR() // takes action based on IR code received

// describing Remote IR codes 

{

  switch(results.value)

  {
  case 0xFFA25D: Serial.println("POWER"); break;
  case 0xFFE21D: Serial.println("FUNC/STOP"); break;
  
  case 0xFF629D: Serial.println("VOL+");
                 analogWrite(ENABLE,200); //enable on
                 digitalWrite(DIRA,HIGH); //one way
                 digitalWrite(DIRB,LOW);
  
                 break;
  case 0xFF22DD: Serial.println("FAST BACK");    break;
  case 0xFF02FD: Serial.println("PAUSE");    break;
  case 0xFFC23D: Serial.println("FAST FORWARD");   break;
  case 0xFFE01F: Serial.println("DOWN");    break;
  case 0xFFA857: Serial.println("VOL-");    break;
  case 0xFF906F: Serial.println("UP");    break;
  case 0xFF9867: Serial.println("EQ");    break;
  case 0xFFB04F: Serial.println("ST/REPT");    break;
  case 0xFF6897: Serial.println("0");    break;
  case 0xFF30CF: Serial.println("1");    break;
  case 0xFF18E7: Serial.println("2");    break;
  case 0xFF7A85: Serial.println("3");    break;
  case 0xFF10EF: Serial.println("4");    break;
  case 0xFF38C7: Serial.println("5");    break;
  case 0xFF5AA5: Serial.println("6");    break;
  case 0xFF42BD: Serial.println("7");    break;
  case 0xFF4AB5: Serial.println("8");    break;
  case 0xFF52AD: Serial.println("9");    break;
  case 0xFFFFFFFF: Serial.println(" REPEAT");
                   analogWrite(ENABLE,200); //enable on
                   digitalWrite(DIRA,HIGH); //one way
                   digitalWrite(DIRB,LOW);
  
                   break;  

  default: 
    Serial.println(" other button   ");

  }// End Case

}





void setup() {

  irrecv.enableIRIn();

  myservo.attach(9);
  myservo.write(90);

  myservo2.attach(13);
  myservo2.write(0);

  
  //---set pin direction
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  Serial.begin(9600);

  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);

  Serial.begin(9600);
}

void loop() {


  delay(50);

  

  valFor = analogRead(X_pin);
  valFor = map (valFor, 0, 1023, 0, 255);
  if (valFor>= 126){
    valFor = valFor - 126;
    }
  else{
      valFor = 0;
    }

  analogWrite(ENABLE,valFor); //enable on
  digitalWrite(DIRA,HIGH); //one way
  digitalWrite(DIRB,LOW);


  valTurn = analogRead(Y_pin);
  valTurn = map (valTurn, 0, 1023, 0, 179);
  myservo.write(valTurn);

  

  delay(50);

  valFrein = digitalRead(SW_pin);

  degres = myservo2.read();
  
  switch(valFrein){
    case HIGH : myservo2.write(degres); break;
    case LOW : if (degres <=80){
      myservo2.write(180);
      }
              else{
      myservo2.write(0);          
                }
      }

  if (irrecv.decode(&results)) // have we received an IR signal?

  {
    translateIR(); 
    irrecv.resume(); // receive the next value
  } 
  
}
