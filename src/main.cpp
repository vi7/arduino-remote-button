#include <Arduino.h>
#include <RCSwitch.h>
#include <avr/sleep.h>

#define RX_PIN            PB0
#define OUTPUT_PIN        PB1
#define TOUCH_BUTTON_PIN  PB2  // PB3 is USB+ on Digispark USB, detach it before programming!

/** 32bit message format: 0xXXXXYYZZ
 * XXXX - 16bit preamble
 * YY   - 8bit receiver address
 * ZZ   - 8bit data
 */
#define MESSAGE_ON    0x3C300303
#define MESSAGE_OFF   0x3C300305

// Messages for RX/TX debug using real remote control and receiver
// #define MESSAGE1 0x530F0  // ORNO OR-GB-406 remote btn D/timer
// #define MESSAGE2 0x530C3  // ORNO OR-GB-406 remote btn F/OFF

RCSwitch receiver = RCSwitch();
volatile uint8_t lastButtonState = LOW;
volatile uint8_t currentButtonState = LOW;

// enables software Serial with TX on PB2 (TX only)
void setupSerial() {
  Serial.begin(9600);
  Serial.println("\nDigispark USB is up. Hey there!");
}

void setupPins() {
  // TODO: check if this fixes behavior of the "USB pins" (PB3, PB4)
  // pinMode(TOUCH_BUTTON_PIN, INPUT);
  // digitalWrite(TOUCH_BUTTON_PIN, LOW);

  pinMode(OUTPUT_PIN, OUTPUT);
  // TODO: set to LOW after debugging
  digitalWrite(OUTPUT_PIN, HIGH);
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
  digitalWrite(OUTPUT_PIN, HIGH);
}

void powerOff() {
  digitalWrite(OUTPUT_PIN, LOW);
}

void onTouch() { digitalRead(OUTPUT_PIN) ? powerOff() : powerOn(); }

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
  // setupSerial();  // uncomment to use Serial for debugging
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
