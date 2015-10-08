#define MAXSERVO 6
long count=0;
long duration[MAXSERVO] = { 15, 15, 15, 15, 15, 15 };
byte pin[MAXSERVO] = { CH1RC, CH2RC, CH3RC, CH1MCU, CH2MCU, CH3MCU };
boolean enabled[MAXSERVO] = { false, false, false, false, false, false };

long induration[20]; // duration result from last computation
long incount[20];    // timer for duration of input signal high

#define servoValid(s) (s >= 0 && s < MAXSERVO)

/** note: not checking validity of pin */
void servoPin(byte servo, byte p)
{
  if (servoValid(servo)) {
    pin[servo] = p;
    pinMode(pin[servo], OUTPUT);
    digitalWrite(pin[servo], LOW);
  }
}

/**
 * per -- length of signal in 100's of usec, e.g., 15 for 1500usec (center)
 */
void servoSet(byte servo, long per)
{
  if (servoValid(servo)) {
    if (per == 0) {
      enabled[servo] = false;
    } else {
      duration[servo] = per;
      enabled[servo] = true;
    }
  }
}

void servoHandler()
{
  byte i;

  // Increment timers for input servo signals
  for (i = 0; i < 20; i++) {
    ++incount[i];
  }

  // Output servo signal handling
  ++count;
  for (i = 0; i < MAXSERVO; i++) {
    if (count > duration[i]) {
      digitalWrite(pin[i], LOW);
    }
  }

  if (count > 200) {
    for (i = 0; i < MAXSERVO; i++) {
      digitalWrite(pin[i], enabled[i] ? HIGH : LOW);
    }
    count = 0;
  }

}


long getDuration(byte servo)
{
  return (servo >= 0 && servo < 20) ? induration[servo] : -1;
}

void servoInPin(byte p)
{
  *digitalPinToPCMSK(p) |= bit (digitalPinToPCMSKbit(p));  // enable pin
  PCIFR  |= bit (digitalPinToPCICRbit(p)); // clear any outstanding interrupt
  PCICR  |= bit (digitalPinToPCICRbit(p)); // enable interrupt for the group
}

void countit(byte p)
{
  if (digitalRead(p) == HIGH) {
    incount[p] = 0;
  } else {
    induration[p] = incount[p];
  }
}

ISR (PCINT0_vect) // handle pin change interrupt for D8 to D13 here
{
  for (int i=8; i <= 13; i++) {
    countit(i);
  }
}
 
ISR (PCINT1_vect) // handle pin change interrupt for A0 to A5 here
{
  for (int i=14; i <= 19; i++) {
    countit(i);
  }
}  
 
ISR (PCINT2_vect) // handle pin change interrupt for D0 to D7 here
{
  for (int i=0; i <= 7; i++) {
    countit(i);
  }
} 
