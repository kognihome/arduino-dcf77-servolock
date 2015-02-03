#include "DCF77.h"
#include "Time.h"
#include <Servo.h> 

#define DCF_PIN 2
#define DCF_INTERRUPT 0
#define LOCK_CLOSED 2
#define LOCK_OPENED 179
#define LOCK_PIN 9
#define LOCK_SWITCH 8
#define LOCK_DELAY 1500
#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

time_t time;
DCF77 DCF = DCF77(DCF_PIN,DCF_INTERRUPT);
int pins[3] = {LED_RED, LED_GREEN, LED_BLUE};
boolean timeUpdated = false;
Servo servoLock;

int servoPos = 0;

void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LOCK_SWITCH, OUTPUT);
  pinMode(DCF_PIN, INPUT);
  digitalWrite(LOCK_SWITCH, LOW);
  Serial.begin(9600);
  spinServo(LOCK_CLOSED);
  DCF.Start();
}

void loop() {
  delay(1000);
  if (!timeUpdated) {
      time_t DCFtime = DCF.getTime();
      if (DCFtime!=0) {
        setTime(DCFtime);
        Serial.println(minute());
        timeUpdated = true;
        digitalWrite(LED_BLUE, LOW);
        DCF.Stop();
      }
  }
  checkTime();
}

void checkTime(){
  //Serial.println(minute());
  if (timeUpdated) {
    if (minute() > 25) {
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, HIGH);
      spinServo(LOCK_OPENED);
    } else {
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, LOW);
      spinServo(LOCK_CLOSED);
    }
  }
}

void spinServo(int value) {
  if (servoPos != value) {
    digitalWrite(LOCK_SWITCH, HIGH);
    servoLock.attach(LOCK_PIN);
    servoLock.write(value);
    delay(LOCK_DELAY);
    servoLock.detach();
    digitalWrite(LOCK_SWITCH, LOW);
    servoPos = value;
  }
}
