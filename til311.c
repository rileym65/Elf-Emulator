/*
 *******************************************************************
 *** This software is copyright 2006 by Michael H Riley          ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

#include "librcs.h"
#include "elf.h"
#include "images/til311.xpm"

int matrix[] = { 0x093f,         /* 0 */
                 0x0386,         /* 1 */
                 0x165b,         /* 2 */
                 0x144f,         /* 3 */
                 0x1b66,         /* 4 */
                 0x1ced,         /* 5 */
                 0x087d,         /* 6 */
                 0x1387,         /* 7 */
                 0x007f,         /* 8 */
                 0x016f,         /* 9 */
                 0x0f77,         /* A */
                 0x1c7f,         /* B */
                 0x0ab9,         /* C */
                 0x1d3f,         /* D */
                 0x1ef9,         /* E */
                 0x1cf1          /* F */
               };

rcs_Pixmap til311Init(rcs_Display* display,rcs_Window win) {
  rcs_Pixmap ret;
  ret=rcs_createPixmap(display,win,83,131);
  rcs_drawPixmap(display,ret,gc,0,0,(const char**)til311_xpm);
  return ret;
  }

TIL311 til311New() {
  int i;
  TIL311 ret;
  ret.number=0;
  ret.blanked='N';
  ret.x=0;
  ret.y=0;
  ret.dpstate=0;
  for (i=0; i<16; i++) ret.segments[i] = 2;
  ret.last = 0;
  return ret;
  }

void til311SetNumber(TIL311* til,int n) { til->number=n; }

void til311SetPosition(TIL311* til,int px,int py) {
  til->x=px;
  til->y=py;
  }

/*
void til311Paint(TIL311* til,rcs_Display* display,rcs_Window win) {
  int ox,oy;
  ox = (til->number & 3) * 21;
  oy = ((til->number>>2) & 3) * 33;
  rcs_copyArea(display,til311,window,gc,ox,oy,20,32,til->x,til->y);
  }
*/

void til311Update(TIL311* til,rcs_Display* d,rcs_Window win) {
  rcs_GC gc;
  int ox,oy;
  int number;
  word mat;
  word l;
  ox=til->x+8; oy=til->y+4;
  number = til->number;
  l = til->last;
  mat = matrix[number];
  if ((l & 1) != (mat & 1)) {
    gc = (mat & 1) ? red : black;
    rcs_drawFilledBox(display,win,gc,ox+4,oy,2,2);
    rcs_drawFilledBox(display,win,gc,ox+8,oy,2,2);
    }
  if ((l & 2) != (mat & 2)) {
    gc = (mat & 2) ? red : black;
    rcs_drawFilledBox(display,win,gc,ox+12,oy+4,2,2);
    rcs_drawFilledBox(display,win,gc,ox+12,oy+8,2,2);
    }
  if ((l & 4) != (mat & 4)) {
    gc = (mat & 4) ? red : black;
    rcs_drawFilledBox(display,win,gc,ox+12,oy+16,2,2);
    rcs_drawFilledBox(display,win,gc,ox+12,oy+20,2,2);
    }
  if ((l & 8) != (mat & 8)) {
    gc = (mat & 8) ? red : black;
    rcs_drawFilledBox(display,win,gc,ox+4,oy+24,2,2);
    rcs_drawFilledBox(display,win,gc,ox+8,oy+24,2,2);
    }
  if ((l & 16) != (mat & 16)) {
    gc = (mat & 16) ? red : black;
    rcs_drawFilledBox(display,win,gc,ox,oy+16,2,2);
    rcs_drawFilledBox(display,win,gc,ox,oy+20,2,2);
    }
  if ((l & 32) != (mat & 32)) {
    gc = (mat & 32) ? red : black;
    rcs_drawFilledBox(display,win,gc,ox,oy+4,2,2);
    rcs_drawFilledBox(display,win,gc,ox,oy+8,2,2);
    }
  if ((l & 64) != (mat & 64)) {
    gc = (mat & 64) ? red : black;
    rcs_drawFilledBox(display,win,gc,ox+4,oy+12,2,2);
    rcs_drawFilledBox(display,win,gc,ox+8,oy+12,2,2);
    }
  if ((l & 128) != (mat & 128)) {
    gc = (mat & 128) ? red : black;
    rcs_drawFilledBox(display,win,gc,ox+12,oy,2,2);
    }
  if ((l & 256) != (mat & 256)) {
    gc = (mat & 256) ? red : black;
    rcs_drawFilledBox(display,win,gc,ox+12,oy+12,2,2);
    }
  if ((l & 512) != (mat & 512)) {
    gc = (mat & 512) ? red : black;
    rcs_drawFilledBox(display,win,gc,ox+12,oy+24,2,2);
    }
  if ((l & 1024) != (mat & 1024)) {
    gc = (mat & 1024) ? red : black;
    rcs_drawFilledBox(display,win,gc,ox,oy+24,2,2);
    }
  if ((l & 2048) != (mat & 2048)) {
    gc = (mat & 2048) ? red : black;
    rcs_drawFilledBox(display,win,gc,ox,oy+12,2,2);
    }
  if ((l & 4096) != (mat & 4096)) {
    gc = (mat & 4096) ? red : black;
    rcs_drawFilledBox(display,win,gc,ox,oy,2,2);
    }
  til->last = mat;
  }

