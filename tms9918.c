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

rcs_Window  tms9918w;

byte revByte(byte b) {
  return ((b & 128) >> 7) |
         ((b & 64) >> 5) |
         ((b & 32) >> 3) |
         ((b & 16) >> 1) |
         ((b & 8) << 1) |
         ((b & 4) << 3) |
         ((b & 2) << 5) |
         ((b & 1) << 7);
  }

void tms9918_configure(char* arg) {
  int port;
  portStruct ports;
  ports = parseArgs(arg);
  port = (ports.outPorts[0] == 0) ? 4 : ports.outPorts[0] - 1;
  outPorts[port].out = tms9918_modeHighOut;
  port = (ports.outPorts[1] == 0) ? 5 : ports.outPorts[1] - 1;
  outPorts[port].out = tms9918_modeLowOut;
  useTMS9918 = 'Y';
  }

void tms9918_setColors(TMS9918* vdp) {
#ifdef WIN32
  if (useGray == 'Y') {
    rcs_foreground(display,vdp->colors[1],0,0,0);
    rcs_foreground(display,vdp->colors[2],0x18,0x18,0x18);
    rcs_foreground(display,vdp->colors[3],0x30,0x30,0x30);
    rcs_foreground(display,vdp->colors[4],0x40,0x40,0x40);
    rcs_foreground(display,vdp->colors[5],0x50,0x50,0x50);
    rcs_foreground(display,vdp->colors[6],0x60,0x60,0x60);
    rcs_foreground(display,vdp->colors[7],0x70,0x70,0x70);
    rcs_foreground(display,vdp->colors[8],0x80,0x80,0x80);
    rcs_foreground(display,vdp->colors[9],0x90,0x90,0x90);
    rcs_foreground(display,vdp->colors[10],0xA0,0xA0,0xA0);
    rcs_foreground(display,vdp->colors[11],0xB0,0xB0,0xB0);
    rcs_foreground(display,vdp->colors[12],0xC0,0xC0,0xC0);
    rcs_foreground(display,vdp->colors[13],0xD0,0xD0,0xD0);
    rcs_foreground(display,vdp->colors[14],0xE0,0xE0,0xE0);
    rcs_foreground(display,vdp->colors[15],0xff,0xff,0xff);
    } else {
    rcs_foreground(display,vdp->colors[1],0,0,0);
    rcs_foreground(display,vdp->colors[2],0,0x7f,0);
    rcs_foreground(display,vdp->colors[3],0,0xff,0);
    rcs_foreground(display,vdp->colors[4],0,0,0x3f);
    rcs_foreground(display,vdp->colors[5],0,0,0xff);
    rcs_foreground(display,vdp->colors[6],0x3f,0,0);
    rcs_foreground(display,vdp->colors[7],0,0x7f,0x7f);
    rcs_foreground(display,vdp->colors[8],0x7f,0,0);
    rcs_foreground(display,vdp->colors[9],0xff,0,0);
    rcs_foreground(display,vdp->colors[10],0x7f,0x7f,0);
    rcs_foreground(display,vdp->colors[11],0xff,0xff,0);
    rcs_foreground(display,vdp->colors[12],0,0x3f,0);
    rcs_foreground(display,vdp->colors[13],0x7f,0,0x7f);
    rcs_foreground(display,vdp->colors[14],0x7f,0x7f,0x7f);
    rcs_foreground(display,vdp->colors[15],0xff,0xff,0xff);
    }
#else
  if (useGray == 'Y') {
    rcs_foreground(display,vdp->colors[1],0,0,0);
    rcs_foreground(display,vdp->colors[2],0x1800,0x1800,0x1800);
    rcs_foreground(display,vdp->colors[3],0x3000,0x3000,0x3000);
    rcs_foreground(display,vdp->colors[4],0x4000,0x4000,0x4000);
    rcs_foreground(display,vdp->colors[5],0x5000,0x5000,0x5000);
    rcs_foreground(display,vdp->colors[6],0x6000,0x6000,0x6000);
    rcs_foreground(display,vdp->colors[7],0x7000,0x7000,0x7000);
    rcs_foreground(display,vdp->colors[8],0x8000,0x8000,0x8000);
    rcs_foreground(display,vdp->colors[9],0x9000,0x9000,0x9000);
    rcs_foreground(display,vdp->colors[10],0xA000,0xA000,0xA000);
    rcs_foreground(display,vdp->colors[11],0xB000,0xB000,0xB000);
    rcs_foreground(display,vdp->colors[12],0xC000,0xC000,0xC000);
    rcs_foreground(display,vdp->colors[13],0xD000,0xD000,0xD000);
    rcs_foreground(display,vdp->colors[14],0xE000,0xE000,0xE000);
    rcs_foreground(display,vdp->colors[15],0xff00,0xff00,0xff00);
    } else {
    rcs_foreground(display,vdp->colors[1],0,0,0);
    rcs_foreground(display,vdp->colors[2],0,0x7f00,0);
    rcs_foreground(display,vdp->colors[3],0,0xff00,0);
    rcs_foreground(display,vdp->colors[4],0,0,0x3f00);
    rcs_foreground(display,vdp->colors[5],0,0,0xff00);
    rcs_foreground(display,vdp->colors[6],0x3f00,0,0);
    rcs_foreground(display,vdp->colors[7],0,0x7f00,0x7f00);
    rcs_foreground(display,vdp->colors[8],0x7f00,0,0);
    rcs_foreground(display,vdp->colors[9],0xff00,0,0);
    rcs_foreground(display,vdp->colors[10],0x7f00,0x7f00,0);
    rcs_foreground(display,vdp->colors[11],0xff00,0xff00,0);
    rcs_foreground(display,vdp->colors[12],0,0x3f00,0);
    rcs_foreground(display,vdp->colors[13],0x7f00,0,0x7f00);
    rcs_foreground(display,vdp->colors[14],0x7f00,0x7f00,0x7f00);
    rcs_foreground(display,vdp->colors[15],0xff00,0xff00,0xff00);
    }
#endif
  }

