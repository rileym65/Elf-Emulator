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
#include <string.h>
#include "elf.h"

#define CYCLE 2000
#define CID_CYCLE 5000

char cmode;
int  cycleSize;

void v1870_redraw();

rcs_GC  gcColors[8];

void v1870_o3(byte value) {
  word old;
  char buffer[80];
  old = v1870.reg3;
  v1870.reg3 = value;
  if (trace == 'Y') {
    strcpy(buffer,"1870:3 HR:");
    if (value & 0x80) strcat(buffer,"40"); else strcat(buffer,"20");
    strcat(buffer," COLB1:");
    if (value & 0x40) strcat(buffer,"1"); else strcat(buffer,"0");
    strcat(buffer," COLB0:");
    if (value & 0x20) strcat(buffer,"1"); else strcat(buffer,"0");
    strcat(buffer," DSP:");
    if (value & 0x10) strcat(buffer,"OFF"); else strcat(buffer,"ON");
    strcat(buffer," CFC:");
    if (value & 0x08) strcat(buffer,"1"); else strcat(buffer,"0");
    strcat(buffer," RED:");
    if (value & 0x04) strcat(buffer,"1"); else strcat(buffer,"0");
    strcat(buffer," BLUE:");
    if (value & 0x02) strcat(buffer,"1"); else strcat(buffer,"0");
    strcat(buffer," GREEN:");
    if (value & 0x01) strcat(buffer,"1"); else strcat(buffer,"0");
    debugDisplay(buffer);
    }
  if (old  != v1870.reg3) v1870_redraw();
  }

void v1870_o5(byte value) {
  word old;
  char buffer[80];
  old = v1870.reg5;
  v1870.reg5 = v1870.addr;
  v1870.reg6 = (v1870.addr & 0x7ff);
  cmode = '0';
  if (v1870.reg5 & 1) cmode = '1';
  if (trace == 'Y') {
    value = v1870.addr;
    strcpy(buffer,"1870:5 VR:");
    if (value & 0x80) strcat(buffer,"24"); else strcat(buffer,"12");
    strcat(buffer," PAGE:");
    if (value & 0x40) strcat(buffer,"DBL"); else strcat(buffer,"SNG");
    strcat(buffer," CH:");
    if (value & 0x20) strcat(buffer,"16"); else strcat(buffer,"8");
    strcat(buffer," CH:");
    if (value & 0x08) strcat(buffer,"-"); else strcat(buffer,"9");
    strcat(buffer," CMEM:");
    if (value & 0x01) strcat(buffer,"ON"); else strcat(buffer,"OFF");
    debugDisplay(buffer);
    }
  if (old  != v1870.reg5) v1870_redraw();
  }

void v1870_o6(byte value) {
  char buffer[80];
  cmode = '2';
  v1870.reg6 = (v1870.addr & 0x7ff);
  if (trace == 'Y') {
    sprintf(buffer,"1870:6 %04X\n",v1870.reg6);
    debugDisplay(buffer);
    }
  }

void v1870_o7(byte value) {
  word old;
  char buffer[80];
  old = v1870.reg7;
  v1870.reg7 = (v1870.addr & 0x7ff);
  if (old != v1870.reg7) v1870_redraw();
  if (trace == 'Y') {
    sprintf(buffer,"1870:7 %04X\n",v1870.reg7);
    debugDisplay(buffer);
    }
  }

void v1870_cycle() {
  v1870.cycle --;
  if (v1870.cycle <= 0) {
    v1870.cycle = cycleSize;
    cdp1802Int(&cpu);
    }
  }

byte v1870EF1() {
  return (v1870.cycle <cycleSize/2) ? 0 : 1;
  }

void v1870_configure(char* arg) {
  use1870 = 'Y';
  outPorts[2].out = v1870_o3;
  outPorts[4].out = v1870_o5;
  outPorts[5].out = v1870_o6;
  outPorts[6].out = v1870_o7;
  outPorts[6].cycle = v1870_cycle;
  efDevices[0].ef1 = v1870EF1;
  }

