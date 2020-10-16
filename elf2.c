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

void elf2_init() {
  int i;
  for (i=0;i<2;i++) {
    data[i]=til311New(); 
    til311SetPosition(&data[i],370+28*i,180);
    til311Paint(&data[i],display,window);
    } 
  power=toggleNew(); 
  toggleSetPosition(&power,490,20);
  toggleSetState(&power,1);
  togglePaint(&power,display,window);
  mp=toggleNew();
  toggleSetPosition(&mp,440,295);
  toggleSetState(&mp,0);
  togglePaint(&mp,display,window);
  load=toggleNew();
  toggleSetPosition(&load,440,265);
  toggleSetState(&load,0);
  togglePaint(&load,display,window);
  run=toggleNew();
  toggleSetPosition(&run,440,235);
  toggleSetState(&run,0);
  togglePaint(&run,display,window);
  q=redLedNew();
  redLedSetPosition(&q,440,190);
  redLedSetState(&q,0);
  redLedPaint(&q,display,window);
  }

void elf2_event(rcs_Event event) {
        if (event.type == EVENT_BUTTON_RELEASE) {
          pressed='N';
          if (inPressed=='Y') {
            inPressed='N';
            cdp1802SetEF(&cpu,4,1);
            }
          }
        if (event.type == EVENT_BUTTON_PRESS && pressed=='N') {
          if (event.d1==3) printf("%d %d\n",event.d2,event.d3);
    /* power switch */
          else if (event.d2>=495 && event.d2<=512 &&
              event.d3>=24 && event.d3<=40) {
            toggleSetState(&power,(toggleGetState(&power))?0:1);
            togglePaint(&power,display,window);
            control = OFF;
            }
    /* mp switch */
          else if (event.d2>=444 && event.d2<=462 &&
            event.d3>=300 && event.d3<=316) {
            toggleSetState(&mp,(toggleGetState(&mp))?0:1);
            togglePaint(&mp,display,window);
            }
    /* load switch */
          else if (event.d2>=444 && event.d2<=462 &&
            event.d3>=271 && event.d3<=288) {
            toggleSetState(&load,(toggleGetState(&load))?0:1);
            togglePaint(&load,display,window);
            cdp1802SetWait(&cpu,(toggleGetState(&load))?0:1);
            if (cpu.mode == 'I') showAddr(0);
            }
    /* run switch */
          else if (event.d2>=444 && event.d2<=462 &&
            event.d3>=241 && event.d3<=257) {
            toggleSetState(&run,(toggleGetState(&run))?0:1);
            togglePaint(&run,display,window);
            cdp1802SetClear(&cpu,(toggleGetState(&run))?1:0);
            if (cpu.mode == 'I') showAddr(0);
            if (cpu.mode != 'R') showClock();
            }
    /* 0 Button */
          else if (event.d2>=305 && event.d2<=332 &&
              event.d3>=329 && event.d3<=355) {
            switches = ((switches << 4) & 0xf0) | 0;
            }
    /* 1 Button */
          else if (event.d2>=338 && event.d2<=364 &&
              event.d3>=329 && event.d3<=355) {
            switches = ((switches << 4) & 0xf0) | 1;
            }
    /* 2 Button */
          else if (event.d2>=371 && event.d2<=397 &&
              event.d3>=329 && event.d3<=355) {
            switches = ((switches << 4) & 0xf0) | 2;
            }
    /* 3 Button */
          else if (event.d2>=403 && event.d2<=428 &&
              event.d3>=329 && event.d3<=355) {
            switches = ((switches << 4) & 0xf0) | 3;
            }
    /* 4 Button */
          else if (event.d2>=305 && event.d2<=332 &&
              event.d3>=297 && event.d3<=324) {
            switches = ((switches << 4) & 0xf0) | 4;
            }
    /* 5 Button */
          else if (event.d2>=338 && event.d2<=364 &&
              event.d3>=297 && event.d3<=324) {
            switches = ((switches << 4) & 0xf0) | 5;
            }
    /* 6 Button */
          else if (event.d2>=371 && event.d2<=397 &&
              event.d3>=297 && event.d3<=324) {
            switches = ((switches << 4) & 0xf0) | 6;
            }
    /* 7 Button */
          else if (event.d2>=403 && event.d2<=428 &&
              event.d3>=297 && event.d3<=324) {
            switches = ((switches << 4) & 0xf0) | 7;
            }
    /* 8 Button */
          else if (event.d2>=305 && event.d2<=332 &&
              event.d3>=265 && event.d3<=291) {
            switches = ((switches << 4) & 0xf0) | 8;
            }
    /* 9 Button */
          else if (event.d2>=338 && event.d2<=364 &&
              event.d3>=265 && event.d3<=291) {
            switches = ((switches << 4) & 0xf0) | 9;
            }
    /* A Button */
          else if (event.d2>=371 && event.d2<=397 &&
              event.d3>=265 && event.d3<=291) {
            switches = ((switches << 4) & 0xf0) | 10;
            }
    /* B Button */
          else if (event.d2>=403 && event.d2<=428 &&
              event.d3>=265 && event.d3<=291) {
            switches = ((switches << 4) & 0xf0) | 11;
            }
    /* C Button */
          else if (event.d2>=305 && event.d2<=332 &&
              event.d3>=234 && event.d3<=260) {
            switches = ((switches << 4) & 0xf0) | 12;
            }
    /* D Button */
          else if (event.d2>=338 && event.d2<=364 &&
              event.d3>=234 && event.d3<=260) {
            switches = ((switches << 4) & 0xf0) | 13;
            }
    /* E Button */
          else if (event.d2>=371 && event.d2<=397 &&
              event.d3>=234 && event.d3<=260) {
            switches = ((switches << 4) & 0xf0) | 14;
            }
    /* F Button */
          else if (event.d2>=403 && event.d2<=428 &&
              event.d3>=234 && event.d3<=260) {
            switches = ((switches << 4) & 0xf0) | 15;
            }
    /* in button */
          else if (event.d2>=436 && event.d2<=465 &&
              event.d3>=329 && event.d3<=357) {
            inPressed='Y';
            if (cpu.mode == 'L') {
              cdp1802DmaIn(&cpu,getData());
              showData(getData());
              } else {
              toggleSetState(&ef[3],0);
              }
            }
        }

  }

MACHINE newElf2() {
  MACHINE ret;
  ret.init = elf2_init;
  ret.event = elf2_event;
  return ret;
  }
