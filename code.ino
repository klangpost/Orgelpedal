#include <MIDIUSB.h>
 
//Variablen für Pedaltasten
// Array mit Pinnummern
const int pedalPins[] = {
  22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
  32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
  42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
  52
};
 
// Midi Noten
const int midiNote[] = {
  36, 37, 38, 39, 40, 41, 42, 43, 44, 45,
  46, 47, 48, 49, 50, 51, 52, 53, 54, 55,
  56, 57, 58, 59, 60, 61, 62, 63, 64, 65
};
 
#define NUM_PEDALS (sizeof(pedalPins) / sizeof(int))  // Anzahl der Pedale berechnen
 
// Anzahl der Tasten ermitteln
bool pedalState[NUM_PEDALS];
 

/**** Funktionen für MIDI ***************************************************/
void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}
 
void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}
 
void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}
 
bool MidiButton(int pin, byte midisig, bool tasteAlt) {      //Button-Pin, Noten-Nummer
  bool taste = digitalRead(pin);
  if(tasteAlt == LOW && taste == HIGH){
    
    Serial.print("NoteON MidiNote: ");
    Serial.print(midisig);
    
    noteOn(0, midisig, 64);  
    MidiUSB.flush();
    tasteAlt = taste;
    delay(100);
    return tasteAlt;
  }
 
  if(tasteAlt == HIGH && taste == LOW){
    Serial.print("NoteOFF MidiNote: ");
    Serial.print(midisig);
    noteOff(0, midisig, 64);  
    MidiUSB.flush();
    tasteAlt = taste;
    delay(100);
    return tasteAlt;
  }
}
 

/****************************************************************************/
 
void setup() {
  Serial.begin(9600); // Öffnet die serielle Schnittstelle bei 9600 Bit/s:
  Serial.print("Setup beginnt");
  Serial.println();
 
  // Setze alle Pedalpins als Eingang mit internen Pull-up-Widerständen (passe dies nach Bedarf an)
  for (int i = 0; i < NUM_PEDALS; i++) {
    pinMode(pedalPins[i], INPUT_PULLUP);
    pedalState[i] = HIGH;
  }
  
  Serial.print("Setup abgeschlossen");
  Serial.println();
}
 
void loop() {
  //Pedaltasten
  for (int i = 0; i < NUM_PEDALS; i++) {
    bool currentState = pedalState[i];
    pedalState[i] = MidiButton(pedalPins[i], midiNote[i], currentState);
  }
  
 
}