V1870 v1870_init() {
  int i;
  V1870 ret;
  if (cidelsa == 'Y') cycleSize = CID_CYCLE;
    else cycleSize = CYCLE;
  for (i=0; i<2024; i++) ret.cram[i] = 0;
  for (i=0; i<2048; i++) ret.pram[i] = 0;
  ret.reg3 = 0;
  ret.reg5 = 0;
  ret.reg6 = 0;
  ret.cycle = cycleSize;
  for (i=0; i<8; i++) gcColors[i] = rcs_openGC(display,window);
    rcs_namedForeground(display,gcColors[0],"black");
    rcs_namedForeground(display,gcColors[1],"green");
    rcs_namedForeground(display,gcColors[2],"blue");
    rcs_namedForeground(display,gcColors[3],"cyan");
    rcs_namedForeground(display,gcColors[4],"red");
    rcs_namedForeground(display,gcColors[5],"yellow");
    rcs_namedForeground(display,gcColors[6],"magenta");
    rcs_namedForeground(display,gcColors[7],"white");
  return ret;
  }

void v1870_event(rcs_Event event) {
  if (event.type == EVENT_EXPOSE && event.d5 == 0) {
    v1870_redraw();
    }
  }

byte v1870_read(word addr) {
  if (addr >= 0xf400 && addr < 0xf800) {
    addr &= 0xf;
    if (cidelsa == 'Y') addr &= 7;
    if (v1870.pcb[addr+(v1870.pram[v1870.reg6])*16] != 0) cid1 |= 128;
      else cid1 &= 0x7f;
    addr += (v1870.pram[v1870.reg6] * 16);
    return v1870.cram[addr];
    }
  if (addr >= 0xf800 && addr <= 0xffff) {
    return v1870.pram[addr - 0xf800];
    }
  return 255;
  }

void v1870_plot(rcs_Display* d,rcs_Window w,rcs_GC *gc,int x,int y) {
  int i;
  if (cidelsa == 'Y') {
    i = x; x = y; y = i;
    x = 200-x;
    }
#ifdef X
  if (vScale == 1) {
    XDrawPoint(d,w,*gc,x,y);
    return;
    }
  x *= vScale;
  y *= vScale;
  XFillRectangle(d,w,*gc,x,y,vScale,vScale);
#else
  rcs_drawPoint(display,v1870Window,*gc,x,y);
#endif
  }

void v1870_drawLine(int x,int y,byte v,byte pcb) {
  int i;
  int clr;
  rcs_GC *fore;
  rcs_GC *back;
  if ((v1870.reg5 & 0x80) == 0) y *= 2;
  back = &(gcColors[v1870.reg3 & 7]);
  clr = 0;
  switch (v1870.reg3 & 0x60) {
    case 0x00:if (v & 0x40) clr += 4;
              if (v & 0x80) clr += 2;
              if (pcb) clr += 1;
              break;
    case 0x20:if (v & 0x40) clr += 4;
              if (pcb) clr += 2;
              if (v & 0x80) clr += 1;
              break;
    case 0x40:if (pcb) clr += 4;
              if (v & 0x40) clr += 2;
              if (v & 0x80) clr += 1;
              break;
    case 0x60:if (pcb) clr += 4;
              if (v & 0x40) clr += 2;
              if (v & 0x80) clr += 1;
              break;
    }
  fore = &(gcColors[clr]);
  clr = 0;
  v <<= 2;
  for (i=0; i<6; i++) {
    if (v & 128) {
      v1870_plot(display,v1870Window,fore,x,y);
      if ((v1870.reg5 & 0x80) == 0) {
        v1870_plot(display,v1870Window,fore,x,y+1);
        }
      if ((v1870.reg3 & 0x80) != 0x80) {
        v1870_plot(display,v1870Window,fore,x+1,y);
        if ((v1870.reg5 & 0x80) == 0) {
          v1870_plot(display,v1870Window,fore,x+1,y+1);
          }
        }
      } else {
      v1870_plot(display,v1870Window,back,x,y);
      if ((v1870.reg5 & 0x80) == 0) {
        v1870_plot(display,v1870Window,back,x,y+1);
        }
      if ((v1870.reg3 & 0x80) != 0x80) {
        v1870_plot(display,v1870Window,back,x+1,y);
        if ((v1870.reg5 & 0x80) == 0) {
          v1870_plot(display,v1870Window,back,x+1,y+1);
          }
        }
      }

    x++;
    if ((v1870.reg3 & 0x80) != 0x80) x++;
    v <<= 1;
    }
  }

