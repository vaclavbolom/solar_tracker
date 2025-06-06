#include <Arduino.h>
#include <Stepper.h>

#define POSITIVE_ROUNDS 2
#define NEGATIVE_ROUNDS POSITIVE_ROUNDS * -1
#define STEPS_PER_REV 200
#define SPEED_RPM 100
#define IN1_PIN 11
#define IN2_PIN 10
#define IN3_PIN 9
#define IN4_PIN 8
#define ENA_PIN_A 12
#define ENA_PIN_B 7

int PIN_FORWARD = 6;
int PIN_BACKWARD = 5;
bool move_forward = false;
bool move_backward = false;
bool stop = true;
bool state_forward = false;
bool state_backward = false;
bool state_stop = true;
String state = "stop";
bool state_changed = false;
int my_delay = 1000;

Stepper stepper = Stepper(STEPS_PER_REV, IN1_PIN, IN2_PIN, IN3_PIN, IN4_PIN);
void enableStepper() {
  digitalWrite(ENA_PIN_A, HIGH);
  digitalWrite(ENA_PIN_B, HIGH);
}
void disableStepper() {
  digitalWrite(ENA_PIN_A, LOW);
  digitalWrite(ENA_PIN_B, LOW);
}
void setup() {
  // Set the motor speed (RPMs):
  stepper.setSpeed(SPEED_RPM);
  pinMode(ENA_PIN_A, OUTPUT);
  pinMode(ENA_PIN_B, OUTPUT);
  pinMode(PIN_FORWARD, INPUT);
  pinMode(PIN_BACKWARD, INPUT);
  Serial.begin(9600);
}
void loop() {
  /*
  enableStepper();
  // make several rounds CW
  stepper.step(POSITIVE_ROUNDS * STEPS_PER_REV);
  disableStepper();
  delay(2000);
  enableStepper();
  // make several rounds CCW
  stepper.step(NEGATIVE_ROUNDS * STEPS_PER_REV);
  disableStepper();
  delay(2000);
  */
  state_forward = digitalRead(PIN_FORWARD);
  state_backward = digitalRead(PIN_BACKWARD);
  state_stop = state_backward == state_forward;

  state_changed = false;
  if(state_forward && (state != "forward"))
  {
    state_changed = true;
    state_stop = false;
    state = "forward";
  }
  if (state_backward && (state != "backward"))
  {
    state_changed = true;
    state_stop = false;
    state = "backward";
  }
  if (state_stop && (state != "stop"))
  {
    state_changed = true;
    state_stop = true;
    state = "stop";
  }

  if (state_changed)
  {
    Serial.print("state changed: ");
    Serial.println(state);
    if (state_stop)
    {
      disableStepper();
    }    
  }
  else {
  {
    if (state_forward)
    {
      enableStepper();
      stepper.step(1);
      delay(1);
    }

    if (state_backward)
    {
      enableStepper();
      stepper.step(-1);
    }
  }
  }

  delay(my_delay);

}