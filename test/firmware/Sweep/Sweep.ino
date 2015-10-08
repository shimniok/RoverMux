#include <TimerOne.h>
#include <Servo.h> 

// Servo IDs
#define RC1   0
#define MCU1  1
#define RC2   2
#define MCU2  3
#define RC3   4
#define MCU3  5

// Pins
#define CH2OUT  5
#define CH2RC   6
#define CH2MCU  7
#define CH3OUT  2
#define CH3RC   3
#define CH3MCU  4
#define CH1OUT  8
#define CH1RC   9
#define CH1MCU 10

#define PAUSE 200

// Channel check errors
#define CON_NONE  0x00             // Both outputs are low
#define CON_MCU   0x01             // MCU connected
#define CON_RC    0x02             // RC connected
#define CON_BOTH  (CON_MCU|CON_RC) // Both outputs are high
#define CON_ERR   0x04             // Flags a connection error of some kind

Servo ch1;
Servo ch3;
 
void setup() 
{  
  pinMode(CH1OUT, INPUT);
  pinMode(CH2OUT, INPUT);
  pinMode(CH3OUT, INPUT);

  Timer1.initialize(100);  // 100usec
  Timer1.attachInterrupt(servoHandler);
  Timer1.start();

  servoPin(RC1, CH1RC); 
  servoPin(MCU1, CH1MCU);
  servoPin(RC2, CH2RC);
  servoPin(MCU2, CH2MCU);
  servoPin(RC3, CH3RC);
  servoPin(MCU3, CH3MCU); 
  
  Serial.begin(9600);
} 
 
 
void loop() 
{ 
  servoSet(RC1, 15);
  delay(500);
} 

#define MAXSERVO 6
long count=0;
long period[MAXSERVO] = { 15, 15, 15, 15, 15, 15 };
int pin[MAXSERVO] = { CH1RC, CH2RC, CH3RC, CH1MCU, CH2MCU, CH3MCU };
int enabled[MAXSERVO] = { false, false, false, false, false, false };

#define servoValid(s) (s >= 0 && s < MAXSERVO)

/** note: not checking validity of pin */
void servoPin(int servo, int p)
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
void servoSet(int servo, long per)
{
  if (servoValid(servo)) {
    if (per == 0) {
      enabled[servo] = false;
    } else {
      period[servo] = per;
      enabled[servo] = true;
    }
  }
}

void servoHandler()
{
  int i;

  ++count;

  for (i = 0; i < MAXSERVO; i++) {
    if (count > period[i]) {
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

/*
int checkChannel(int mcu, int rc, int output, int desired) 
{
  int result = 0;

  pinMode(mcu, OUTPUT);
  pinMode(rc, OUTPUT);
  pinMode(output, INPUT);
  
  digitalWrite(mcu, HIGH);
  digitalWrite(rc, LOW);
  delay(1);
  if (digitalRead(output) == HIGH) result |= CON_MCU;

  digitalWrite(mcu, LOW);
  digitalWrite(rc, HIGH);
  delay(1);
  if (digitalRead(output) == HIGH) result |= CON_RC;

  digitalWrite(mcu, LOW);
  digitalWrite(rc, LOW);
  delay(1);
  if (digitalRead(output) == HIGH) result |= CON_ERR;

  Serial.print(result);

  return result;
}
*/

