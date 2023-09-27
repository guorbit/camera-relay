#include <Arduino.h>
#include "Serial.h"

#define GPIO_PIN_IDLE 12
#define GPIO_PIN_CAPTURE 11
#define GPIO_PIN_USB_TOGGLE 10
#define PWM_PIN 0

#define IDLE_PULSE_WIDTH_US 1000
#define CAPTURE_PULSE_WIDTH_US 2000
#define USB_TOGGLE_PULSE_WIDTH_US 1500
#define DOWNTIME_SHORT_MS 100
#define DOWNTIME_LONG_MS 2000

bool do_pwm = true;
int pulse_counter = 0;
int current_pulse_width_us = IDLE_PULSE_WIDTH_US;

void send_idle_signal()
{
  do_pwm = true;
  pulse_counter = 0;
  current_pulse_width_us = IDLE_PULSE_WIDTH_US;
}

void send_capture_pulse()
{
  do_pwm = false;
  pulse_counter = 0;
  current_pulse_width_us = CAPTURE_PULSE_WIDTH_US;
}

void send_usb_toggle_pulse()
{
  do_pwm = false;
  pulse_counter = 0;
  current_pulse_width_us = USB_TOGGLE_PULSE_WIDTH_US;
}

void setup() {
  Serial.begin(9600);

  pinMode(GPIO_PIN_IDLE, INPUT);
  pinMode(GPIO_PIN_CAPTURE, INPUT);
  pinMode(GPIO_PIN_USB_TOGGLE, INPUT);
  pinMode(PWM_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(GPIO_PIN_IDLE), send_idle_signal, CHANGE);
  attachInterrupt(digitalPinToInterrupt(GPIO_PIN_CAPTURE), send_capture_pulse, CHANGE);
  attachInterrupt(digitalPinToInterrupt(GPIO_PIN_USB_TOGGLE), send_usb_toggle_pulse, CHANGE);
}

void loop() {
  if (Serial.available())
  {
    int c = Serial.read();
    if (c == 'i')
    {
      do_pwm = true;
      current_pulse_width_us = IDLE_PULSE_WIDTH_US;
      pulse_counter = 0;
    }
    else if (c == 'c')
    {
      do_pwm = false;
      current_pulse_width_us = CAPTURE_PULSE_WIDTH_US;
      pulse_counter = 0;
    }
    else if (c == 'p')
    {
      do_pwm = false;
      current_pulse_width_us = USB_TOGGLE_PULSE_WIDTH_US;
      pulse_counter = 0;
    }
    else
    {
      Serial.println("invalid input for serial mode override");
    }
  }

  if (do_pwm || pulse_counter < 1)
  {
    Serial.print("do_pwm: ");
    Serial.println(do_pwm);
    Serial.print("pulse counter: ");
    Serial.println(pulse_counter);
    Serial.print("current_pulse_width_us: ");
    Serial.println(current_pulse_width_us);

    digitalWrite(PWM_PIN, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    delayMicroseconds(current_pulse_width_us);
    digitalWrite(PWM_PIN, LOW);
    digitalWrite(LED_BUILTIN, LOW);
    delay(DOWNTIME_SHORT_MS);

    digitalWrite(PWM_PIN, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    delayMicroseconds(IDLE_PULSE_WIDTH_US);
    digitalWrite(PWM_PIN, LOW);
    digitalWrite(LED_BUILTIN, LOW);
    
    pulse_counter++;
  }

  delay(DOWNTIME_LONG_MS);
}
