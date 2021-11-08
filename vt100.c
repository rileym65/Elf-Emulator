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

#include "librcs.h"
#include "elf.h"
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

/* rcs_GC       gc; */
rcs_Event    event;
int          ctrlState;
int          curLine;
int          curPos;
char         escBuffer[256];
int          escPos;
int          saveLine;
int          savePos;

byte vidRam[25][80];
char keyBuffer[16];
int  keyStart;
int  keyEnd;
byte vt100EF;
byte vtEnabled;
char xmodemMode;
byte xmodemBuffer[256];
int  xmodemCount;
int  xmodemFile;
int  xmodemBlock;
#ifdef WIN32
rcs_Pixmap   vt100Buffer2;
#endif


void configureVt100(char* arg) {
  int port;
  portStruct ports;
  ports = parseArgs(arg);
  port = (ports.outPorts[0] == 0) ? 6 : ports.outPorts[0] - 1;
  outPorts[port].out = vt100Out;
  outPorts[port].cycle = vt100Cycle;
  port = (ports.ef[0] == 0) ? 3 : ports.ef[0] - 1;
  switch (port) {
    case 0:efDevices[0].ef1 = getVt100EF; break;
    case 1:efDevices[1].ef2 = getVt100EF; break;
    case 2:efDevices[2].ef3 = getVt100EF; break;
    case 3:efDevices[3].ef4 = getVt100EF; break;
    }
  vtEnabled = 1;
  useVt100 = 'Y';
  vtCount = -1;
  vtOutCount = -1;
  vtOut = -1;
  vt100EF = 1;
  }

int vt100Parity(int value) {
  int i;
  int par;
  par = 0;
  for (i=0; i<8; i++) {
    if (value & 1) par++;
    value >>= 1;
    }
  return (par & 1);
  }

void vt100Cycle() {
  int i;
  byte checksum;
  if (vtOutCount > 0) {
    vtOutCount--;
    if (vtOutCount <= 0) {
      vt100EF = (vtOut & 1) ? 1 : 0;
      vtOut = (vtOut >> 1) | 128;
      vtOutCount = baud;
      if (vtOutBits < 2) vt100EF = 1;
      if (--vtOutBits == 0) {
        vtOut = -1;
        vtOutCount = -1;
        if (xmodemMode == 'S' && xmodemCount < 132) {
          vtOut = xmodemBuffer[xmodemCount++];
          }
        }
      }
    } else if (vtOut >= 0 && vtEnabled) {
if (xmodemMode == 'R') printf("-->%02x\n",vtOut);
if (xmodemMode == 'S') printf("<%02x>",vtOut);
if (vtOut == 0x06) printf("<ACK>\n");
    vt100EF = 0;
    vtOutCount = baud;
    vtOutBits = 10;
    }
  if (vtCount < 0) {
    if (cpu.q) {
      vtCount = baud + baud/2;
      vtBits = 9;
      rs232 = 0;
      }
    } else {
    vtCount--;
    if (vtCount <= 0) {
      if (vtBits > 1) {
        rs232 >>= 1;
        rs232 |= (cpu.q) ? 0 : 128;
        }
      vtCount = baud;
      if (--vtBits == 0) {
        vtCount = -1;
if (xmodemMode == ' ') {
//if (rs232 != 10 && rs232 != 13 && rs232 != 0x1b && (rs232 < 32 || rs232 > 0x7e)) printf("[%02x]\n",rs232);
  }
if (xmodemMode == 'R') printf("<%02x>",rs232);
if (xmodemMode == 'S') printf("\n[%02x]\n",rs232);
        if (xmodemMode == 'R') {
          if (xmodemCount == 0 && (rs232 & 0xff) == 0x04) {
printf("\nEOT received\n");
            xmodemMode = ' ';
            close(xmodemFile);
            vtOut = 0x06;
            }
          else {
            xmodemBuffer[xmodemCount++] = rs232 & 0xff;
            if (xmodemCount == 132) {
              checksum = 0;
              for (i=0; i<131; i++) checksum += xmodemBuffer[i];
printf("\nBlock %d received %02x == %02x\n",xmodemBuffer[1],xmodemBuffer[131],checksum);
              write(xmodemFile, xmodemBuffer+3, 128);
              xmodemCount = 0;
              vtOut = 0x06;
              }
            }
          }
        else if (xmodemMode == 'S') {
          if (rs232 == 0x06 || rs232 == 0x15) {
            i = read(xmodemFile, xmodemBuffer+3, 128);
            if (i <= 0) {
printf("<04>\n");
              vtOut = 0x04;
              close(xmodemFile);
              xmodemMode = ' ';
              }
            else {
              while (i<128) { xmodemBuffer[i+3]=0x1a; i++; }
              xmodemBuffer[0] = 0x01;
              xmodemBuffer[1] = xmodemBlock;
              xmodemBuffer[2] = (255 - xmodemBlock) & 0xff;
              checksum = 0;
              for (i=0; i<131; i++) checksum += xmodemBuffer[i];
              xmodemBuffer[131] = checksum;
              xmodemCount = 1;
              vtOut = 0x01;
              xmodemBlock = (xmodemBlock + 1) & 0xff;
              }
            }
          if (rs232 == 0x18) {
printf("CAN received\n");
            xmodemMode = ' ';
            }
          }
        else {
          vt100Display(rs232 & 0x7f);
          }
        }
      }
    }
  }

