#include <Tone.h>
//#include "pitches.h"


int octave[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
  
int b1, b2, b3, b4, b5, b6, b7, b8, trackButton;
int trackPin = 37;
int buttonPins[] = {22, 23, 24, 25, 26, 27, 28, 29};
int buzzerPins[] = {40, 41, 42, 43};
int ledPins[] = {50, 51, 52, 53};

Tone tones[4];
Tone currentTone;
int currentToneIndex = 0;

void setup() {
  Serial.begin(9600);

  
 // configurare pini pentru butoane, intrare
  pinMode(buttonPins[0], INPUT_PULLUP);
  pinMode(buttonPins[1], INPUT_PULLUP);
  pinMode(buttonPins[2], INPUT_PULLUP);
  pinMode(buttonPins[3], INPUT_PULLUP);
  pinMode(buttonPins[4], INPUT_PULLUP);
  pinMode(buttonPins[5], INPUT_PULLUP);
  pinMode(buttonPins[6], INPUT_PULLUP);
  pinMode(buttonPins[7], INPUT_PULLUP);
  //pinMode(trackPin, INPUT);

  pinMode(ledPins[0], OUTPUT);
  pinMode(ledPins[1], OUTPUT);
  pinMode(ledPins[2], OUTPUT);
  pinMode(ledPins[3], OUTPUT);

  
  //tones[0].begin(buzzerPins[0]);
  //tones[1].begin(buzzerPins[1]);
  //tones[2].begin(buzzerPins[2]);
  //tones[3].begin(buzzerPins[3]);
  
  //currentTone = tones[0];
  digitalWrite(ledPins[0], HIGH);
  digitalWrite(ledPins[1], LOW);
  digitalWrite(ledPins[2], LOW);
  digitalWrite(ledPins[3], LOW);
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
  //trackButton = digitalRead(trackPin);
  
  
  if (b1 == LOW) {
    //currentTone.play(octave[0]);
    Serial.println("b1");
    delay(500);
  }
  else if (b2 == LOW) {
    //currentTone.play(octave[1]);
    Serial.println("b2");
    delay(500);
  }
  else if  (b3 == LOW) {
    //currentTone.play(octave[2]);
    digitalWrite(ledPins[1], HIGH);
  }
  else if (b4 == LOW) {
    //currentTone.play(octave[3]);
    digitalWrite(ledPins[1], LOW);
  }
  else if (b5 == LOW) {
    //currentTone.play(octave[4]);
  }
  else if (b6 == LOW) {
    //currentTone.play(octave[5]);
  }
  else if (b7 == LOW) {
    //currentTone.play(octave[6]);
  }
  else if (b8 == LOW) {
    //currentTone.play(octave[7]);
  }
  else  {
    //currentTone.stop();
  }
  /*
  if (trackButton == LOW) {
    currentTone.stop();
    digitalWrite(ledPins[currentToneIndex], LOW);
    currentToneIndex = (currentToneIndex + 1) % 2 ;
    currentTone = tones[currentToneIndex];
    digitalWrite(ledPins[currentToneIndex], HIGH);
  }*/
  
}
