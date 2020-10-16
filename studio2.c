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
#include "elf.h"

byte         studioKeys[2][10];
int          studioPort;
int          studioBank;
int          studioKey;

void configurePortStudio2() {
  int i,j;
  FILE* romFile;
  outPorts[1].out = studio2Out;
  outPorts[0].out = pixieOut;
  studioPort = 0;
  efDevices[2].ef3 = studio2Ef3;
  efDevices[3].ef4 = studio2Ef4;
  romFile = fopen("studio2.rom","rb");
  if (romFile == NULL) {
    printf("Could not read studio2.rom\n");
    exit(1);
    }
  fread(ram,1,8192,romFile);
  fclose(romFile);

  studioBank = -1;
  studioKey = -1;
  for (j=0; j<2; j++) for (i=0; i<10; i++) studioKeys[j][i] = 0;
  }

void studio2key(rcs_Event event) {
  if (event.type == EVENT_BUTTON_RELEASE && event.d1 == 1) {
    if (studioBank >= 0) {
      studioKeys[studioBank][studioKey] = 0;
      }
    }
  if (event.type == EVENT_BUTTON_PRESS && event.d1 == 3)
          if (event.d1==3) printf("%d %d\n",event.d2,event.d3);

  if (event.type == EVENT_BUTTON_PRESS && event.d1 == 1) {
    studioBank = -1;
    studioKey = -1;
    if (event.d2 >= 197 && event.d2 <= 228 &&
        event.d3 >= 171 && event.d3 <=185) {
      cdp1802SetWait(&cpu,1);
      cdp1802SetClear(&cpu,0);
      cdp1802SetWait(&cpu,1);
      cdp1802SetClear(&cpu,1);
      cpu.cycles = 0;
      graphics='N';
      graphicsMode = 0;
      graphicsNext = cpu.cycles + 14;
      startTime = time(NULL);
      }

    if (event.d2 >= 44 && event.d2 <= 63 &&
        event.d3 >= 63 && event.d3 <=84) { studioBank = 0; studioKey = 1; }
    if (event.d2 >= 67 && event.d2 <= 86 &&
        event.d3 >= 63 && event.d3 <=84) { studioBank = 0; studioKey = 2; }
    if (event.d2 >= 90 && event.d2 <= 110 &&
        event.d3 >= 63 && event.d3 <=84) { studioBank = 0; studioKey = 3; }
    if (event.d2 >= 44 && event.d2 <= 63 &&
        event.d3 >= 92 && event.d3 <=113) { studioBank = 0; studioKey = 4; }
    if (event.d2 >= 67 && event.d2 <= 86 &&
        event.d3 >= 92 && event.d3 <=113) { studioBank = 0; studioKey = 5; }
    if (event.d2 >= 90 && event.d2 <= 110 &&
        event.d3 >= 92 && event.d3 <=113) { studioBank = 0; studioKey = 6; }
    if (event.d2 >= 44 && event.d2 <= 63 &&
        event.d3 >= 120 && event.d3 <=142) { studioBank = 0; studioKey = 7; }
    if (event.d2 >= 67 && event.d2 <= 86 &&
        event.d3 >= 120 && event.d3 <=142) { studioBank = 0; studioKey = 8; }
    if (event.d2 >= 90 && event.d2 <= 110 &&
        event.d3 >= 120 && event.d3 <=142) { studioBank = 0; studioKey = 9; }
    if (event.d2 >= 67 && event.d2 <= 86 &&
        event.d3 >= 149 && event.d3 <=172) { studioBank = 0; studioKey = 0; }

    if (event.d2 >= 313 && event.d2 <= 332 &&
        event.d3 >= 63 && event.d3 <=84) { studioBank = 1; studioKey = 1; }
    if (event.d2 >= 336 && event.d2 <= 356 &&
        event.d3 >= 63 && event.d3 <=84) { studioBank = 1; studioKey = 2; }
    if (event.d2 >= 359 && event.d2 <= 379 &&
        event.d3 >= 63 && event.d3 <=84) { studioBank = 1; studioKey = 3; }
    if (event.d2 >= 313 && event.d2 <= 332 &&
        event.d3 >= 92 && event.d3 <=113) { studioBank = 1; studioKey = 4; }
    if (event.d2 >= 336 && event.d2 <= 356 &&
        event.d3 >= 92 && event.d3 <=113) { studioBank = 1; studioKey = 5; }
    if (event.d2 >= 359 && event.d2 <= 379 &&
        event.d3 >= 92 && event.d3 <=113) { studioBank = 1; studioKey = 6; }
    if (event.d2 >= 313 && event.d2 <= 332 &&
        event.d3 >= 120 && event.d3 <=142) { studioBank = 1; studioKey = 7; }
    if (event.d2 >= 336 && event.d2 <= 356 &&
        event.d3 >= 120 && event.d3 <=142) { studioBank = 1; studioKey = 8; }
    if (event.d2 >= 359 && event.d2 <= 379 &&
        event.d3 >= 120 && event.d3 <=142) { studioBank = 1; studioKey = 9; }
    if (event.d2 >= 336 && event.d2 <= 356 &&
        event.d3 >= 149 && event.d3 <=172) { studioBank = 1; studioKey = 0; }
    if (studioBank != -1) {
      studioKeys[studioBank][studioKey] = 1;
      }
    }
  }

void studio2Out(byte value) {
  while (value >= 10) value -= 10;
  studioPort = value;
  }

byte studio2In() {
  return 255;
  }

byte studio2Ef3() {
  if (studioPort<0 || studioPort>9) return 0;
  return (studioKeys[0][studioPort]) ? 0 : 1;
  }

byte studio2Ef4() {
  if (studioPort<0 || studioPort>9) return 0;
  return (studioKeys[1][studioPort]) ? 0 : 1;
  }

void studio2_init() {
  }

void studio2_event(rcs_Event event) {
  studio2key(event);
  }

MACHINE newStudio2() {
  MACHINE ret;
  ret.init = studio2_init;
  ret.event = studio2_event;
  return ret;
  }

