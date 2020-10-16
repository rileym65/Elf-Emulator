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

void hbelf_init() {
  int i;
  for (i=0;i<4;i++) {
    addr[i]=til311New();
    til311SetPosition(&addr[i],18+40*i,234);
    til311Paint(&addr[i],display,window);
    }
  for (i=0;i<2;i++) {
    data[i]=til311New();
    til311SetPosition(&data[i],218+40*i,234);
    til311Paint(&data[i],display,window);
    }
  for (i=0;i<8;i++) {
    datasw[i]=toggleNew();
    toggleSetPosition(&datasw[i],18+30*(7-i),374);
    toggleSetState(&datasw[i],0);
    togglePaint(&datasw[i],display,window);
    }
  for (i=0;i<4;i++) {
    ef[i]=toggleNew();
    toggleSetPosition(&ef[i],138+30*(3-i),438);
    toggleSetState(&ef[i],0);
    togglePaint(&ef[i],display,window);
    }
  mp=toggleNew();
  toggleSetPosition(&mp,150,320);
  toggleSetState(&mp,0);
  togglePaint(&mp,display,window);
  run=toggleNew();
  toggleSetPosition(&run,190,320);
  toggleSetState(&run,0);
  togglePaint(&run,display,window);
  load=toggleNew();
  toggleSetPosition(&load,75,320);
  toggleSetState(&load,0);
  togglePaint(&load,display,window);
  dmain=pushNew();
  pushSetPosition(&dmain,33,320);
  pushSetState(&dmain,0);
  pushPaint(&dmain,display,window);
  q=redLedNew();
  redLedSetPosition(&q,324,258);
  redLedSetState(&q,0);
  redLedPaint(&q,display,window);
  power=toggleNew();
  toggleSetPosition(&power,313,320);
  toggleSetState(&power,1);
  togglePaint(&power,display,window);
  }

void hbelf_event(rcs_Event event) {
  int i;
        if (event.type == EVENT_BUTTON_RELEASE) {
          pressed='N';
    /* in button */
          if (inPressed=='Y') {
            pushSetState(&dmain,0);
            pushPaint(&dmain,display,window);
            paint(38,314,55,354);
            inPressed='N';
            toggleSetState(&ef[3],0);
            }
          }
        if (event.type == EVENT_BUTTON_PRESS && pressed=='N') {
          pressed='Y';
          if (event.d1==3) printf("%d %d\n",event.d2,event.d3);
          else if (event.d1==1) {
    /* data switches */
            if (event.d3>=376 && event.d3<=399) {
              for (i=0;i<8;i++) if (event.d2>=22+(i*30) && event.d2<=41+(i*30)) {
                toggleSetState(&datasw[7-i],(toggleGetState(&datasw[7-i]))?0:1);
                togglePaint(&datasw[7-i],display,window);
                paint(22+(i*30),370,41+(i*30),409);
                }
              }
    /* ef switches */
            if (event.d3>=442 && event.d3<=463) {
              for (i=0;i<4;i++) if (event.d2>=141+(i*30) && event.d2<=160+(i*30)) {
                toggleSetState(&ef[3-i],(toggleGetState(&ef[3-i]))?0:1);
                togglePaint(&ef[3-i],display,window);
                paint(141+(i*30),432,160+(i*30),473);
                cdp1802SetEF(&cpu,4-i,toggleGetState(&ef[3-i]));
                }
              }
    /* control switches */
            if (event.d3>=324 && event.d3<=344) {
    /* in button */
              if (event.d2>=38 && event.d2<=55) {
                pushSetState(&dmain,1);
                pushPaint(&dmain,display,window);
                paint(38,314,55,354);
                inPressed='Y';
                if (cpu.mode == 'L') {
                  cdp1802DmaIn(&cpu,getData());
                  showData(getData());
                  } else {
                  toggleSetState(&ef[3],1);
                  }
                }
    /* load switch */
              if (event.d2>=79 && event.d2<=97) {
                toggleSetState(&load,(toggleGetState(&load))?0:1);
                togglePaint(&load,display,window);
                paint(79,314,97,354);
                cdp1802SetWait(&cpu,(toggleGetState(&load))?0:1);
                if (cpu.mode == 'I') showAddr(0);
                }
    /* mp switch */
              if (event.d2>=154 && event.d2<=173) {
                toggleSetState(&mp,(toggleGetState(&mp))?0:1);
                togglePaint(&mp,display,window);
                paint(154,314,173,354);
                }
    /* run switch */
              if (event.d2>=195 && event.d2<=213) {
                toggleSetState(&run,(toggleGetState(&run))?0:1);
                togglePaint(&run,display,window);
                paint(195,314,213,354);
                cdp1802SetClear(&cpu,(toggleGetState(&run))?1:0);
                if (cpu.mode == 'I') showAddr(0);
                if (cpu.mode != 'R') showClock();
                }
    /* power switch */
              if (event.d2>=317 && event.d2<=336) {
                toggleSetState(&power,(toggleGetState(&power))?0:1);
                togglePaint(&power,display,window);
                control = OFF;
                }
              }
            }
          }

  }

MACHINE newHbElf() {
  MACHINE ret;
  ret.init = hbelf_init;
  ret.event = hbelf_event;
  return ret;
  }