void tms9918_reset(TMS9918* vdp) {
  }

TMS9918 tms9918_init() {
  int     i;
  TMS9918 vdp;
  vdp.window=rcs_createWindow(display,rcs_rootWindow(display),0,0,256*vScale,192*vScale);
  rcs_setWindowName(display,vdp.window,"TMS9918A");
  rcs_showWindow(display,vdp.window);
  for (i=0; i<8; i++) vdp.regs[i] = 0;
  vdp.mode = 0;
  vdp.nameAddr = 0;
  vdp.colorAddr = 0;
  vdp.patternAddr = 0;
  vdp.spAttrAddr = 0;
  vdp.spPatternAddr = 0;
  vdp.curAddr = 0;
  for (i=0; i<16; i++) vdp.colors[i] = rcs_openGC(display,vdp.window);
  tms9918_setColors(&vdp);
  vdp.toggle = 0;
  return vdp;
  }

void tms9918_destroy(TMS9918* vdp) {
  int i;
  for (i=1; i<16; i++) rcs_closeGC(display,vdp->colors[i]);
  rcs_closeWindow(display,vdp->window);
  }

void tms9918_plot(rcs_Display* d,rcs_Window w,rcs_GC gc,int x,int y) {
#ifdef X
  if (vScale == 1) {
    XDrawPoint(d,w,gc,x,y);
    return;
    }
  x *= vScale;
  y *= vScale;
  XFillRectangle(d,w,gc,x,y,vScale,vScale);
#else
  rcs_drawPoint(d,w,gc,x,y);
#endif
  }

void tms9918_drawTile(TMS9918* vdp,word tile) {
  int px,py;
  int p;
  int c;
  int b;
  int cl;
  int x,y;
  word ofs;
  p = vdp->mem[vdp->nameAddr+tile];
  switch (vdp->mode) {
    case 0:c = vdp->mem[vdp->colorAddr + (p >> 3)];
           x = tile % 32;
           y = tile / 32;
           for (py=0; py<8; py++) {
             b = vdp->mem[vdp->patternAddr+p*8+py];
             for (px=0; px<8; px++) {
               cl = (b & 128) ? c>>4 : c & 0xf;
               if (cl == 0) cl = vdp->regs[7] & 0xf;
               if (cl == 0) cl = 1;
                 tms9918_plot(display,vdp->window,vdp->colors[cl],x*8+px,y*8+py);
               b = (b << 1) & 0xff;
               }
             }
           break;
    case 1:x = tile % 32;
           y = tile / 32;
           ofs = 0;
           if (y>7) ofs = 2048;
           if (y>15) ofs = 4096;
           for (py=0; py<8; py++) {
             b = vdp->mem[ofs + vdp->patternAddr+p*8+py];
             c = vdp->mem[ofs + vdp->colorAddr + p*8 + py];
             for (px=0; px<8; px++) {
               cl = (b & 128) ? c>>4 : c & 0xf;
               if (cl == 0) cl = vdp->regs[7] & 0xf;
               if (cl == 0) cl = 1;
                 tms9918_plot(display,vdp->window,vdp->colors[cl],x*8+px,y*8+py);
               b = (b << 1) & 0xff;
               }
             }
           break;
    case 4:c = vdp->regs[7];
           x = tile % 40;
           y = tile / 40;
           for (py=0; py<8; py++) {
             b = vdp->mem[vdp->patternAddr+p*8+py];
             for (px=0; px<6; px++) {
               cl = (b & 128) ? c>>4 : c & 0xf;
               if (cl == 0) cl = vdp->regs[7] & 0xf;
               if (cl == 0) cl = 1;
                 tms9918_plot(display,vdp->window,vdp->colors[cl],x*6+px,y*8+py);
               b = (b << 1) & 0xff;
               }
             }
           break;
    }
#ifdef X
  XFlush(display);
#endif
  }

void tms9918_drawScreen(TMS9918* vdp) {
  int x;
  if (vdp->mode == 0) for (x=0; x<768; x++) tms9918_drawTile(vdp,x);
  if (vdp->mode == 1) for (x=0; x<768; x++) tms9918_drawTile(vdp,x);
  if (vdp->mode == 4) for (x=0; x<960; x++) tms9918_drawTile(vdp,x);
  }


