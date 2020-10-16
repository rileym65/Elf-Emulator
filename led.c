/*
 *******************************************************************
 *** This software is copyright 2006 by Michael H Riley          ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

#include "elf.h"

void configureLedModule(char* arg) {
  int port;
  portStruct ports;
  ports = parseArgs(arg);
  port = (ports.outPorts[0] == 0) ? 3 : ports.outPorts[0] - 1;
  outPorts[port].out = ledWrite;
  ledModule='Y';
  }

void ledWrite(byte value) {
  int i;
  int states[8];
  for (i=0;i<8;i++) states[i]=led[i].state;
  for (i=0;i<8;i++) {
    led[i].state = (value & 1) ? 1 : 0;
    value>>=1;
    if (led[i].state != states[i]) redLedPaint(&led[i],display,ledWindow);
    }
  }

void ledEvent(rcs_Event event) {
  rcs_GC gc;
  int    i;
  if (event.type == EVENT_EXPOSE) {
    gc=rcs_openGC(display,ledWindow);
    rcs_copyArea(display,ledBuffer,ledWindow,gc,0,0,156,116,0,0);
    rcs_closeGC(display,gc);
    for (i=0;i<8;i++) redLedPaint(&led[i],display,ledWindow);
    }
  }