void vt100Out(byte value) {
  if (value == 0x40) vtEnabled = 0;
  if (value == 0x80) vtEnabled = 1;

  }

byte getVt100EF() {
  return vt100EF;
//  return (ef4Sense^vt100EF);
  }


void vt100Display(int byt);

void vt100ScrollUp() {
  int x,y;
  rcs_GC gc;
  gc = rcs_openGC(display,vt100Buffer);
#ifdef WIN32
  rcs_copyArea(display,vt100Buffer,vt100Buffer2,gc,0,16,640,384,0,0);
  rcs_copyArea(display,vt100Buffer2,vt100Buffer,gc,0,0,640,384,0,0);
#else
  rcs_copyArea(display,vt100Buffer,vt100Buffer,gc,0,16,640,384,0,0);
#endif
  rcs_namedForeground(display,gc,"black");
  rcs_drawFilledBox(display,vt100Buffer,gc,0,384,640,16);
  rcs_copyArea(display,vt100Buffer,vt100Window,gc,0,0,640,400,0,0);
  rcs_closeGC(display,gc);
  for (y=0; y<24; y++)
    for (x=0; x<80; x++) {
      vidRam[y][x] = vidRam[y+1][x];
      }
  for (x=0; x<80; x++) vidRam[24][x] = 32;
  }

void showCursor() {
  rcs_GC gc;
  gc = rcs_openGC(display,vt100Buffer);
  rcs_drawPixmap(display,vt100Buffer,gc,curPos<<3,curLine<<4,(const char**)icharset[vidRam[curLine][curPos]]);
  rcs_drawPixmap(display,vt100Window,gc,curPos<<3,curLine<<4,(const char**)icharset[vidRam[curLine][curPos]]);
  rcs_closeGC(display,gc);
  }

void removeCursor() {
  rcs_GC gc;
  gc = rcs_openGC(display,vt100Buffer);
  rcs_drawPixmap(display,vt100Buffer,gc,curPos<<3,curLine<<4,(const char**)charset[vidRam[curLine][curPos]]);
  rcs_drawPixmap(display,vt100Window,gc,curPos<<3,curLine<<4,(const char**)charset[vidRam[curLine][curPos]]);
  rcs_closeGC(display,gc);
  }

char* getInteger(char* buffer,int* dest) {
  *dest = 0;
  while (*buffer >= '0' && *buffer <= '9') {
    *dest = *dest * 10 + (*buffer - '0');
    buffer++;
    }
  return buffer;
  }

void vterr() {
  word i;
  escPos = -1;
  for (i=0; i<strlen(escBuffer); i++) vt100Display(escBuffer[i]);
  }

