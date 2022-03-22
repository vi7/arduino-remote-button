/**
 *
 * Settings.h
 *
 **/

#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>

#define RX_PIN            PB0
#define OUTPUT_PIN        PB1
#define OUTPUT_ON         LOW  // SSR's have low level trigger
#define OUTPUT_OFF        HIGH
#define DEFAULT_STATE     OUTPUT_ON
#define TOUCH_BUTTON_PIN  PB2  // PB3 is USB+ on Digispark USB, detach it before programming!

/** 32bit message format: 0xXXXXYYZZ
 * XXXX - 16bit preamble
 * YY   - 8bit receiver address
 * ZZ   - 8bit data
 */
#define MESSAGE_ON    0x15550303
#define MESSAGE_OFF   0x1555030C

// Messages for RX/TX debug using real remote control and receiver
// #define MESSAGE1 0x530F0  // ORNO OR-GB-406 remote btn D/timer
// #define MESSAGE2 0x530C3  // ORNO OR-GB-406 remote btn F/OFF

#endif
