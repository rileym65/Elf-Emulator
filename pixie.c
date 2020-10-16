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

byte pbacking[64][128];
byte vidInt;
byte vidCycle;

void configurePixie(char* arg) {
  int x,y;
  int port;
  portStruct ports;
  ports = parseArgs(arg);
  port = (ports.outPorts[0] == 0) ? 0 : ports.outPorts[0] - 1;
  outPorts[port].out = pixieOut;
  outPorts[port].cycle = pixieCycle;
  port = (ports.inPorts[0] == 0) ? 0 : ports.inPorts[0] - 1;
  inPorts[port].in = pixieIn;
  port = (ports.ef[0] == 0) ? 0 : ports.ef[0] - 1;
  switch (port) {
    case 0:efDevices[0].ef1 = getPixieEF; break;
    case 1:efDevices[1].ef2 = getPixieEF; break;
    case 2:efDevices[2].ef3 = getPixieEF; break;
    case 3:efDevices[3].ef4 = getPixieEF; break;
    }
  graphics = 'Y';
  graphicsOn = 'N';
  pixieEF = 1;
  for (x=0; x<64; x++) for(y=0; y<128; y++) pbacking[x][y] = 255;
  graphicsNext = 0;
  graphicsMode = 0;
  vidInt = 0;
  vidCycle = 0;
  }

void pixieCycle() {
  int i,j;
  byte v;
  if (graphicsNext == 0) {
    if (trace=='Y') debugTrace("H.Sync");
    graphicsMode++;
    if (graphicsMode == 60) pixieEF = 0;
    if (graphicsMode == 64) pixieEF = 1;
    if (graphicsMode == 188) pixieEF = 0;
    if (graphicsMode == 192) pixieEF = 1;
    if (graphicsMode >= 262) graphicsMode = 0;
    }
  if (graphicsNext == 2) {
    if (graphicsMode == 62) {
      if (graphicsOn == 'Y') {
        cdp1802Int(&cpu);
        vidInt = 1;
        cycle0 = 1;
        } else vidInt = 0;
      }
    }
  if (graphicsMode >= 64 && graphicsMode <=191 && graphicsOn == 'Y' &&
     vidInt == 1 && graphicsNext >=4 && graphicsNext <=11) {
    j = 0;
    while (graphicsNext >= 4 && graphicsNext <= 11) {
      graphicsNext ++;
      v = cdp1802DmaOut(&cpu);
      for (i=0; i<8; i++) {
        pixiePlot(j+i,graphicsMode-64,(v & 128) ? 1 : 0);
        v <<= 1;
        }
      j += 8;
      }
    cycle0 = 1;
    graphicsNext -= 1;
    } 
  graphicsNext += 1;
  if (graphicsNext > 13) graphicsNext = 0;
  }

void pixieOut(byte value) {
  graphicsOn = 'N';
  }

byte pixieIn() {
  graphicsOn = 'Y';
  return 0;
  }

byte getPixieEF() {
  return pixieEF;
  }

void pixiePlot(int x,int y,int c) {
  if (pbacking[x][y] == c) return;
  pbacking[x][y] = c;
  x <<= 2;
  y <<= 1;
#ifdef X
  if (c) XFillRectangle(display,pixieWindow,white,x*vScale,y*vScale,4*vScale,2*vScale);
    else XFillRectangle(display,pixieWindow,black,x*vScale,y*vScale,4*vScale,2*vScale);
#else
  if (c) rcs_drawFilledBox(display,pixieWindow,white,x*vScale,y*vScale,4*vScale,2*vScale);
    else rcs_drawFilledBox(display,pixieWindow,black,x*vScale,y*vScale,4*vScale,2*vScale);
#endif
  }

void pixieEvent(rcs_Event event) {
  int x,y;
  if (event.type == EVENT_EXPOSE) {
    for (x=0; x<64; x++) for(y=0; y<128; y++) pbacking[x][y] = 255;
    }
  }

