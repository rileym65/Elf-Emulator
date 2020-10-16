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
#include "images/ledon.xpm"
#include "images/ledoff.xpm"

REDLED redLedNew() {
  REDLED ret;
  ret.state=0;
  ret.x=0;
  ret.y=0;
  return ret;
  }

void redLedSetPosition(REDLED* t,int px,int py) {
  t->x=px;
  t->y=py;
  }

void redLedSetState(REDLED* t,int s) {
  t->state=s;
  }

int redLedGetState(REDLED* t) { return t->state; }

void redLedPaint(REDLED* t,rcs_Display* display,rcs_Window win) {
  rcs_GC gc;
  gc=rcs_openGC(display,win);
  if (t->state) rcs_drawPixmap(display,win,gc,t->x,t->y,ledon_xpm);
    else rcs_drawPixmap(display,win,gc,t->x,t->y,ledoff_xpm);
  rcs_closeGC(display,gc);
  }

