/*
 *******************************************************************
 *** This software is copyright 2006 by Michael H Riley          ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

#include <librcs.h>
#include "elf.h"
#include "images/pushup.xpm"
#include "images/pushdn.xpm"

PUSH pushNew() {
  PUSH ret;
  ret.state=0;
  ret.x=0;
  ret.y=0;
  return ret;
  }

void pushSetPosition(PUSH* t,int px,int py) {
  t->x=px;
  t->y=py;
  }

void pushSetState(PUSH* t,int s) {
  t->state=s;
  }

int pushGetState(PUSH* t) {
  return (t->state) ? 1 : 0;
  }

void pushPaint(PUSH* t,rcs_Display* display,rcs_Window win) {
  rcs_GC gc;
  gc=rcs_openGC(display,win);
  if (!t->state) rcs_drawPixmap(display,win,gc,t->x,t->y,pushup_xpm);
    else rcs_drawPixmap(display,win,gc,t->x,t->y,pushdn_xpm);
  rcs_closeGC(display,gc);
  }

