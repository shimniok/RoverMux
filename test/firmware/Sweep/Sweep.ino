#include <TimerOne.h>
#include <Servo.h> 

#define CH1OUT  8
#define CH1RC   9
#define CH1MCU 10

#define CH2OUT  5
#define CH2RC   6
#define CH2MCU  7

#define CH3OUT  2
#define CH3RC   3
#define CH3MCU  4

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
  pinMode(CH1MCU, OUTPUT);
  pinMode(CH1RC, OUTPUT);
  pinMode(CH1OUT, INPUT);

  pinMode(CH2MCU, OUTPUT);
  pinMode(CH2RC, OUTPUT);
  pinMode(CH2OUT, INPUT);

  pinMode(CH3MCU, OUTPUT);
  pinMode(CH3RC, OUTPUT);
  pinMode(CH3OUT, INPUT);

  Timer1.initialize(100);  // 100usec
  Timer1.attachInterrupt(servos);
  Timer1.start();
  
  servoOn(0);
  servoOn(3);
  servoOn(4);
  
  Serial.begin(9600);
} 
 
 
void loop() 
{ 

} 

#define MAXSERVO 6
long count=0;
long period[MAXSERVO] = { 15, 15, 15, 15, 15, 15 };
int rc[MAXSERVO] = { CH1RC, CH2RC, CH3RC, CH1MCU, CH2MCU, CH3MCU };
int on[MAXSERVO] = { false, false, false, false, false, false };

#define servoValid(s) (s >= 0 && s < MAXSERVO)

void servoOn(int servo)
{
  if (servoValid(servo)) on[servo] = true;
}

void servoOff(int servo)
{
  if (servoValid(servo)) on[servo] = false;
}

/**
 * per -- usec length of signal, e.g., 1500 (1.5ms, typically center)
 */
void servoSet(int servo, long per)
{
  if (servoValid(servo) && per >= 1000 && per <= 2000) {
    period[servo] = per;
  }
}

void servos()
{
  int i;

  ++count;

  for (i = 0; i < MAXSERVO; i++) {
    if (count > period[i]) {
      digitalWrite(rc[i], LOW);
    }
  }

  if (count > 200) {
    for (i = 0; i < MAXSERVO; i++) {
      digitalWrite(rc[i], on[i] ? HIGH : LOW);
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

