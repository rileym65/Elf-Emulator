/*
 *******************************************************************
 *** This software is copyright 2006 by Michael H Riley          ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

#include <stdio.h>
#include "elf.h"

#define PS2_CYCLES    20

#ifdef WIN32
#define  KEY_SHIFT_L  60001
#define  KEY_ALT_L    60002
#define  KEY_ALT_R    60003
#define  KEY_CAPS_LK  60004
#define  KEY_KP_UP    60005
#define  KEY_KP_DOWN  60006
#define  KEY_KP_LEFT  60007
#define  KEY_KP_RIGHT 60008
#define  KEY_KP_HOME  60009
#define  KEY_KP_PAGE_UP 60010
#define  KEY_KP_PAGE_DN 60011
#define  KEY_KP_ENTER   60012
#define  KEY_KP_END     60013
#define  KEY_NUM_LK     60014
#endif

char         ps2KValue;
char         ps2CValue;
byte         ps2Value;
byte         ps2Port;
byte         ps2keyboardEF;
int          ps2keyCycle;
byte         ps2Buffer[256];
int          ps2KeyStart;
int          ps2KeyEnd;
char         ps2keyboardMode;
long         ps2Cycles;
byte         ps2Parity;

byte scancodes[] = {   0,       /* 0 */
                       0,       /* 1 */
                       0,       /* 2 */
                       0,       /* 3 */
                       0,       /* 4 */
                       0,       /* 5 */
                       0,       /* 6 */
                       0,       /* 7 */
                       0x66,    /* 8 */
                       0x0d,    /* 9 */
                       0,       /* 10 */
                       0,       /* 11 */
                       0,       /* 12 */
                       0x5a,    /* 13 */
                       0,       /* 14 */
                       0,       /* 15 */
                       0,       /* 16 */
                       0,       /* 17 */
                       0,       /* 18 */
                       0,       /* 19 */
                       0,       /* 20 */
                       0,       /* 21 */
                       0,       /* 22 */
                       0,       /* 23 */
                       0,       /* 24 */
                       0,       /* 25 */
                       0,       /* 26 */
                       0x76,    /* 27 */
                       0,       /* 28 */
                       0,       /* 29 */
                       0,       /* 30 */
                       0,       /* 31 */
                       0x29,    /* 32 */
                       0x16,    /* 33 */
                       0x52,    /* 34 */
                       0x26,    /* 35 */
                       0x25,    /* 36 */
                       0x2e,    /* 37 */
                       0x3d,    /* 38 */
                       0x52,    /* 39 */
                       0x46,    /* 40 */
                       0x45,    /* 41 */
                       0x3e,    /* 42 */
                       0x55,    /* 43 */
                       0x41,    /* 44 */
                       0x4e,    /* 45 */
                       0x49,    /* 46 */
                       0x4a,    /* 47 */
                       0x45,    /* 48 */
                       0x16,    /* 49 */
                       0x1e,    /* 50 */
                       0x26,    /* 51 */
                       0x25,    /* 52 */
                       0x2e,    /* 53 */
                       0x36,    /* 54 */
                       0x3d,    /* 55 */
                       0x3e,    /* 56 */
                       0x46,    /* 57 */
                       0x4c,    /* 58 */
                       0x4c,    /* 59 */
                       0x41,    /* 60 */
                       0x55,    /* 61 */
                       0x49,    /* 62 */
                       0x4a,    /* 63 */
                       0x1e,    /* 64 */
                       0x1c,    /* 65 */
                       0x32,    /* 66 */
                       0x21,    /* 67 */
                       0x23,    /* 68 */
                       0x24,    /* 69 */
                       0x2b,    /* 70 */
                       0x34,    /* 71 */
                       0x33,    /* 72 */
                       0x43,    /* 73 */
                       0x3b,    /* 74 */
                       0x42,    /* 75 */
                       0x4b,    /* 76 */
                       0x3a,    /* 77 */
                       0x31,    /* 78 */
                       0x44,    /* 79 */
                       0x4d,    /* 80 */
                       0x15,    /* 81 */
                       0x2d,    /* 82 */
                       0x1b,    /* 83 */
                       0x2c,    /* 84 */
                       0x3c,    /* 85 */
                       0x2a,    /* 86 */
                       0x1d,    /* 87 */
                       0x22,    /* 88 */
                       0x35,    /* 89 */
                       0x1a,    /* 90 */
                       0x54,    /* 91 */
                       0x5d,    /* 92 */
                       0x5b,    /* 93 */
                       0x36,    /* 94 */
                       0x4e,    /* 95 */
                       0x0e,    /* 96 */
                       0x1c,    /* 97 */
                       0x32,    /* 98 */
                       0x21,    /* 99 */
                       0x23,    /* 100 */
                       0x24,    /* 101 */
                       0x2b,    /* 102 */
                       0x34,    /* 103 */
                       0x33,    /* 104 */
                       0x43,    /* 105 */
                       0x3b,    /* 106 */
                       0x42,    /* 107 */
                       0x4b,    /* 108 */
                       0x3a,    /* 109 */
                       0x31,    /* 110 */
                       0x44,    /* 111 */
                       0x4d,    /* 112 */
                       0x15,    /* 113 */
                       0x2d,    /* 114 */
                       0x1b,    /* 115 */
                       0x2c,    /* 116 */
                       0x3c,    /* 117 */
                       0x2a,    /* 118 */
                       0x1d,    /* 119 */
                       0x22,    /* 120 */
                       0x35,    /* 121 */
                       0x1a,    /* 122 */
                       0x54,    /* 123 */
                       0x5d,    /* 124 */
                       0x5b,    /* 125 */
                       0x0e,    /* 126 */
                       0        /* 127 */
                 };

