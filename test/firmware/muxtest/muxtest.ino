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

#define PAUSE1 200
#define PAUSE2 3000

#define CON_MCU 0x01
#define CON_RC  0x02
#define CON_NC  0x03
#define CON_ERR 0x04

Servo rc1, rc2, rc3, mcu1, mcu2, mcu3;

uint16_t dur1 = 0;
uint16_t count1 = 0;

void setup() {

  pinMode(CH1OUT, INPUT);
  pinMode(CH1RC, OUTPUT);
  pinMode(CH1MCU, OUTPUT);
  mcu1.attach(CH1MCU);
  mcu1.write(110);
  servoTimer(CH1OUT);
  servoTimer(CH1RC);
  servoTimer(CH1MCU);

  pinMode(CH2RC, OUTPUT);
  rc2.attach(CH2RC);
  rc2.write(50);
  pinMode(CH2MCU, OUTPUT);
  mcu2.attach(CH2MCU);
  mcu2.write(130);   
  pinMode(CH2OUT, INPUT);
  servoTimer(CH2OUT);
  servoTimer(CH2RC);
  servoTimer(CH2MCU);

  pinMode(CH3OUT, INPUT);
  pinMode(CH3RC, OUTPUT);
  digitalWrite(CH3RC, LOW);
  pinMode(CH3MCU, OUTPUT);
  mcu3.attach(CH3MCU);
  mcu3.write(140);
  servoTimer(CH3OUT);
  servoTimer(CH3RC);
  servoTimer(CH3MCU);
    
  Serial.begin(115200);
}


byte test_state = 0;

void loop() {

  // CH1 off, CH3 off  
  Serial.println("\n----------------");
  Serial.println("CH1=off, CH3=off\n");
  rc1.detach();
  pinMode(CH1RC, OUTPUT);
  digitalWrite(CH1RC, LOW);
  rc3.detach();
  pinMode(CH3RC, OUTPUT);
  digitalWrite(CH3RC, LOW);
  delay(PAUSE1);
  checkChannel(CH1MCU, CH1RC, CH1OUT, CON_MCU, "CH1");
  checkChannel(CH2MCU, CH2RC, CH2OUT, CON_MCU, "CH2");
  checkChannel(CH3MCU, CH3RC, CH3OUT, CON_MCU, "CH3");
  delay(PAUSE2);

  // CH1 on, CH3 off
  Serial.println("\n----------------");
  Serial.println("CH1=on, CH3=off\n");
  rc1.attach(CH1RC);
  rc1.write(60);
  delay(PAUSE1);
  checkChannel(CH1MCU, CH1RC, CH1OUT, CON_RC, "CH1");
  checkChannel(CH2MCU, CH2RC, CH2OUT, CON_RC, "CH2");
  checkChannel(CH3MCU, CH3RC, CH3OUT, CON_NC, "CH3");
  delay(PAUSE2);

  // CH1 on, CH3 hi
  Serial.println("\n----------------");
  Serial.println("CH1=on, CH3=hi\n");
  rc3.attach(CH3RC);
  rc3.write(110);
  delay(PAUSE1);
  checkChannel(CH1MCU, CH1RC, CH1OUT, CON_RC, "CH1");
  checkChannel(CH2MCU, CH2RC, CH2OUT, CON_RC, "CH2");
  checkChannel(CH3MCU, CH3RC, CH3OUT, CON_RC, "CH3");
  delay(PAUSE2);

  // CH1 on, CH3 low  
  Serial.println("\n----------------");
  Serial.println("CH1=on, CH3=lo\n");
  rc3.write(70);
  delay(PAUSE1);
  checkChannel(CH1MCU, CH1RC, CH1OUT, CON_MCU, "CH1");
  checkChannel(CH2MCU, CH2RC, CH2OUT, CON_MCU, "CH2");
  checkChannel(CH3MCU, CH3RC, CH3OUT, CON_MCU, "CH3");
  delay(PAUSE2);

  Serial.println();
  Serial.println();
}


byte checkChannel(byte mcupin, byte rcpin, byte outpin, byte desired, const char *s) {
  byte result;
  const uint16_t tol = 10; // error tolerance, +/-
  uint16_t m, r, o;        //durations for mcu, r/c, out

  m = getDuration(mcupin);
  r = getDuration(rcpin);
  o = getDuration(outpin);
  
  Serial.print(s);
  Serial.print(":");
  
  if (o == 0) {
    result = CON_NC;
  } else if (m > o-tol && m < o+tol) {
    result = CON_MCU;
  } else if (r > o-tol && r < o+tol) {
    result = CON_RC;
  } else {
    result = CON_ERR;
  }

  if (result == desired) {
    Serial.print("AOK ");
  } else if (result == CON_NC) {
    Serial.print("N/C ");
  } else {
    Serial.print("ERR ");
  }
  
  Serial.print(" r=");
  Serial.print(r);
  Serial.print(" m=");
  Serial.print(m);
  Serial.print(" o=");
  Serial.print(o);
  Serial.println();
  
  return result;
}