void tms9918_modeHighOut(byte value) {
  if (vdp.toggle) {
    if ((value & 0xf8) == 0x80 ) {
      vdp.regs[value & 7] = vdp.value;
      vdp.nameAddr = (vdp.regs[2] & 0xf) << 10;
      vdp.colorAddr = vdp.regs[3] << 6;
      vdp.patternAddr = (vdp.regs[4] & 0x7) << 11;
      vdp.mode = 0;
      if (vdp.regs[0] & 2) vdp.mode = 1;
      if (vdp.regs[1] & 8) vdp.mode = 2;
      if (vdp.regs[1] & 16) vdp.mode = 4;
      if (vdp.mode == 1) {
        vdp.colorAddr = (vdp.regs[3] & 128) ? 0x2000 : 0;
        vdp.patternAddr = (vdp.regs[4] & 4) ? 0x2000 : 0;
        }
      tms9918_drawScreen(&vdp);
      }
    if ((value & 0xc0) == 0x40) {
      vdp.curAddr = vdp.value + ((value & 0x3f) << 8);
      }
    vdp.toggle = 0;
    } else {
    vdp.value = value;
    vdp.toggle = 1;
    }
  }

void tms9918_modeLowOut(byte value) {
  int  i;
  int  p;
  word addr;
  addr = vdp.curAddr;
  vdp.mem[vdp.curAddr++] = value;
  switch (vdp.mode) {
    case 0:if (addr >= vdp.nameAddr && addr <= vdp.nameAddr+768) {
             tms9918_drawTile(&vdp,addr - vdp.nameAddr);
             return;
             }
           if (addr >= vdp.colorAddr && addr <= vdp.colorAddr+32) {
             p = addr - vdp.colorAddr;
             for (i=0; i<768; i++)
               if ((vdp.mem[vdp.nameAddr+i] >> 3) == p) tms9918_drawTile(&vdp,i);
             return;
             }
           if (addr >= vdp.patternAddr && addr <= vdp.patternAddr+2048) {
             p = (addr - vdp.patternAddr) >> 3;
             for (i=0; i<768; i++)
               if (vdp.mem[vdp.nameAddr+i] == p) tms9918_drawTile(&vdp,i);
             return;
             }
           break;
    case 1:if (addr >= vdp.nameAddr && addr <= vdp.nameAddr+768) {
             tms9918_drawTile(&vdp,addr - vdp.nameAddr);
             return;
             }
           if (addr >= vdp.colorAddr && addr <= vdp.colorAddr+2048) {
             p = addr - vdp.colorAddr;
             for (i=0; i<256; i++)
               if (vdp.mem[vdp.nameAddr+i] == p) tms9918_drawTile(&vdp,i);
             return;
             }
           if (addr >= vdp.colorAddr+2048 && addr <= vdp.colorAddr+4096) {
             p = addr - (vdp.colorAddr-2048);
             for (i=256; i<512; i++)
               if (vdp.mem[vdp.nameAddr+i] == p) tms9918_drawTile(&vdp,i);
             return;
             }
           if (addr >= vdp.colorAddr+4096 && addr <= vdp.colorAddr+6144) {
             p = addr - (vdp.colorAddr-4096);
             for (i=512; i<768; i++)
               if (vdp.mem[vdp.nameAddr+i] == p) tms9918_drawTile(&vdp,i);
             return;
             }
           if (addr >= vdp.patternAddr && addr <= vdp.patternAddr+2048) {
             p = (addr - (vdp.patternAddr-2048)) >> 3;
             for (i=0; i<256; i++)
               if (vdp.mem[vdp.nameAddr+i] == p) tms9918_drawTile(&vdp,i);
             return;
             }
           if (addr >= vdp.patternAddr+2048 && addr <= vdp.patternAddr+4096) {
             p = (addr - (vdp.patternAddr-2048)) >> 3;
             for (i=256; i<512; i++)
               if (vdp.mem[vdp.nameAddr+i] == p) tms9918_drawTile(&vdp,i);
             return;
             }
           if (addr >= vdp.patternAddr+4096 && addr <= vdp.patternAddr+6144) {
             p = (addr - (vdp.patternAddr-4096)) >> 3;
             for (i=512; i<768; i++)
               if (vdp.mem[vdp.nameAddr+i] == p) tms9918_drawTile(&vdp,i);
             return;
             }
           break;
    case 4:if (addr >= vdp.nameAddr && addr <= vdp.nameAddr+960) {
             tms9918_drawTile(&vdp,addr - vdp.nameAddr);
             return;
             }
           break;
    }
  }

void tms9918_event(rcs_Event event) {
  if (event.type == EVENT_EXPOSE) tms9918_drawScreen(&vdp);
  if (event.type == EVENT_BUTTON_PRESS) {
    useGray = (useGray == 'Y') ? 'N' : 'Y';
    tms9918_setColors(&vdp);
    tms9918_drawScreen(&vdp);
    }
  }

