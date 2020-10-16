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

void clockEvent(rcs_Event event) {
  rcs_GC gc;
  if (event.type == EVENT_EXPOSE) {
    gc=rcs_openGC(display,clockWindow);
    rcs_copyArea(display,clockBuffer,clockWindow,gc,0,0,260,170,0,0);
    rcs_closeGC(display,gc);
    }
  if (event.type == EVENT_BUTTON_PRESS) {
    rcs_hideWindow(display,clockWindow);
    clockOpen = 'N';
    }
  }

void showClock() {
  rcs_GC gc;
  int h,m,s;
  double f1,f2;
  char buffer[128];
  if (cpu.cycles != 0) {
    endTime = time(NULL);
    s = endTime - startTime;
    h = s / 3600;
    s -= (h * 3600);
    m = s / 60;
    s -= (m * 60);
    f2 = endTime - startTime;
    f1 = cpu.cycles;
    f1 /= f2;
    f1 = f1 / 1000000 * 8;
    gc=rcs_openGC(display,clockWindow);
    rcs_drawFilledBox(display,clockBuffer,white,0,0,240,70);
#ifdef WIN32
    sprintf(buffer,"Cycles executed: %ld",cpu.cycles);
#else
    sprintf(buffer,"Cycles executed: %lld",cpu.cycles);
#endif
    rcs_drawString(display,clockBuffer,gc,10,20,buffer);
    sprintf(buffer,"Run Time %2d:%2d:%2d",h,m,s);
    rcs_drawString(display,clockBuffer,gc,10,40,buffer);
    sprintf(buffer,"Effective clock=%12.6f Mhz",f1);
    rcs_drawString(display,clockBuffer,gc,10,60,buffer);
    rcs_copyArea(display,clockBuffer,clockWindow,gc,0,0,260,70,0,0);
    rcs_closeGC(display,gc);
    }
  }

