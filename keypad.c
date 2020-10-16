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

void keypadEvent(rcs_Event event) {
  rcs_GC gc;
  if (event.type == EVENT_BUTTON_PRESS && event.d1==1) {
/* dmain */
    if (event.d2>=12 && event.d2<44 &&
        event.d3>=12 && event.d3<44) {
        cdp1802DmaIn(&cpu,keypadValue);
        showData(keypadValue);
      }

    if (event.d2>=12 && event.d2<44 &&
        event.d3>=45 && event.d3<77) {
      keypadValue=(nextNybble=='H')?(keypadValue&15)+0xc0:(keypadValue&0xf0)+12;
      nextNybble=(nextNybble=='H')?'L':'H';
      }
    if (event.d2>=45 && event.d2<77 &&
        event.d3>=45 && event.d3<77) {
      keypadValue=(nextNybble=='H')?(keypadValue&15)+0xd0:(keypadValue&0xf0)+13;
      nextNybble=(nextNybble=='H')?'L':'H';
      }
    if (event.d2>=77 && event.d2<109 &&
        event.d3>=45 && event.d3<77) {
      keypadValue=(nextNybble=='H')?(keypadValue&15)+0xe0:(keypadValue&0xf0)+14;
      nextNybble=(nextNybble=='H')?'L':'H';
      }
    if (event.d2>=109 && event.d2<141 &&
        event.d3>=45 && event.d3<77) {
      keypadValue=(nextNybble=='H')?(keypadValue&15)+0xf0:(keypadValue&0xf0)+15;
      nextNybble=(nextNybble=='H')?'L':'H';
      }

    if (event.d2>=12 && event.d2<44 &&
        event.d3>=77 && event.d3<109) {
      keypadValue=(nextNybble=='H')?(keypadValue&15)+0x80:(keypadValue&0xf0)+8;
      nextNybble=(nextNybble=='H')?'L':'H';
      }
    if (event.d2>=45 && event.d2<77 &&
        event.d3>=77 && event.d3<109) {
      keypadValue=(nextNybble=='H')?(keypadValue&15)+0x90:(keypadValue&0xf0)+9;
      nextNybble=(nextNybble=='H')?'L':'H';
      }
    if (event.d2>=77 && event.d2<109 &&
        event.d3>=77 && event.d3<109) {
      keypadValue=(nextNybble=='H')?(keypadValue&15)+0xa0:(keypadValue&0xf0)+10;
      nextNybble=(nextNybble=='H')?'L':'H';
      }
    if (event.d2>=109 && event.d2<141 &&
        event.d3>=77 && event.d3<109) {
      keypadValue=(nextNybble=='H')?(keypadValue&15)+0xb0:(keypadValue&0xf0)+11;
      nextNybble=(nextNybble=='H')?'L':'H';
      }

    if (event.d2>=12 && event.d2<44 &&
        event.d3>=109 && event.d3<141) {
      keypadValue=(nextNybble=='H')?(keypadValue&15)+0x40:(keypadValue&0xf0)+4;
      nextNybble=(nextNybble=='H')?'L':'H';
      }
    if (event.d2>=45 && event.d2<77 &&
        event.d3>=109 && event.d3<141) {
      keypadValue=(nextNybble=='H')?(keypadValue&15)+0x50:(keypadValue&0xf0)+5;
      nextNybble=(nextNybble=='H')?'L':'H';
      }
    if (event.d2>=77 && event.d2<109 &&
        event.d3>=109 && event.d3<141) {
      keypadValue=(nextNybble=='H')?(keypadValue&15)+0x60:(keypadValue&0xf0)+6;
      nextNybble=(nextNybble=='H')?'L':'H';
      }
    if (event.d2>=109 && event.d2<141 &&
        event.d3>=109 && event.d3<141) {
      keypadValue=(nextNybble=='H')?(keypadValue&15)+0x70:(keypadValue&0xf0)+7;
      nextNybble=(nextNybble=='H')?'L':'H';
      }

    if (event.d2>=12 && event.d2<44 &&
        event.d3>=141 && event.d3<173) {
      keypadValue=(nextNybble=='H')?(keypadValue&15)+0x00:(keypadValue&0xf0)+0;
      nextNybble=(nextNybble=='H')?'L':'H';
      }
    if (event.d2>=45 && event.d2<77 &&
        event.d3>=141 && event.d3<173) {
      keypadValue=(nextNybble=='H')?(keypadValue&15)+0x10:(keypadValue&0xf0)+1;
      nextNybble=(nextNybble=='H')?'L':'H';
      }
    if (event.d2>=77 && event.d2<109 &&
        event.d3>=141 && event.d3<173) {
      keypadValue=(nextNybble=='H')?(keypadValue&15)+0x20:(keypadValue&0xf0)+2;
      nextNybble=(nextNybble=='H')?'L':'H';
      }
    if (event.d2>=109 && event.d2<141 &&
        event.d3>=141 && event.d3<173) {
      keypadValue=(nextNybble=='H')?(keypadValue&15)+0x30:(keypadValue&0xf0)+3;
      nextNybble=(nextNybble=='H')?'L':'H';
      }
    keypadHi.state=(nextNybble=='H')?1:0;
    keypadLo.state=(nextNybble=='H')?0:1;
	redLedPaint(&keypadHi,display,hexWindow);
    redLedPaint(&keypadLo,display,hexWindow);
    }
  if (event.type == EVENT_EXPOSE) {
    gc=rcs_openGC(display,hexWindow);
    rcs_copyArea(display,hexBuffer,hexWindow,gc,0,0,178,229,0,0);
    rcs_closeGC(display,gc);
	redLedPaint(&keypadHi,display,hexWindow);
    redLedPaint(&keypadLo,display,hexWindow);
    }
  }

