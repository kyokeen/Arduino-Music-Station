// Citirea starii butoanelor conectate la pinii 2, 4, 7, 8
// afisarea prin interfata seriala
// se transmite un numar care are ultimele 4 cifre starea butoanelor apasate
// variabile pentru starea butoanelor

#include "pitches.h"
 
// notes in the melody:
int melody[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
  
int b1;
int b2;
int b3;
int b4;
int button1_pin = 2;
int button2_pin = 4;
int button3_pin = 12;
int button4_pin = 13;
int passiveBuzzer = 8;



void setup() {
 // configurare pini pentru butoane, intrare
  pinMode(button1_pin, INPUT_PULLUP);
  pinMode(button2_pin, INPUT_PULLUP);
  pinMode(button3_pin, INPUT_PULLUP);
  pinMode(button4_pin, INPUT_PULLUP);
  pinMode(passiveBuzzer, OUTPUT);
}

void loop() {
  // citire stare butoane
  b1 = digitalRead(button1_pin);
  b2 = digitalRead(button2_pin);
  b3 = digitalRead(button3_pin);
  b4 = digitalRead(button4_pin);

  if (b1 == LOW) {
    tone(passiveBuzzer, NOTE_G4);
  }
  else if (b2 == LOW) {
    tone(passiveBuzzer, NOTE_A4);
  }
  else if  (b3 == LOW) {
    tone(passiveBuzzer, NOTE_C5);
  }
  else if (b4 == LOW) {
    tone(passiveBuzzer, NOTE_D5);
  }
  else noTone(passiveBuzzer);
  
}