void vt100Display(int byt) {
  int i;
  int x,y;
  char *pBuffer;
  FILE* file;
  rcs_GC gc;
  if (serialLog == 'Y') {
    file = fopen("vt100.log","a");
    fprintf(file,"%c",byt);
    fclose(file);
    }
  removeCursor();
  if (escPos >= 0) {
    if (escPos > 20) {
      escPos = -1;
      return;
      }
    escBuffer[escPos++] = byt;
    escBuffer[escPos] = 0;
    if (byt == 'A' && escBuffer[0] == '[') {
      pBuffer = getInteger(&escBuffer[1],&x);
      if (*pBuffer == byt) {
        if (x == 0) x = 1;
        curLine -= x;
        if (curLine < 0) curLine = 0;
        escPos = -1;
        } else vterr();
      } else if (byt == 'A') vterr();
    if (byt == 'B' && escBuffer[0] == '[') {
      pBuffer = getInteger(&escBuffer[1],&x);
      if (*pBuffer == byt) {
        if (x == 0) x = 1;
        curLine += x;
        if (curLine > 24) curLine = 24;
        escPos = -1;
        } else vterr();
      } else if (byt == 'B') vterr();
    if (byt == 'C' && escBuffer[0] == '[') {
      pBuffer = getInteger(&escBuffer[1],&x);
      if (*pBuffer == byt) {
        if (x == 0) x = 1;
        curPos += x;
        if (curPos > 79) curPos = 79;
        escPos = -1;
        } else vterr();
      } else if (byt == 'C') vterr();
    if (byt == 'D' && escBuffer[0] == '[') {
      pBuffer = getInteger(&escBuffer[1],&x);
      if (*pBuffer == byt) {
        if (x == 0) x = 1;
        curPos -= x;
        if (curPos < 0) curPos = 0;
        escPos = -1;
        } else vterr();
      } else if (byt == 'D') vterr();
    if (byt == 'H' && escBuffer[0] == '[') {
      pBuffer = getInteger(&escBuffer[1],&y);
      if (*pBuffer == ';') {
        pBuffer = getInteger(pBuffer+1,&x);
        } else x = 0;
      if (*pBuffer == byt) {
        curPos = (x < 80) ? x : 79;
        curLine = (y < 25) ? y : 24;
        escPos = -1;
        } else vterr();
      } else if (byt == 'H') vterr();
    if (byt == 'J' && escBuffer[0] == '[') {
      if (strcmp(escBuffer,"[J") == 0) {
        gc = rcs_openGC(display,vt100Window);
        for (y=curLine; y<25; y++)
          for (x=0; x<80; x++) vidRam[y][x] = 32;
        rcs_namedForeground(display,gc,"black");
        rcs_drawFilledBox(display,vt100Buffer,gc,0,(curLine)<<4,640,400);
        rcs_drawFilledBox(display,vt100Window,gc,0,(curLine)<<4,640,400);
        rcs_closeGC(display,gc);
        escPos = -1;
        }
      else if (strcmp(escBuffer,"[1J") == 0) {
        gc = rcs_openGC(display,vt100Window);
        for (y=0; y<=curLine; y++)
          for (x=0; x<80; x++) vidRam[y][x] = 32;
        rcs_namedForeground(display,gc,"black");
        rcs_drawFilledBox(display,vt100Buffer,gc,0,0,640,(1+curLine)<<4);
        rcs_drawFilledBox(display,vt100Window,gc,0,0,640,(1+curLine)<<4);
        rcs_closeGC(display,gc);
        escPos = -1;
        }
      else if (strcmp(escBuffer,"[2J") == 0) {
        gc = rcs_openGC(display,vt100Window);
        for (y=0; y<25; y++)
          for (x=0; x<80; x++) vidRam[y][x] = 32;
        rcs_namedForeground(display,gc,"black");
        rcs_drawFilledBox(display,vt100Buffer,gc,0,0,640,400);
        rcs_drawFilledBox(display,vt100Window,gc,0,0,640,400);
        rcs_closeGC(display,gc);
        curPos = 0;
        curLine = 0;
        escPos = -1;
        }
      else vterr();
      } else if (byt == 'J') vterr();
    if (byt == 'K' && escBuffer[0] == '[') {
      if (strcmp(escBuffer,"[K") == 0) {
        gc = rcs_openGC(display,vt100Window);
        for (i=curPos; i<80; i++) {
          vidRam[curLine][i] = 32;
          rcs_drawPixmap(display,vt100Buffer,gc,i<<3,curLine<<4,(const char**)charset[vidRam[curLine][i]]);
          rcs_drawPixmap(display,vt100Window,gc,i<<3,curLine<<4,(const char**)charset[vidRam[curLine][i]]);
          }
        rcs_closeGC(display,gc);
        escPos = -1;
        }
      else if (strcmp(escBuffer,"[1K") == 0) {
        gc = rcs_openGC(display,vt100Window);
        for (i=0; i<=curPos; i++) {
          vidRam[curLine][i] = 32;
          rcs_drawPixmap(display,vt100Buffer,gc,i<<3,curLine<<4,(const char**)charset[vidRam[curLine][i]]);
          rcs_drawPixmap(display,vt100Window,gc,i<<3,curLine<<4,(const char**)charset[vidRam[curLine][i]]);
          }
        rcs_closeGC(display,gc);
        escPos = -1;
        }
      else if (strcmp(escBuffer,"[2K") == 0) {
        gc = rcs_openGC(display,vt100Window);
        for (i=0; i<80; i++) {
          vidRam[curLine][i] = 32;
          rcs_drawPixmap(display,vt100Buffer,gc,i<<3,curLine<<4,(const char**)charset[vidRam[curLine][i]]);
          rcs_drawPixmap(display,vt100Window,gc,i<<3,curLine<<4,(const char**)charset[vidRam[curLine][i]]);
          }
        rcs_closeGC(display,gc);
        escPos = -1;
        }
      else vterr();
      } else if (byt == 'K') vterr();
    if (byt == 'f' && escBuffer[0] == '[') {
      pBuffer = getInteger(&escBuffer[1],&y);
      if (*pBuffer == ';') {
        pBuffer = getInteger(pBuffer+1,&x);
        } else x = 0;
      if (*pBuffer == byt) {
        curPos = (x < 80) ? x : 79;
        curLine = (y < 25) ? y : 24;
        escPos = -1;
        } else vterr();
      } else if (byt == 'f') vterr();
    if (byt == 's' && escBuffer[0] == '[') {
      if (escPos == 2) {
        savePos = curPos;
        saveLine = curLine;
        escPos = -1;
        } else vterr();
      } else if (byt == 's') vterr();
    if (byt == 'u' && escBuffer[0] == '[') {
      if (escPos == 2) {
        curPos = savePos;
        curLine = saveLine;
        escPos = -1;
        } else vterr();
      } else if (byt == 'u') vterr();
    showCursor();
    return;
    }
  if (byt < 32) {
    if (byt == 27) {
      escPos = 0;
      }
    if (byt == 13) {
      curPos = 0;
      }
    if (byt == 10) {
      if (++curLine == 25) {
        vt100ScrollUp();
        curLine = 24;
        }
      }
    if (byt == 8 && (curLine > 0 || curPos > 0)) {
      if (--curPos < 0) {
        curPos = 79;
        --curLine;
        }
      vidRam[curLine][curPos] = 32;
      }
    if (byt == 9) {
      curPos = (curPos + 8) & 0xf8;
      if (curPos >= 80) {
        curPos = 0;
        if (++curLine == 25) {
          vt100ScrollUp();
          curLine = 24;
          }
        }
      }

    showCursor();
    return;
    }
  if (byt == 0x7f) return;
  gc = rcs_openGC(display,vt100Buffer);
  rcs_drawPixmap(display,vt100Buffer,gc,curPos<<3,curLine<<4,(const char**)charset[byt]);
  rcs_drawPixmap(display,vt100Window,gc,curPos<<3,curLine<<4,(const char**)charset[byt]);
  rcs_closeGC(display,gc);
  vidRam[curLine][curPos] = byt;
  if (++curPos == 80) {
    curPos = 0;
    if (++curLine == 25) {
      vt100ScrollUp();
      curLine = 24;
      }
    }
  showCursor();
  }

