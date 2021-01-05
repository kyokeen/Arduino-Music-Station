//#include <Tone.h>
#include "pitches.h"


int octave1[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
  
int b1, b2, b3, b4, b5, b6, b7, b8;
int buttonPins[] = {22, 23, 24, 25, 26, 27, 28, 29};
int buzzerPins[] = {40, 41, 42, 43};

//Tone tone1, tone2, tone3, tone4;

void setup() {
 // configurare pini pentru butoane, intrare
  pinMode(buttonPins[0], INPUT_PULLUP);
  pinMode(buttonPins[1], INPUT_PULLUP);
  pinMode(buttonPins[2], INPUT_PULLUP);
  pinMode(buttonPins[3], INPUT_PULLUP);
  pinMode(buttonPins[4], INPUT_PULLUP);
  pinMode(buttonPins[5], INPUT_PULLUP);
  pinMode(buttonPins[6], INPUT_PULLUP);
  pinMode(buttonPins[7], INPUT_PULLUP);


  
  //tone1.begin(buzzerPins[0]);
  //tone1.stop();
  //tone2.begin(buzzerPins[1]);
  //tone3.begin(buzzerPins[2]);
  //tone4.begin(buzzerPins[3]);
}

void loop() {
  // citire stare butoane
  b1 = digitalRead(buttonPins[0]);
  b2 = digitalRead(buttonPins[1]);
  b3 = digitalRead(buttonPins[2]);
  b4 = digitalRead(buttonPins[3]);
  b5 = digitalRead(buttonPins[4]);
  b6 = digitalRead(buttonPins[5]);
  b7 = digitalRead(buttonPins[6]);
  b8 = digitalRead(buttonPins[7]);

  if (b1 == LOW) {
    //tone1.play(NOTE_G4);
    tone(buzzerPins[0], NOTE_C4);
  }
  else if (b2 == LOW) {
    tone(buzzerPins[0], NOTE_D4);
    //tone1.play(NOTE_A4);
  }
  else if  (b3 == LOW) {
    tone(buzzerPins[0], NOTE_E4);
    //tone1.play(NOTE_C5);
  }
  else if (b4 == LOW) {
    //tone1.play(NOTE_D5);
    tone(buzzerPins[0], NOTE_F4);
  }
  else if (b5 == LOW) {
    //tone1.play(NOTE_E5);
    tone(buzzerPins[0], NOTE_G4);
  }
  else if (b6 == LOW) {
    //tone1.play(NOTE_F5);
    tone(buzzerPins[0], NOTE_A4);
  }
  else if (b7 == LOW) {
    //tone1.play(NOTE_G5);
    tone(buzzerPins[0], NOTE_B4);
  }
  else if (b8 == LOW) {
    //tone1.play(NOTE_C6);
    tone(buzzerPins[0], NOTE_C5);
  }
  //else tone1.stop();
  else noTone(buzzerPins[0]);
}
