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
#define CH1OUT  8
#define CH1RC   9
#define CH1MCU 10
#define CH2OUT  5
#define CH2RC   6
#define CH2MCU  7
#define CH3OUT  2
#define CH3RC   3
#define CH3MCU  4

#define MCU1DUR 10
#define MCU2DUR 11
#define MCU3DUR 12
#define RC1DUR  14
#define RC2DUR  15
#define RC3DURL 13
#define RC3DURH 17

#define PAUSE 2000

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
  Timer1.initialize(100);  // 100usec
  Timer1.attachInterrupt(servoHandler);
  Timer1.start();

  servoPin(RC1, CH1RC); 
  servoPin(MCU1, CH1MCU);
  servoPin(RC2, CH2RC);
  servoPin(MCU2, CH2MCU);
  servoPin(RC3, CH3RC);
  servoPin(MCU3, CH3MCU); 

  servoInPin(CH1OUT);
  servoInPin(CH2OUT);
  servoInPin(CH3OUT);
 
  servoSet(MCU1, MCU1DUR);
  servoSet(MCU2, MCU2DUR);
  servoSet(MCU3, MCU3DUR);
    
  Serial.begin(9600);
} 


void displayServos()
{
  Serial.print(" CH1OUT: ");
  Serial.print(getDuration(CH1OUT));
  Serial.print(" CH2OUT: ");
  Serial.print(getDuration(CH2OUT));
  Serial.print(" CH3OUT: ");
  Serial.print(getDuration(CH3OUT));
  Serial.println();
}

boolean checkChannel(long dur, long desired)
{
  boolean result;
  
  if (dur == result) {
    Serial.print("OK ");
  } else {
    Serial.print("BAD ");
  }
  
  return result;
}
 
void loop() 
{ 
  servoSet(RC1, 0);
  servoSet(RC2, 0);
  servoSet(RC3, 0);
  delay(PAUSE);
  displayServos();

  servoSet(RC1, RC1DUR);
  servoSet(RC2, RC2DUR);
  servoSet(RC3, 0);
  delay(PAUSE);
  displayServos();

  servoSet(RC1, RC1DUR);
  servoSet(RC2, RC2DUR);
  servoSet(RC3, RC3DURL);
  delay(PAUSE);
  displayServos();

  servoSet(RC1, RC1DUR);
  servoSet(RC2, RC2DUR);
  servoSet(RC3, RC3DURH);
  delay(PAUSE);
  displayServos();

  Serial.println("----------------------------------");
} 