void vt100Event(rcs_Event event) {
  rcs_GC gc;
    if (event.type == EVENT_EXPOSE) {
      gc = rcs_openGC(display,vt100Window);
      rcs_copyArea(display,vt100Buffer,vt100Window,gc,event.d1,event.d2,
                                            event.d3,event.d4,
                                            event.d1,event.d2);
      rcs_closeGC(display,gc);
      }
    else if (event.type == EVENT_KEY_PRESS) {
      if (event.d1 == KEY_CTRL_L ||
          event.d1 == KEY_CTRL_R) {
        ctrlState = 1;
        event.d1 = 0;
        }
      else if (event.d1 == KEY_F1) {
printf("F1 pressed - Receive mode\n");
        xmodemMode = 'R';
        xmodemCount = 0;
        vtOut = 0x15;
        xmodemFile = open("xmodem.dat",O_WRONLY | O_TRUNC | O_CREAT,0666);
        }
      else if (event.d1 == KEY_F2) {
printf("F2 pressed - Send mode\n");
        xmodemMode = 'S';
        xmodemCount = 128;
        xmodemBlock = 1;
        xmodemFile = open("xmodem.dat",O_RDONLY);
        }
      else if (event.d1 == KEY_F3) {
        xmodemMode = ' ';
        }
      else if (event.d1 == KEY_BACK_SP) event.d1 = 8;
      else if (event.d1 == KEY_TAB) event.d1 = 9;
      else {
        if (ctrlState == 1) {
          if (event.d1 >='a' && event.d1 <= 'z') event.d1 -= ('a' - 1);
          if (event.d1 >='A' && event.d1 <= 'Z') event.d1 -= ('A' - 1);
          } else {
          if (forceUC == 'Y' && event.d1 >= 'a' && event.d1 <= 'z')
            event.d1 -= 32;
          if (event.d1 > 255) event.d1 = 0;
          }
        }
      if (event.d1 != 0 && event.d1 < 128) {
        if (keyEnd != keyStart-1 ||
            (keyEnd ==15 && keyStart != 0)) {
          keyBuffer[keyEnd++] = event.d1;
          if (keyEnd == 16) keyEnd = 0;
          }
        }
      }
    else if (event.type == EVENT_KEY_RELEASE) {
      if (event.d1 == KEY_CTRL_L ||
          event.d1 == KEY_CTRL_R) ctrlState = 0;
      }
  }

