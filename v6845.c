/*
 *******************************************************************
 *** This software is copyright 2006 by Michael H Riley          ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

#ifndef _CHARSET
#include "images/charset.xpm"
#include "images/icharset.xpm"
#define _CHARSET
#endif

#include <librcs.h>
#include "elf.h"

void v6845Write(word addr,byte value) {
  rcs_GC gc;
  int x,y;
  y = (addr/charLine)*16;
  x = (addr%charLine)*8;
  gc = rcs_openGC(display,v6845Buffer);
  if (value < 32) value += '@';
  if (!(value & 128)) {
    rcs_drawPixmap(display,v6845Buffer,gc,x,y,(const char**)charset[value]);
    rcs_drawPixmap(display,v6845Window,gc,x,y,(const char**)charset[value]);
    } else {
    value &= 0x7f;
    if (value < 32) value += '@';
    rcs_drawPixmap(display,v6845Buffer,gc,x,y,(const char**)icharset[value]);
    rcs_drawPixmap(display,v6845Window,gc,x,y,(const char**)icharset[value]);
    }
  rcs_closeGC(display,gc); 
  }

void v6845OutEvent(rcs_Event event) {
  rcs_GC gc;
  if (event.type == EVENT_EXPOSE) {
    gc=rcs_openGC(display,v6845Window);
    rcs_copyArea(display,v6845Buffer,v6845Window,gc,0,0,512,256,0,0);
    rcs_closeGC(display,gc);
    }
  }
