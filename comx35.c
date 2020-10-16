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
#include <stdlib.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
#include "elf.h"

#define CYCLE 64000
#define VCYCLE 50

char         keyboardValue;
byte         keyboardEF;
int          keyCycles;
byte         lastCode;
byte         qmode;
byte         comx35EF2;
int          vcount;
int next;

byte comx_ef1() {
  return comx35EF2;
  }

byte comx_ef2() {
  if (qmode) {
    return 1;
    } 
  if (comxVidMode == 1) return 0;
  return 1;
  }

byte comxEF() {
  return keyboardEF;
  }

byte comxIn() {
  word ret;
  keyboardEF = 1;
  ret = keyboardValue;
if (ret == 13) {
  ret = 128;
  }
if (ret == 8) {
  ret = 0x86;
  }
  keyboardValue = 0;
  switch (ret) {
    case '-':ret = '|'; break;
    case '=':ret = '-'; break;
    case '^':ret = '='; break;
    case ';':ret = '+'; break;
    case '/':ret = '^'; break;
    case '+':ret = '['; break;
    case '*':ret = ']'; break;
    case '.':ret = ':'; break;
    case ',':ret = ';'; break;
    case '[':ret = '('; break;
    case ']':ret = ')'; break;
    case '(':ret = '<'; break;
    case ')':ret = '>'; break;
    case '?':ret = '@'; break;
    case ':':ret = '*'; break;
    case ' ':ret = '_'; break;
    case '_':ret = '?'; break;
    case '<':ret = ','; break;
    case '>':ret = '.'; break;
    case '\\':ret = '/'; break;
    case 27:ret = 0x81; break;
    case KEY_UP:ret = 0x88; break;
    case KEY_KP_UP:ret = 0x88; break;
    case KEY_DOWN:ret = 0x8a; break;
    case KEY_KP_DOWN:ret = 0x8a; break;
    case KEY_LEFT:ret = 0x8b; break;
    case KEY_KP_LEFT:ret = 0x8b; break;
    case KEY_RIGHT:ret = 0x89; break;
    case KEY_KP_RIGHT:ret = 0x89; break;
/* Not sure if these are needed for windows, or for foreign keyboards */
#ifdef WIN32
    case '@':ret = '"'; break;
    case '-':ret = '|'; break;
    case 39:ret = 35; break;
    case 96:ret = 39; break;
    case 126:ret = ' '; break;
#endif

    }
  return ret & 0xff;
  }

void comxCycle() {
  if (cpu.q) {
    qmode = 1;
    }
  if (--vcount <= 0) {
    comx35EF2 = (comx35EF2 != 0) ? 0 : 1;
    vcount = VCYCLE;
    }
  v1870.cycle --;
  if (v1870.cycle <= 0) {
    v1870.cycle = CYCLE;
    cdp1802Int(&cpu);
    }

  if (keyFile != 0 && keyboardEF == 1 && keyCycles == 0) {
    if (read(keyFile,&keyboard,1) == 0) {
      close(keyFile);
      keyFile = 0;
      }
    keyCycles = 1000;
    if (keyboard == 10) keyboard = 13;
    if (keyboard == 13 && lastCode == 13) keyboard = 0;
    lastCode = keyboard;
    if (keyboard == 13) keyCycles = 130000;
    }
  if (keyCycles > 0 && keyboardEF == 1) keyCycles--;
  if (keyboard<0) keyboard = 0;
  if (keyboard != 0 && (byte)keyboard <= 127) {
    if (forceUC == 'Y' && keyboard >= 'a' && keyboard <= 'z')
      keyboard -= 32;
    keyboardValue = keyboard;
    keyboard = 0;
    keyboardEF = 0;
    }
  }

void configureComx(char* arg) {
  int port;
  portStruct ports;
  ports = parseArgs(arg);
  port = (ports.inPorts[0] == 0) ? 2 : ports.inPorts[0] - 1;
  keyboardEF = 1;
  useKeyboard = 'Y';
  inPorts[port].in = comxIn;
  port = (ports.ef[0] == 0) ? 2 : ports.ef[0] - 1;
  switch (port) {
    case 0:efDevices[0].ef1 = comxEF; break;
    case 1:efDevices[1].ef2 = comxEF; break;
    case 2:efDevices[2].ef3 = comxEF; break;
    case 3:efDevices[3].ef4 = comxEF; break;
    }
  efDevices[0].ef1 = comx_ef1;
  efDevices[1].ef2 = comx_ef2;
  inPorts[port].cycle = comxCycle;
  keyCycles = 500000;
next = 128;
  }

void comx35_event(rcs_Event event) {
  if (event.type == EVENT_BUTTON_PRESS) {
    if (event.d1==3) printf("%d %d\n",event.d2,event.d3);
/* power switch */
    else if (event.d2>=472 && event.d2<=495 &&
        event.d3>=13 && event.d3<=33) {
      toggleSetState(&power,(toggleGetState(&power))?0:1);
      togglePaint(&power,display,window);
      control = OFF;
      }
    }
  }

void comx35_init() {
  int i;
  for (i=0;i<4;i++) {
    addr[i]=til311New();
    til311SetPosition(&addr[i],218+40*i,234);
    }
  for (i=0;i<2;i++) {
    data[i]=til311New();
    til311SetPosition(&data[i],218+40*i,234);
    }
  q=redLedNew();
  qmode = 0;
  redLedSetPosition(&q,324,258);
  toggleSetPosition(&power,470,10);
  toggleSetState(&power,1);
  togglePaint(&power,display,window);
  cdp1802SetClear(&cpu,1);
  cdp1802SetWait(&cpu,1);
  vcount = VCYCLE;
  comx35EF2 = 0;
  }

MACHINE newComx35() {
  MACHINE ret;
  ret.init = comx35_init;
  ret.event = comx35_event;
  return ret;
  }