int vt100GetKey() {
  int ret;
  if (keyStart == keyEnd) return -1;
  ret = keyBuffer[keyStart++];
  if (keyStart == 16) keyStart = 0;
  return ret;
  }

void vt100Reset() {
  escPos = -1;
  vtEnabled = 1;
  }

void vt100Init() {
  rcs_GC gc;
  int x,y;
  for (y=0; y<25; y++)
    for (x=0; x<80; x++) vidRam[y][x] = 32;
  ctrlState = 0;
  escPos = -1;
  curLine = 0;
  curPos = 0;
  saveLine = 0;
  savePos = 0;
  keyStart = 0;
  keyEnd = 0;
  xmodemMode = ' ';
  vt100Window = rcs_createWindow(display,rcs_rootWindow(display),10,10,640,400);
  rcs_setWindowName(display,vt100Window,"Vt-100");
  rcs_showWindow(display,vt100Window);
  rcs_moveWindow(display,vt100Window,vt100X,vt100Y);
  vt100Buffer = rcs_createPixmap(display,vt100Window,640,400);
  gc = rcs_openGC(display,vt100Buffer);
  rcs_namedForeground(display,gc,"black");
  rcs_drawFilledBox(display,vt100Buffer,gc,0,0,640,400);
  rcs_copyArea(display,vt100Buffer,vt100Window,gc,0,0,640,400,0,0);
  rcs_closeGC(display,gc);
  showCursor();
#ifdef WIN32
  vt100Buffer2 = rcs_createPixmap(display,vt100Window,640,400);
#endif

  }

void vt100Close() {
#ifdef WIN32
  rcs_closePixmap(display,vt100Buffer2);
#endif
  rcs_closePixmap(display,vt100Buffer);
  rcs_closeWindow(display,vt100Window);
  }