void til311Paint(TIL311* til,rcs_Display* display,rcs_Window win) {
  int ox,oy;
  int number;
  ox=til->x+8; oy=til->y+4;
  number = til->number;
  rcs_drawFilledBox(display,win,black,til->x+4,til->y+1,23,34);
  til->last = 0;
  til311Update(til,display,win);
  return;

  if (matrix[number] & 1) {
    rcs_drawFilledBox(display,win,red,ox+4,oy,2,2);
    rcs_drawFilledBox(display,win,red,ox+8,oy,2,2);
    }
  if (matrix[number] & 2) {
    rcs_drawFilledBox(display,win,red,ox+12,oy+4,2,2);
    rcs_drawFilledBox(display,win,red,ox+12,oy+8,2,2);
    }
  if (matrix[number] & 4) {
    rcs_drawFilledBox(display,win,red,ox+12,oy+16,2,2);
    rcs_drawFilledBox(display,win,red,ox+12,oy+20,2,2);
    }
  if (matrix[number] & 8) {
    rcs_drawFilledBox(display,win,red,ox+4,oy+24,2,2);
    rcs_drawFilledBox(display,win,red,ox+8,oy+24,2,2);
    }
  if (matrix[number] & 16) {
    rcs_drawFilledBox(display,win,red,ox,oy+16,2,2);
    rcs_drawFilledBox(display,win,red,ox,oy+20,2,2);
    }
  if (matrix[number] & 32) {
    rcs_drawFilledBox(display,win,red,ox,oy+4,2,2);
    rcs_drawFilledBox(display,win,red,ox,oy+8,2,2);
    }
  if (matrix[number] & 64) {
    rcs_drawFilledBox(display,win,red,ox+4,oy+12,2,2);
    rcs_drawFilledBox(display,win,red,ox+8,oy+12,2,2);
    }
  if (matrix[number] & 128) {
    rcs_drawFilledBox(display,win,red,ox+12,oy,2,2);
    }
  if (matrix[number] & 256) {
    rcs_drawFilledBox(display,win,red,ox+12,oy+12,2,2);
    }
  if (matrix[number] & 512) {
    rcs_drawFilledBox(display,win,red,ox+12,oy+24,2,2);
    }
  if (matrix[number] & 1024) {
    rcs_drawFilledBox(display,win,red,ox,oy+24,2,2);
    }
  if (matrix[number] & 2048) {
    rcs_drawFilledBox(display,win,red,ox,oy+12,2,2);
    }
  if (matrix[number] & 4096) {
    rcs_drawFilledBox(display,win,red,ox,oy,2,2);
    }
  }
