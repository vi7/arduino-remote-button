#include <Arduino.h>
#include <RCSwitch.h>
#include <avr/sleep.h>

#include "Settings.h"

RCSwitch receiver = RCSwitch();
volatile uint8_t lastButtonState = LOW;
volatile uint8_t currentButtonState = LOW;

/*
 * Serial TX on PB2 (TX only!)
 */
void setupSerial() {
  Serial.begin(9600);
  Serial.println("\nDigispark USB is up. Hey there!");
}

void setupPins() {
  pinMode(OUTPUT_PIN, OUTPUT);
  digitalWrite(OUTPUT_PIN, DEFAULT_STATE);
}

void setupInterrupts() {
  noInterrupts();
  bitClear(GIMSK,
           INT0);  // disable INT0 external interrupt on PB2 - this is optional
  bitSet(GIMSK, PCIE);  // enable pin change interrupt
  // activate interrupts for the specific pins
  bitSet(PCMSK, TOUCH_BUTTON_PIN);
  bitSet(PCMSK, RX_PIN);
  interrupts();
}

void cpuSleep() {
  sleep_enable();

  // Do not interrupt before we go to sleep, or the
  // ISR will detach interrupts and we won't wake.
  noInterrupts();

  // We are guaranteed that the sleep_cpu call will be done
  // as the processor executes the next instruction after
  // interrupts are turned on.
  interrupts();  // one cycle
  sleep_cpu();   // one cycle
}

void powerOn() {
  digitalWrite(OUTPUT_PIN, OUTPUT_ON);
}

void powerOff() {
  digitalWrite(OUTPUT_PIN, OUTPUT_OFF);
}

void onTouch() { digitalRead(OUTPUT_PIN) ? powerOn() : powerOff(); }

void parseMessage() {
  switch (receiver.getReceivedValue()) {
    case MESSAGE_ON:
      powerOn();
      break;

    case MESSAGE_OFF:
      powerOff();
      break;

    default:
      break;
  }
}

/**********
 *  MAIN  *
 **********/

void setup() {
  // setupSerial();  // <- uncomment to use Serial for debugging
  setupPins();
  setupInterrupts();
}

void loop() {
  cpuSleep();
}

/*********************
 * Interrupt handler *
 *********************/
ISR(PCINT0_vect) {
  // Button interrupt handling
  currentButtonState = digitalRead(TOUCH_BUTTON_PIN);
  if (currentButtonState && !lastButtonState) {  // Button pressed
    onTouch();
    lastButtonState = currentButtonState;
    return;
  } else if (!currentButtonState && lastButtonState) {  // Button released
    lastButtonState = LOW;
    return;
  }

  // Radio receiver interrupt handling
  RCSwitch::handleInterrupt();
  if (receiver.available()) {
    parseMessage();
    receiver.resetAvailable();
  }
}