void v1870_drawChar(int x,int y,byte v) {
  int i;
  int a;
  int l;
  byte pcb;
  pcb = (v & 0x80) ? 1 : 0;
  if ((v1870.reg3 & 0x10) == 0x10) {
    return;
    }
  if (v1870Pcb == 'Y') v &= 0x7f;
  if ((v1870.reg3 & 0x80) != 0x80) x *= 2;
  l = ((v1870.reg5 & 0x8) == 0x8) ? 8 : 9;
  a = v * 16;
  for (i=0; i<l; i++) {
    if (cidelsa == 'Y') pcb = v1870.pcb[a];
    v1870_drawLine(x,y,v1870.cram[a],pcb);
    a++;
    y++;
    }
  }

void v1870_write(word addr,byte v) {
  int i;
  int l;
  int a;
  int s;
  int x,y;
  word ac;
  int m;

  if (addr >= 0xf800 && addr <= 0xffff && cmode=='1') {
      v1870.pram[v1870.reg6] = v;
      return;
    }

  m = ((v1870.reg3 & 0x80) == 0x80) ? 40 : 20;
  i = ((v1870.reg5 & 0x80) == 0x80) ? 24 : 12;
  l = ((v1870.reg5 & 0x8) == 0x8) ? 8 : 9;
  s = m * i;
/*  if (addr >= 0xf400 && addr < 0xf800) { */
  if ((addr - 0xf800) != v1870.reg6) {
    if ((v1870.reg5 & 1) == 1) {
      addr &= 0xf;
      if (cidelsa == 'Y') addr &= 7;
      ac = v1870.reg6;
      addr += (v1870.pram[ac] * 16);
      v1870.cram[addr] = v;
      if (cidelsa == 'Y') v1870.pcb[addr] = (cpu.q) ? 1 : 0;
      a = v1870.reg7;
    a = (a / m) * m;
    while (a < 0) a += 960;
      for (i=0; i<s; i++) {
        if (v1870.pram[a] == v1870.pram[ac]) {
          x = (i % m) * 6;
          y = (i / m) * l;
          v1870_drawChar(x,y,v1870.pram[a]);
          }
        a++;
        if (a >= 2048) a = 0;
        }
#ifdef X
      XFlush(display);
#endif
      return;
      }
    }

  if (addr >= 0xf800 && addr <= 0xffff) {
if (cmode == '1') {
printf("mode 1\n");
  v1870.pram[v1870.reg6] = v;
  return;
  }
    addr = addr - 0xf800;
    v1870.pram[addr] = v;
    a = addr - ((v1870.reg7 / m) * m);
    while (a < 0) a += 960;
    x = (a % m) * 6;
    y = (a / m) * l;
    v1870_drawChar(x,y,v);
    }
#ifdef X
    XFlush(display);
#endif
  }

void v1870_redraw() {
  int i;
  int s;
  int m;
  int l;
  int x,y;
  int a;
  rcs_GC gc;
  if ((v1870.reg3 & 0x10) == 0x10) {
    gc = rcs_openGC(display,v1870Window);
    rcs_namedForeground(display,gc,"black");
    if (cidelsa == 'Y') 
      rcs_drawFilledBox(display,v1870Window,gc,0,0,224*vScale,240*vScale);
    else
      rcs_drawFilledBox(display,v1870Window,gc,0,0,240*vScale,224*vScale);
    rcs_closeGC(display,gc);
    return;
    }
  m = ((v1870.reg3 & 0x80) == 0x80) ? 40 : 20;
  i = ((v1870.reg5 & 0x80) == 0x80) ? 24 : 12;
  l = ((v1870.reg5 & 0x8) == 0x8) ? 8 : 9;
  s = m * i;
  a = v1870.reg7;
  a = (a/m) * m;
  while (a<0) a += 960;
  for (i=0; i<s; i++) {
    x = (i % m) * 6;
    y = (i / m) * l;
    v1870_drawChar(x,y,v1870.pram[a]);
    a++;
/*    if (a>=2048) a = 0; */
    if (a>=960) a = 0;
    }
#ifdef X
    XFlush(display);
#endif
  }