word getScanCode(word value) {
  if (value < 128) return scancodes[value];
  switch (value) {
    case KEY_F1:return 0x05;
    case KEY_F2:return 0x06;
    case KEY_F3:return 0x04;
    case KEY_F4:return 0x0c;
    case KEY_F5:return 0x03;
    case KEY_F6:return 0x0b;
    case KEY_F7:return 0x83;
    case KEY_F8:return 0x0a;
    case KEY_F9:return 0x01;
    case KEY_F10:return 0x09;
    case KEY_F11:return 0x78;
    case KEY_F12:return 0x07;
    case KEY_BACK_SP:return 0x66;
    case KEY_SHIFT_L: return 0x12;
    case KEY_SHIFT_R: return 0x59;
    case KEY_CTRL_L: return 0x14;
#ifndef WIN32
    case KEY_CTRL_R: return 0xe014;
#endif
    case KEY_ALT_L: return 0x11;
    case KEY_ALT_R: return 0xe011;
    case KEY_TAB:return 0x0d;
    case KEY_CAPS_LK:return 0x58;
    case KEY_UP: return 0xe075;
    case KEY_DOWN: return 0xe072;
    case KEY_LEFT: return 0xe06b;
    case KEY_RIGHT: return 0xe074;
    case KEY_KP_UP: return 0x75;
    case KEY_KP_DOWN: return 0x72;
    case KEY_KP_LEFT: return 0x6b;
    case KEY_KP_RIGHT: return 0x74;
    case KEY_INSERT: return 0xe070;
    case KEY_DELETE: return 0xe071;
    case KEY_HOME: return 0xe06c;
    case KEY_END: return 0xe069;
    case KEY_PAGE_UP: return 0xe07d;
    case KEY_PAGE_DN: return 0xe07a;
    case KEY_KP_0: return 0x70;
    case KEY_KP_1: return 0x69;
    case KEY_KP_2: return 0x72;
    case KEY_KP_3: return 0x7a;
    case KEY_KP_4: return 0x6b;
    case KEY_KP_5: return 0x73;
    case KEY_KP_6: return 0x74;
    case KEY_KP_7: return 0x6c;
    case KEY_KP_8: return 0x75;
    case KEY_KP_9: return 0x7d;
    case KEY_KP_HOME: return 0x6c;
    case KEY_KP_END: return 0x69;
    case KEY_KP_PAGE_UP: return 0x7d;
    case KEY_KP_PAGE_DN: return 0x7a;
    case KEY_KP_ENTER: return 0xe05a;
    case KEY_KP_ADD: return 0x79;
    case KEY_KP_SUB: return 0x7b;
    case KEY_KP_MUL: return 0x7c;
    case KEY_KP_DIV: return 0xe04a;
    case KEY_NUM_LK: return 0x77;
    }
  return 0;
  }

