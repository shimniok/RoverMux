#include <Servo.h>

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

#define PAUSE 200

Servo rc1, rc2, rc3, mcu1, mcu2, mcu3;

uint16_t dur1 = 0;
uint16_t count1 = 0;

void setup() {

  pinMode(CH1OUT, INPUT);
  pinMode(CH1RC, OUTPUT);
  pinMode(CH1MCU, OUTPUT);
  mcu1.attach(CH1MCU);
  mcu1.write(140);

  pinMode(CH2RC, OUTPUT);
  rc2.attach(CH2RC);
  rc2.write(40);
  pinMode(CH2MCU, OUTPUT);
  mcu2.attach(CH2MCU);
  mcu2.write(140);   
  pinMode(CH2OUT, INPUT);

  pinMode(CH3OUT, INPUT);
  pinMode(CH3RC, OUTPUT);
  digitalWrite(CH3RC, LOW);
  pinMode(CH3MCU, OUTPUT);
  mcu3.attach(CH3MCU);
  mcu3.write(140);
  
  Serial.begin(115200);
}

void loop() {
  // CH1 off, CH3 off  
  rc1.detach();
  pinMode(CH1RC, OUTPUT);
  digitalWrite(CH1RC, LOW);
  rc3.detach();
  pinMode(CH3RC, OUTPUT);
  digitalWrite(CH3RC, LOW);
  delay(PAUSE);
  
  // CH1 on, CH3 off
  rc1.attach(CH1RC);
  rc1.write(40);
  delay(PAUSE);
  Serial.println(dur1);

  // CH1 on, CH3 hi
  rc3.attach(CH3RC);
  rc3.write(110);
  delay(PAUSE);

  // CH1 on, CH3 low  
  rc3.write(70);
  delay(PAUSE);
}

