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

void super_init() {
  int i;
  for (i=0;i<4;i++) {
    addr[i]=til311New();
    til311SetPosition(&addr[i],300+28*i,140);
    til311Paint(&addr[i],display,window);
    }
  for (i=0;i<2;i++) {
    data[i]=til311New();
    til311SetPosition(&data[i],430+28*i,140);
    til311Paint(&data[i],display,window);
    }
  q=redLedNew();
  redLedSetPosition(&q,280,210);
  redLedSetState(&q,0);
  redLedPaint(&q,display,window);
  for (i=0; i<8; i++) {
    superLeds[i] = redLedNew();
    redLedSetPosition(&superLeds[i],280,233+15*i);
    redLedSetState(&superLeds[i],0);
    redLedPaint(&superLeds[i],display,window);
    }
  power=toggleNew();
  toggleSetPosition(&power,460,40);
  toggleSetState(&power,1);
  togglePaint(&power,display,window);
  mp=toggleNew();
  toggleSetPosition(&mp,0,0);
  toggleSetState(&mp,0);
  }

void super_event(rcs_Event event) {
        if (event.type == EVENT_BUTTON_RELEASE) {
          pressed='N'; 
          if (inPressed=='Y') {
            inPressed='N';
            }
          }
        if (event.type == EVENT_BUTTON_PRESS && pressed=='N') {
          if (event.d1==3) printf("%d %d\n",event.d2,event.d3);
    /* power switch */
          else if (event.d2>=462 && event.d2<=482 &&
              event.d3>=44 && event.d3<=61) {
            toggleSetState(&power,(toggleGetState(&power))?0:1);
            togglePaint(&power,display,window);
            control = OFF;
            }
    /* Load Button */
          else if (event.d2>=435 && event.d2<=462 &&
              event.d3>=234 && event.d3<=259) {
            if (cpu.mode != 'L') showAddr(0); 
            lastMode2 = cpu.mode;
            cdp1802SetClear(&cpu,0);
            cdp1802SetWait(&cpu,0);
            if (lastMode2 == 'R' && cpu.mode != 'R') showClock();
            }
    /* Reset Button */
          else if (event.d2>=435 && event.d2<=462 &&
              event.d3>=265 && event.d3<=291) {
            lastMode2 = cpu.mode;
            cdp1802SetClear(&cpu,0);
            cdp1802SetWait(&cpu,1);
            if (cpu.mode == 'I') showAddr(0);
            if (lastMode2 == 'R' && cpu.mode != 'R') showClock();
            singleStep = 0;
            toggleSetState(&mp,0);
            monitor = 'N';
            state = 'F';
            vt100Reset();
            }
    /* Go Button */
          else if (event.d2>=435 && event.d2<=462 &&
              event.d3>=298 && event.d3<=324) {
            cdp1802SetClear(&cpu,1);
            cdp1802SetWait(&cpu,1);
            }
    /* Pause Button */
          else if (event.d2>=435 && event.d2<=462 &&
              event.d3>=329 && event.d3<=355) {
            cdp1802SetClear(&cpu,1);
            cdp1802SetWait(&cpu,0);
            toggleSetState(&mp,0);
            singleStep = 0;
            }
    /* Single Step Button */
          else if (event.d2>=467 && event.d2<=492 &&
              event.d3>=267 && event.d3<=290) {
/*
            cdp1802SetClear(&cpu,1);
            cdp1802SetWait(&cpu,1);
*/
            singleStep = 1;
            }
    /* Monitor Button */
          else if (event.d2>=467 && event.d2<=492 &&
              event.d3>=234 && event.d3<=259) {
            if (superBoard == 'Y') {
              cdp1802SetClear(&cpu,1);
              cdp1802SetWait(&cpu,1);
              cpu.r[0] = 0x8000;
              } else monitor = 'Y';
            }
    /* 0 Button */
          else if (event.d2>=306 && event.d2<=333 &&
              event.d3>=233 && event.d3<=259) {
            switches = ((switches << 4) & 0xf0) | 0;
            }
    /* 1 Button */
          else if (event.d2>=338 && event.d2<=363 &&
              event.d3>=233 && event.d3<=259) {
            switches = ((switches << 4) & 0xf0) | 1;
            }
    /* 2 Button */
          else if (event.d2>=370 && event.d2<=396 &&
              event.d3>=233 && event.d3<=259) {
            switches = ((switches << 4) & 0xf0) | 2;
            }
    /* 3 Button */
          else if (event.d2>=402 && event.d2<=428 &&
              event.d3>=233 && event.d3<=259) {
            switches = ((switches << 4) & 0xf0) | 3;
            }
    /* 4 Button */
          else if (event.d2>=306 && event.d2<=333 &&
              event.d3>=265 && event.d3<=291) {
            switches = ((switches << 4) & 0xf0) | 4;
            }
    /* 5 Button */
          else if (event.d2>=338 && event.d2<=363 &&
              event.d3>=265 && event.d3<=291) {
            switches = ((switches << 4) & 0xf0) | 5;
            }
    /* 6 Button */
          else if (event.d2>=370 && event.d2<=396 &&
              event.d3>=265 && event.d3<=291) {
            switches = ((switches << 4) & 0xf0) | 6;
            }
    /* 7 Button */
          else if (event.d2>=402 && event.d2<=428 &&
              event.d3>=265 && event.d3<=291) {
            switches = ((switches << 4) & 0xf0) | 7;
            }
    /* 8 Button */
          else if (event.d2>=306 && event.d2<=333 &&
              event.d3>=297 && event.d3<=324) {
            switches = ((switches << 4) & 0xf0) | 8;
            }
    /* 9 Button */
          else if (event.d2>=338 && event.d2<=363 &&
              event.d3>=297 && event.d3<=324) {
            switches = ((switches << 4) & 0xf0) | 9;
            }
    /* A Button */
          else if (event.d2>=370 && event.d2<=396 &&
              event.d3>=297 && event.d3<=324) {
            switches = ((switches << 4) & 0xf0) | 10;
            }
    /* B Button */
          else if (event.d2>=402 && event.d2<=428 &&
              event.d3>=297 && event.d3<=324) {
            switches = ((switches << 4) & 0xf0) | 11;
            }
    /* C Button */
          else if (event.d2>=306 && event.d2<=333 &&
              event.d3>=329 && event.d3<=355) {
            switches = ((switches << 4) & 0xf0) | 12;
            }
    /* D Button */
          else if (event.d2>=338 && event.d2<=363 &&
              event.d3>=329 && event.d3<=355) {
            switches = ((switches << 4) & 0xf0) | 13;
            }
    /* E Button */
          else if (event.d2>=370 && event.d2<=396 &&
              event.d3>=329 && event.d3<=355) {
            switches = ((switches << 4) & 0xf0) | 14;
            }
    /* F Button */
          else if (event.d2>=402 && event.d2<=428 &&
              event.d3>=329 && event.d3<=355) {
            switches = ((switches << 4) & 0xf0) | 15;
            }
    /* P Button */
          else if (event.d2>=467 && event.d2<=493 &&
              event.d3>=297 && event.d3<=323) {
            toggleSetState(&mp,1);
            }
    /* in button */
          else if (event.d2>=467 && event.d2<=492 &&
              event.d3>=329 && event.d3<=355) {
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

MACHINE newSuper() {
  MACHINE ret;
  ret.init = super_init;
  ret.event = super_event;
  return ret;
  }
