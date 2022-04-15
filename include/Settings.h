/**
 *
 * Settings.h
 *
 **/

#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>

#define RX_PIN            PB0
#define OUTPUT_PIN        PB1  // PB1 is LED_BUILTIN
#define OUTPUT_ON         LOW  // SSR's have low level trigger
#define OUTPUT_OFF        HIGH
#define DEFAULT_STATE     OUTPUT_ON

#ifdef TOUCH_BUTTON
  #define TOUCH_BUTTON_PIN  PB2  // PB3 is USB+ on Digispark USB, detach it before programming!
#endif

/**
 * 32bit message format proposal: 0xXXXXYYZZ
 * XXXX - 16bit preamble
 * YY   - 8bit receiver address
 * ZZ   - 8bit data
 */
// #define MESSAGE_ON    0x15550303
// #define MESSAGE_OFF   0x1555030C

/**
 *  Messages for RX/TX debug using real remote control
 */
// #define MESSAGE_ON  0x530F0  // ORNO OR-GB-406 remote btn D/timer
// #define MESSAGE_OFF 0x530C3  // ORNO OR-GB-406 remote btn F/OFF
// #define MESSAGE_ON  0xC01305 // Cheap LED strip driver RF remote 'Bright+' btn
// #define MESSAGE_OFF 0xC01311  // Cheap LED strip driver RF remote 'Mode-' btn

/**
 * Mimic ORNO OR-AE-13132 power extender messages
 * for compatibility with arduino-grower (https://github.com/vi7/arduino-grower)
 */
#define MESSAGE_ON  0x17703
#define MESSAGE_OFF 0x1770C

#endif
