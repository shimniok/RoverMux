
uint16_t tstart[20]; // start time
uint16_t tdur[20];   // duration
byte prevb, prevc, prevd; // previous pin states

void pciSetup(byte pin) {
  *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
  PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
  PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
  tstart[pin] = micros();
  tdur[pin] = 0; 
}

uint16_t getDuration(byte pin) {
  return (pin >= 0 && pin < 20) ? tdur[pin] : 0;
}

// handle pin change interrupt for D8 to D13 here
ISR (PCINT0_vect) {
  byte changed;
  const byte offset = 8; // PB0 ==> D8
  
  changed = PINB ^ prevb;
  prevb = PINB;
  
  byte b = 0x01; // bitmask
  for (int i = 0; i < 5; i++) {

    b <<= 1;
    if (b & changed) {

      if (PINB & b) { // high
        tstart[i+offset] = micros();
      } else {        // low
        tdur[i+offset] = micros() - tstart[i+offset];
      }

    } // if changed

  } // for
  
} // PCINT0_vect
 
// handle pin change interrupt for A0 to A5 here 
ISR (PCINT1_vect) {
  const byte offset = 14; // PC0 ==> D14
} // PCINT1_vect
  
// handle pin change interrupt for D0 to D7 here
ISR (PCINT2_vect) {
  const byte offset = 0; // PD0 ==> D0
} // PCINT2_vect
 