void configurePs2Keyboard(char* arg) {
  int port;
  portStruct ports;
  ps2Intr = 0;
  ports = parseArgs(arg);
  port = (ports.inPorts[0] == 0) ? 6 : ports.inPorts[0] - 1;
  ps2keyboardEF = 1;
  usePS2 = 'Y';
  inPorts[port].in = ps2keyboardIn;
  port = (ports.outPorts[0] == 0) ? 6 : ports.outPorts[0] - 1;
  outPorts[port].out = ps2keyboardOut;
  port = (ports.ef[0] == 0) ? 2 : ports.ef[0] - 1;
  switch (port) {
    case 0:efDevices[0].ef1 = ps2getKeyboardEF; break;
    case 1:efDevices[1].ef2 = ps2getKeyboardEF; break;
    case 2:efDevices[2].ef3 = ps2getKeyboardEF; break;
    case 3:efDevices[3].ef4 = ps2getKeyboardEF; break;
    }
  inPorts[port].cycle = ps2keyboardCycle;
  ps2keyCycle = -1;
  ps2KeyStart = 0;
  ps2KeyEnd = 0;
  ps2keyboardMode = 'X';
  ps2Cycles = cpu.cycles;
  ps2Port = 1;
  ps2CValue = 3;
  ps2KValue = 3;
  if (ports.intr == 1) ps2Intr = 'Y';
  }

void addToBuffer(byte value) {
  if (ps2KeyEnd != ps2KeyStart-1 ||
      (ps2KeyEnd ==255 && ps2KeyStart != 0)) {
    ps2Buffer[ps2KeyEnd++] = value;
    if (ps2KeyEnd == 256) ps2KeyEnd = 0;
    }
  }

void ps2keyboardCycle() {
  word scancode;
  word keycode;
  if (ps2Event.type != 0) {
    if (ps2Event.type == EVENT_KEY_RELEASE) {
      keycode = ps2Event.d1;
      ps2Event.type = 0;
      scancode = getScanCode(keycode);
      if ((scancode >> 8 ) > 0) addToBuffer(scancode>>8);
      addToBuffer(0xf0);
      addToBuffer(scancode & 0xff);
      return;
      }
    if (ps2Event.type == EVENT_KEY_PRESS) {
      keycode = ps2Event.d1;
      ps2Event.type = 0;
      scancode = getScanCode(keycode);
      if ((scancode >> 8 ) > 0) addToBuffer(scancode>>8);
      addToBuffer(scancode & 0xff);
      }
    }
  if (ps2keyboardMode == 'X' && (ps2CValue & 2)) {
    if (ps2KeyStart == ps2KeyEnd) return;
    ps2Value = ps2Buffer[ps2KeyStart++];
    if (ps2KeyStart == 256) ps2KeyStart = 0;
    ps2keyboardMode = 'I';
    ps2Cycles = PS2_CYCLES;
    ps2keyCycle = 0;
    ps2KValue = 2;
    ps2Parity = 1;
    }
  if (ps2keyboardMode == 'I') {
    if ((ps2Cycles -= 1) <= 0) {
      ps2KValue ^= 2;
      if (trace=='Y') {
        if (ps2KValue & 2) debugTrace("PS2 Clock High");
          else debugTrace("PS2 Clock Low");
        }
      if (ps2Intr == 'Y' && (ps2KValue & 2) == 0) cdp1802Int(&cpu);
      ps2Cycles = PS2_CYCLES;
      if (ps2keyCycle >= 11) ps2KValue |= 2;
      if ((ps2KValue & 2) == 2) {
        ps2KValue &= 2;
        if (ps2keyCycle == 0) {
          }
if (ps2keyCycle == 8) printf(" ");
        if (ps2keyCycle < 8) {
          ps2Parity += (ps2Value & 1) ? 1 : 0;
          ps2KValue |= (ps2Value & 1) ? 1 : 0;
          ps2Value >>= 1;
          }
        if (ps2keyCycle == 8) {
          ps2KValue |= (ps2Parity & 1) ? 1 : 0;
          }
        if (ps2keyCycle == 9) {
          ps2KValue |= 1;
          }
        if (ps2keyCycle == 10) {
            ps2keyboardMode = 'R';
            ps2Cycles = PS2_CYCLES;
            ps2KValue = 3;
            }
        ps2keyCycle++;
        }
      }
    }
  if (ps2keyboardMode == 'R') {
    if ((ps2Cycles -= 1) < 0) ps2keyboardMode = 'X';
    }
  if (ps2keyboardMode == 'O') {
    }
  }

void ps2keyboardOut(byte value) {
  ps2CValue = value;
  if ((ps2CValue & 2) == 0) {
    ps2keyboardMode = 'X';
    ps2KValue = 3;
    ps2Cycles = cpu.cycles;
    ps2Port = 1;
    }
  }

byte ps2keyboardIn() {
  byte ret;
  ret = (ps2CValue & 1) ? (ps2KValue & 1) : 0;
  ret |= (ps2CValue & 2) ? (ps2KValue & 2) : 0;
  return ret;
  }

byte ps2getKeyboardEF() {
  return (ps2CValue & 2) ? ((ps2KValue & 2) ? 1 : 0) : 0;
  }

