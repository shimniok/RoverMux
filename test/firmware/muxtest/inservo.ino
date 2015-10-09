
uint16_t tstart[20]; // start time
uint16_t tdur[20];   // duration
byte prevb, prevc, prevd; // previous pin states


void servoTimer(byte pin) {
  *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
  PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
  PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
  tstart[pin] = 0;
  tdur[pin] = 0;
  prevb = PINB;
  prevc = PINC;
  prevd = PIND;
}


uint16_t getDuration(byte pin) {
  uint16_t dur;
 
  dur = (pin >= 0 && pin < 20) ? tdur[pin] : 0;
  tdur[pin] = 0;

  return dur;
}


// handle pin change interrupt for D8 to D13 here
ISR (PCINT0_vect) {
  byte changed;
  const byte offset = 8; // PB0 ==> D8
  
  changed = PINB ^ prevb;
  prevb = PINB;
  
  for (int i = 0; i <= 5; i++) { // PB0..PB5

    byte b = (1<<i);
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
  byte changed; 
  const byte offset = 14; // PC0 ==> D14
  tdur[7] = 4;  
} // PCINT1_vect


// handle pin change interrupt for D0 to D7 here
ISR (PCINT2_vect) {
  const byte offset = 0; // PD0 ==> D0
  byte changed;

  changed = PIND ^ prevd;
  prevd = PIND;
  
  for (int i = 0; i <= 7; i++) { // PD0..PD7

    byte b = (1<<i);
    if (b & changed) {

      if (PIND & b) { // high
        tstart[i+offset] = micros();
      } else {        // low
        tdur[i+offset] = micros() - tstart[i+offset];
      }

    } // if changed

  } // for

} // PCINT2_vect
 

