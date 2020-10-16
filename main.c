/*
 *******************************************************************
 *** This software is copyright 2006 by Michael H Riley          ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

#define MAIN
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef WIN32
#include <io.h>
#include <sys/timeb.h>
#else
#include <unistd.h>
#include <sys/timeb.h>
#endif
#include <fcntl.h>
#include "elf.h"
#include "images/elf.xpm"
#include "images/elf2.xpm"
#include "images/studio2.xpm"
#include "images/comx35.xpm"
#include "images/superelf.xpm"
#include "images/eprom.xpm"
#include "images/hexpad.xpm"
#include "images/led.xpm"
#include "images/hex.xpm"
#include "images/debug.xpm"

byte minimon[] = { 0xf8, 0xff, 0xa1, 0xe1, 0x6c, 0x64, 0xa3, 0x21,
                   0x6c, 0x64, 0x3f, 0x07, 0x37, 0x0c, 0x3a, 0x11,
                   0xd3, 0xe3, 0xf6, 0x33, 0x17, 0x7b, 0x6c, 0x64,
                   0x23, 0x3f, 0x13, 0x37, 0x1b, 0x13, 0x30, 0x13 };

portStruct parseArgs(char* arg) {
  int        i;
  int        ip;
  int        op;
  int        ef;
  portStruct ret;
  ip = 0;
  op = 0;
  ef = 0;
  for (i=0; i<7; i++) {
    ret.inPorts[i] = 0;
    ret.outPorts[i] = 0;
    }
  for (i=0; i<4; i++) {
    ret.ef[i] = 0;
    }
  ret.q = 0;
  ret.intr = 0;
  while (*arg != 0 && *arg != '=') arg++;
  if (*arg == 0) return ret;
  arg++;
  while (*arg != 0) {
    if (*arg == 'o' || *arg == 'O') {
      arg++;
      if (*arg != 0) {
        i = *arg - '0';
        if (i >= 1 && i<= 7) ret.outPorts[op++] = i;
        } else arg--;
      }
    else if (*arg == 'i' || *arg == 'I') {
      arg++;
      if (*arg != 0) {
        i = *arg - '0';
        if (i >= 1 && i<= 7) ret.inPorts[op++] = i;
        } else arg--;
      }
    else if (*arg == 'e' || *arg == 'E') {
      arg++;
      if (*arg != 0) {
        i = *arg - '0';
        if (i >= 1 && i<= 4) ret.ef[ef++] = i;
        } else arg--;
      }
    else if (*arg == 'q' || *arg == 'Q') {
      ret.q = 1;
      }
    else if (*arg == 't' || *arg == 'T') {
      ret.intr = 1;
      }
    else if (*arg == ',');
    arg++;
    }
  return ret;
  }

void show_time() {
  int h,m,s;
  double f1,f2;
  if (cpu.cycles != 0) {
    endTime = time(NULL);
    s = endTime - startTime;
    h = s / 3600;
    s -= (h * 3600);
    m = s / 60;
    s -= (m * 60);
    f2 = endTime - startTime;
    f1 = cpu.cycles;
    f1 /= f2;
    f1 = f1 / 1000000 * 8;
#ifdef WIN32
    printf("Cycles executed: %ld\n",cpu.cycles);
#else
    printf("Cycles executed: %lld\n",cpu.cycles);
#endif
    printf("Run Time %2d:%2d:%2d\n",h,m,s);
    printf("Effective clock=%12.6f Mhz\n",f1);
    }
  }

void repaint(int x,int y,int w,int h) {
  int    i;
  rcs_copyArea(display,screen,window,gc,x,y,w,h,x,y);
  if (computer == '1') {
    pushPaint(&dmain,display,window);
    for (i=0;i<8;i++) togglePaint(&datasw[i],display,window);
    for (i=0;i<4;i++) togglePaint(&ef[i],display,window);
    }
  if (computer == '1' || computer == '2') {
    togglePaint(&mp,display,window);
    togglePaint(&load,display,window);
    togglePaint(&run,display,window);
    }
  if (computer == 'S') {
    for (i=0; i<8; i++) redLedPaint(&superLeds[i],display,window);
    }
  redLedPaint(&q,display,window);
  togglePaint(&power,display,window);
  if (computer != '2') for (i=0;i<4;i++) til311Paint(&addr[i],display,window);
  for (i=0;i<2;i++) til311Paint(&data[i],display,window);
  }

void paint(int x1,int y1,int x2,int y2) {
  rcs_GC gc;
  return;
  gc=rcs_openGC(display,window);
  rcs_copyArea(display,window,window,gc,x1,y1,(x2-x1),(y2-y1),x1,y1);
  rcs_closeGC(display,gc);
  }

byte getData() {
  if (computer != '1') return switches;
  return (toggleGetState(&datasw[7])?128:0)+
         (toggleGetState(&datasw[6])?64:0)+
         (toggleGetState(&datasw[5])?32:0)+
         (toggleGetState(&datasw[4])?16:0)+
         (toggleGetState(&datasw[3])?8:0)+
         (toggleGetState(&datasw[2])?4:0)+
         (toggleGetState(&datasw[1])?2:0)+
         (toggleGetState(&datasw[0])?1:0)
         ;
  }

void showData(byte val) {
  int i;
  byte temp[2];
  temp[0]=data[0].number;
  temp[1]=data[1].number;
  data[0].number=(val>>4)&15;
  data[1].number=val&15;
  for (i=0;i<2;i++) {
    if (temp[i] != data[i].number) {
      til311Paint(&data[i],display,window);
      }
    }
  }

void showAddr(word adr) {
  int i;
  byte temp[4];
  if (computer == '2') return;
  for (i=0;i<4;i++) temp[i]=addr[i].number;
  addr[0].number=adr>>12;
  addr[1].number=(adr>>8)&15;
  addr[2].number=(adr>>4)&15;
  addr[3].number=adr&15;
  if (cpu.mode != 'R' || showAddress == 'Y') 
  for (i=0;i<4;i++) {
    if (addr[i].number != temp[i]) {
       til311Update(&addr[i],display,window);
/*
       til311Paint(&addr[i],display,window);
*/
       }
    }
  }

void writeMem(word addr,byte value) {
  unsigned int extAddr;
if (memTrap == 1 && addr >= 0x203d && addr <= 0x3060) {
  if (addr != 0x276b && addr != 0x276c) {
  steps = 0;
  debugDisplay("Memory Trap");
  }
  }
  showAddr(addr);
  cpu.address = addr;
  if (use1870 == 'Y') {
    if (addr >= 0xf400 && addr <= 0xffff) {
      v1870_write(addr,value);
      return;
      }
    }
  if (useEMS == 'Y') { 
    if (addr>=0x8000 && addr <=0xbfff) {
      ems[(long)((addr & 0x3fff) | (emsPage << 14))] = value;
      return;
      }
    if (addr>=0xc000 && addr <=0xffff) {
      emsPage = value;
      }
    }
  if (use6845 == 'Y' && addr >= 0xe000 && addr <= 0xe000+1024) {
    v6845Write(addr-0xe000,value);
    ram[addr] = value;
    return;
    }
  if (romMap[addr >> 3] & (1<<(addr & 7))) return;
  if (!ramMap[addr >> 3] & (1<<(addr & 7))) return;
  if (usePager == 'Y') {
    extAddr = (pager[addr>>12] << 12) | (addr &0xfff);
    ram[extAddr] = value;
    }
  if (addr < 32 && monitor == 'Y') return;
  if (toggleGetState(&mp)) return;
  ram[addr]=value;
  }

byte readMem(word addr) {
  unsigned int extAddr;
  showAddr(addr);
  cpu.address = addr;
  if (use1870 == 'Y') {
    if (addr >= 0xf400 && addr <= 0xffff) {
      return v1870_read(addr);
      }
    }
  if (useEMS == 'Y' && addr>=0x8000 && addr <=0xbfff) {
    return ems[(long)((addr & 0x3fff) | (emsPage << 14))];
    }
  if (usePager == 'Y') {
    extAddr = (pager[addr>>12] << 12) | (addr &0xfff);
    return ram[extAddr];
    }
  if (use6845 == 'Y' && addr >= 0xe000 && addr <= 0xe000+1024) {
    return ram[addr];
    }
  if (romMap[addr >> 3] & (1<<(addr & 7))) return ram[addr];
  if ((ramMap[addr >> 3] & (1<<(addr & 7))) == 0) return 255;
  if (addr <32 && monitor == 'Y') return minimon[addr];
  return ram[addr];
  }

byte readPort(byte port) {
  byte ret;
  ret = 255;
  if (cidelsa == 'Y') {
    if (port == 1) return cid1;
    if (port == 2) return cid2;
    if (port == 4) return cid4;
    }
  if (inPorts[port-1].in != NULL) ret = inPorts[port-1].in();
  inValues[port] = ret;
  return ret;
  }

void writePort(byte port,byte value) {
  outValues[port] = value;
  if (outPorts[port-1].out != NULL) outPorts[port-1].out(value);
/*
  if (port == 5) extPort = value;
  if (port == 6) {
    if (extPort >=1 && extPort <=15) {
      pager[extPort] = value;
      }
    return;
    }
*/
  }

void debug(char* msg) {
  if (trace != 'N') printf("%s",msg);
  }

void init() {
  int i;
  if (computer == '2') machine = newElf2();
  else if (computer == 'S') machine = newSuper();
  else if (computer == '1') machine = newHbElf();
  else if (computer == 'T') machine = newStudio2();
  else if (computer == 'X') machine = newComx35();
  cpu=cdp1802New();
  cpu.readMem=readMem;
  cpu.writeMem=writeMem;
  cpu.readPort=readPort;
  cpu.writePort=writePort;
  machine.cpu = &cpu;
  switches = 0;
  for (i=0; i<16; i++) cpu.r[i] = 0;
  if (debugMode=='Y') cpu.debug=debug;
  machine.init();
  }

void machineCycle(CDP1802* cpu,int cycle) {
  int i;
  for (i=0; i<7; i++) {
    if (inPorts[i].cycle != NULL) inPorts[i].cycle();
    if (outPorts[i].cycle != NULL) outPorts[i].cycle();
    }
  for (i=0; i<4; i++) {
    if (efDevices[i].cycle != NULL) efDevices[i].cycle();
    }
  if (qDevice.cycle != NULL) qDevice.cycle();
        if (efDevices[0].ef1 != NULL) {
          cdp1802SetEF(cpu,1,efDevices[0].ef1() ^ ef1Sense);
          } else cdp1802SetEF(cpu,1,1);
        if (efDevices[1].ef2 != NULL) {
          cdp1802SetEF(cpu,2,efDevices[1].ef2() ^ ef2Sense);
          } else cdp1802SetEF(cpu,2,1);
        if (efDevices[2].ef3 != NULL) {
          cdp1802SetEF(cpu,3,efDevices[2].ef3() ^ ef3Sense);
          } else cdp1802SetEF(cpu,3,1);
        if (efDevices[3].ef4 != NULL) {
          cdp1802SetEF(cpu,4,efDevices[3].ef4() ^ ef4Sense);
          } else cdp1802SetEF(cpu,4,((inPressed == 'Y') ? 0 : 1 ));
  }

void runMachine() {
  FILE*     config;
  rcs_Event event;
  char      flag;
  int       i;
  int       j;
/*
  char      pressed;
  char      inPressed;
  char      lastMode2;
  char      singleStep;
  char      state;
*/
  char      lastMode;
  char      buffer[64];
  char      buffer2[64];
#ifdef WIN32
  DWORD     tb;
#else
  struct timeb tb;
#endif
  if (revQ) cpu.q=1;
  instCount = 1;
  lastMode = ' ';
  flag=' ';
  pressed='N';
  inPressed='N';
  monitor = 'N';
  singleStep = 0;
  cpu.ef = 0xf;
  if (useVt100 == 'Y') cdp1802SetEF(&cpu,4,0);
  if (computer == 'T') {
     cdp1802SetWait(&cpu,1);
     cdp1802SetClear(&cpu,0);
     cdp1802SetWait(&cpu,1);
     cdp1802SetClear(&cpu,1);
     cpu.cycles = 0;
     startTime = time(NULL);
     }
  while (flag==' ') {
    cpu.cycleCount = 0;
/*
if (debugMode == 'Y') {
  printf("d=%2x x=%2x ef=%x r0=%4x r1=%4x r4=%4x\n",cpu.d,cpu.x,cpu.ef,
         cpu.r[0],cpu.r[2],cpu.r[4]);
  }
*/
    if (computer == 'S' && cpu.mode != lastMode) {
      redLedSetState(&superLeds[0],0);
      redLedSetState(&superLeds[1],0);
      redLedSetState(&superLeds[2],0);
      redLedSetState(&superLeds[3],0);
      switch(cpu.mode) {
        case 'L':redLedSetState(&superLeds[0],1); break;
        case 'I':redLedSetState(&superLeds[1],1); break;
        case 'R':redLedSetState(&superLeds[2],1); break;
        case 'P':redLedSetState(&superLeds[3],1); break;
        }
      redLedPaint(&superLeds[0],display,window);
      redLedPaint(&superLeds[1],display,window);
      redLedPaint(&superLeds[2],display,window);
      redLedPaint(&superLeds[3],display,window);
      lastMode = cpu.mode;
      }
    if (cpu.mode == 'R' && singleStep != 0 && state == 'F') {
      redLedSetState(&superLeds[4],1);
      redLedSetState(&superLeds[5],0);
      redLedPaint(&superLeds[4],display,window);
      redLedPaint(&superLeds[5],display,window);
      state = 'E';
      cdp1802SetWait(&cpu,0);
      }
    if (debugOpen == 'Y') updateDebugger();
    if (cpu.mode == 'R') {
      if (steps != 0) {
        cycle0=0;
if (cidelsa == 'Y') {
  if (cidEF2 != 0) cpu.ef |=2;
    else cpu.ef &= 0xd;
  }
        machineCycle(&cpu,0);
        if (cycle0 == 0) machineCycle(&cpu,1);
        if (cycle0 == 0 && steps != 0) {
          cpu.cycleCount += 2;
          cdp1802Cycle(&cpu);
          cpu.cycles += 2;
          }

        }
      if (steps != 0 && numBP > 0) {
        for (i=0; i<numBP; i++)
          if (breakPoints[i] == cpu.r[cpu.p]) {
            steps = 0;
            if (lastMsg != MSG_BP) {
              debugDisplay("Hit Breakpoint");
              lastMsg = MSG_BP;
              }
            }
        }
      if (steps != 0 && numTraps > 0) {
        for (i=0; i<numTraps; i++)
          if ((traps[i][0] == 1 && traps[i][1] == ram[cpu.r[cpu.p]]) ||
              (traps[i][0] == 2 && traps[i][1] == ram[cpu.r[cpu.p]] &&
                                   traps[i][2] == ram[cpu.r[cpu.p]+1]) ||
              (traps[i][0] == 3 && traps[i][1] == ram[cpu.r[cpu.p]] &&
                                   traps[i][2] == ram[cpu.r[cpu.p]+1] &&
                                   traps[i][3] == ram[cpu.r[cpu.p]+2])) {
            steps = 0;
            if (lastMsg != MSG_INST_TRAP) {
              debugDisplay("Instruction Trap");
              lastMsg = MSG_INST_TRAP;
              }
            }
        }
      if (steps != 0 && numTregs > 0) {
        j = -1;
        strcpy(buffer2,"Register Trap: ");
        for (i=0; i<numTregs; i++) {
          if (tregs[i][0] == 'D' && tregs[i][1] == cpu.d) {
             sprintf(buffer,"D=%02X",cpu.d);
             j = i;
             }
          if (tregs[i][0] == 'F' && tregs[i][1] == cpu.df) {
             sprintf(buffer,"DF=%X",cpu.df);
             j = i;
             }
          if (tregs[i][0] == 'P' && tregs[i][1] == cpu.p) {
             sprintf(buffer,"P=%X",cpu.p);
             j = i;
             }
          if (tregs[i][0] == 'X' && tregs[i][1] == cpu.x) {
             sprintf(buffer,"X=%X",cpu.x);
             j = i;
             }
          if (tregs[i][0] == 'T' && tregs[i][1] == cpu.t) {
             sprintf(buffer,"X=%02X",cpu.t);
             j = i;
             }
          if (tregs[i][0] == 'Q' && tregs[i][1] == cpu.q) {
             sprintf(buffer,"Q=%X",cpu.q);
             j = i;
             }
          if (tregs[i][0] >= '0' && tregs[i][0] <= '9' &&
              tregs[i][1] == cpu.r[tregs[i][0]-'0']) {
             sprintf(buffer,"R%X=%02X",tregs[i][0]-'0',tregs[i][1]);
             j = i;
             }
          if (tregs[i][0] >= 'a' && tregs[i][0] <= 'f' &&
              tregs[i][1] == cpu.r[tregs[i][0]-87]) {
             sprintf(buffer,"R%X=%02X",tregs[i][0]-87,tregs[i][1]);
             j = i;
             }
          }
        if (j >= 0) {
          steps = 0;
          if (lastMsg != MSG_REG_TRAP) {
            strcat(buffer2,buffer);
            debugDisplay(buffer2);
            lastMsg = MSG_REG_TRAP;
            }
          }
        }
      if (steps > 0) steps--;
      state = 'F';
      if (singleStep != 0) {
        redLedSetState(&superLeds[4],0);
        redLedSetState(&superLeds[5],1);
        redLedPaint(&superLeds[4],display,window);
        redLedPaint(&superLeds[5],display,window);
        }
      iCount++;
      if (singleStep) {
        cdp1802SetWait(&cpu,0);
        }

      if (fixedClock && (instCount-=cpu.cycleCount) <= 0) {
        instCount = fixedClock;
#ifdef WIN32
		tb = GetTickCount()/50;
		while (lastMS == tb) tb = GetTickCount()/50;
		lastMS = tb;
#else
        ftime(&tb);
        while (lastMS == tb.millitm) ftime(&tb);
        lastMS = tb.millitm;
#endif
        }

      }
      else {
      graphicsOn = 'N';
      cpu.cycles = 0;
      pixieEF = 1;
      graphicsNext = 0;
      graphicsMode = 0;
      startTime = time(NULL);
      if (iCount != 0) { iCount = 0; }
      if (cpu.mode == 'L') {
        showData(readMem(cpu.address));
        }
      }
    if (cpu.q != q.state) {
      q.state=cpu.q;
      redLedPaint(&q,display,window);
      }
/*
    sched_yield();
*/
    event=rcs_pollEvent(display);
	if (event.type == EVENT_QUIT) {
		flag = '*';
	}
	if (event.type == EVENT_CLOSE) {
		flag = '*';
	}
  if (event.type == EVENT_CONFIGURE) {
    if (event.window == pixieWindow) {
      if (event.d1 != 0 && event.d2 != 0) {
        pixieX = event.d1;
        pixieY = event.d2;
        }
      }
    else if (event.window == hexWindow) {
      if (event.d1 != 0 && event.d2 != 0) {
        hexX = event.d1;
        hexY = event.d2;
        }
      }
    else if (event.window == vt100Window) {
      if (event.d1 != 0 && event.d2 != 0) {
        vt100X = event.d1;
        vt100Y = event.d2;
        }
      }
    else {
      if (event.d1 != 0 && event.d2 != 0) {
        mainX = event.d1;
        mainY = event.d2;
        }
      }
#ifdef WIN32
    config = fopen("elfrc","w");
#else
    config = fopen("./elfrc","w");
#endif
    fprintf(config,"%d %d %d %d %d %d %d %d\n",pixieX,pixieY,hexX,hexY,vt100X,vt100Y,
                                       mainX,mainY);
    fclose(config);
    }

    if (useVt100 == 'Y' && vtOut <= 0) {
      vtOut = vt100GetKey();
      }
   
    if (event.window == hexWindow) keypadEvent(event);
    else if (event.window == ledWindow) ledEvent(event);
    else if (event.window == hexOutWindow) hexOutEvent(event);
    else if (event.window == pixieWindow) pixieEvent(event);
    else if (event.window == vt100Window) vt100Event(event);
    else if (event.window == v6845Window) v6845OutEvent(event);
    else if (event.window == v1870Window) v1870_event(event);
    else if (event.window == debugWindow) debugEvent(event);
    else if (event.window == clockWindow) clockEvent(event);
    else if (event.window == vdp.window) tms9918_event(event);
    else {
      control = 0;
      if (event.type == EVENT_EXPOSE) {
        repaint(event.d1,event.d2,event.d3,event.d4);
        }
      if (event.type == EVENT_KEY_RELEASE) {
        if (cidelsa == 'Y') {
          if (event.d1 == 13) cidEF2 = 2;
          if (event.d1 == '1') cid1 |= 2;
          if (event.d1 == '2') cid1 |= 4;
          if (event.d1 == KEY_LEFT) cid1 |= 16;
          if (event.d1 == KEY_RIGHT) cid1 |= 8;
          if (event.d1 == KEY_UP) cid4 |= 1;
          if (event.d1 == KEY_DOWN) cid4 |= 2;
          if (event.d1 == ' ') cid1 |= 32;
          }
        if (event.d1 == KEY_CTRL_L ||
            event.d1 == KEY_CTRL_R) ctrlState = 0;
        if (usePS2 == 'Y') ps2Event = event;
        }
      if (event.type == EVENT_KEY_PRESS) {
        if (cidelsa == 'Y') {
          if (event.d1 == 13) cidEF2 = 0;
          if (event.d1 == '1') cid1 &= 0xfd;
          if (event.d1 == '2') cid1 &= 0xfb;
          if (event.d1 == KEY_LEFT) cid1 &= 0xef;
          if (event.d1 == KEY_RIGHT) cid1 &= 0xf7;
          if (event.d1 == KEY_UP) cid4 &= 0xfe;
          if (event.d1 == KEY_DOWN) cid4 &= 0xfd;
          if (event.d1 == ' ') cid1 &= 0xdf;
          }
        if (event.d1 == KEY_CTRL_L ||
            event.d1 == KEY_CTRL_R) {
          ctrlState = 1;
          event.d1 = 0;
          }
        if (event.d1 >= 'A' && event.d1 <= 'Z' && ctrlState ==1)
          event.d1 -= ('A'-1);
        if (event.d1 >= 'a' && event.d1 <= 'z' && ctrlState ==1)
          event.d1 -= ('a'-1);
        if (event.d1 == KEY_F1) {
          if (debugOpen == 'Y') {
            debugOpen = 'N';
            rcs_hideWindow(display,debugWindow);
            } else {
            debugOpen = 'Y';
            rcs_showWindow(display,debugWindow);
            }
          event.d1 = 0;
          }
        if (event.d1 == KEY_F2) {
          if (clockOpen == 'Y') {
            clockOpen = 'N';
            rcs_hideWindow(display,clockWindow);
            } else {
            clockOpen = 'Y';
            showClock();
            rcs_showWindow(display,clockWindow);
            }
          event.d1 = 0;
          }
        if (event.d1 == KEY_F9) {
          flag = '*';
          }
        if (usePS2 == 'Y') ps2Event = event;
        if (event.d1 < 128 || computer == 'X') keyboard = event.d1;
/*        if (event.d1 < 128) keyboard = event.d1; */
	if (event.d1 == KEY_BACK_SP) event.d1 = 8;

        if (useKeyboard == 'N' && usePS2 != 'Y') {
          switch(keyboard) {
            case '0':switches = ((switches << 4) & 0xf0) | 0; break;
            case '1':switches = ((switches << 4) & 0xf0) | 1; break;
            case '2':switches = ((switches << 4) & 0xf0) | 2; break;
            case '3':switches = ((switches << 4) & 0xf0) | 3; break;
            case '4':switches = ((switches << 4) & 0xf0) | 4; break;
            case '5':switches = ((switches << 4) & 0xf0) | 5; break;
            case '6':switches = ((switches << 4) & 0xf0) | 6; break;
            case '7':switches = ((switches << 4) & 0xf0) | 7; break;
            case '8':switches = ((switches << 4) & 0xf0) | 8; break;
            case '9':switches = ((switches << 4) & 0xf0) | 9; break;
            case 'A':switches = ((switches << 4) & 0xf0) | 10; break;
            case 'B':switches = ((switches << 4) & 0xf0) | 11; break;
            case 'C':switches = ((switches << 4) & 0xf0) | 12; break;
            case 'D':switches = ((switches << 4) & 0xf0) | 13; break;
            case 'E':switches = ((switches << 4) & 0xf0) | 14; break;
            case 'F':switches = ((switches << 4) & 0xf0) | 15; break;
            case 'a':switches = ((switches << 4) & 0xf0) | 10; break;
            case 'b':switches = ((switches << 4) & 0xf0) | 11; break;
            case 'c':switches = ((switches << 4) & 0xf0) | 12; break;
            case 'd':switches = ((switches << 4) & 0xf0) | 13; break;
            case 'e':switches = ((switches << 4) & 0xf0) | 14; break;
            case 'f':switches = ((switches << 4) & 0xf0) | 15; break;
            case 13:inPressed='Y';
                    if (cpu.mode == 'L') {
                      cdp1802DmaIn(&cpu,getData());
                      showData(getData());
                      } else {
                      toggleSetState(&ef[3],0);
                      }
                    break;
            }
          keyboard = 0;
          }
        }
      machine.event(event);
      if (control == OFF) {
        flag='*';
        }
      }
    }
  }

char *convHex(char* buffer,word* adr) {
  word ret;
  ret=0;
  while (*buffer != 0 && !isxdigit(*buffer)) buffer++;
  while (isxdigit(*buffer)) {
    ret*=16;
    switch (*buffer) {
      case '1':ret+=1; break;
      case '2':ret+=2; break;
      case '3':ret+=3; break;
      case '4':ret+=4; break;
      case '5':ret+=5; break;
      case '6':ret+=6; break;
      case '7':ret+=7; break;
      case '8':ret+=8; break;
      case '9':ret+=9; break;
      case 'a':
      case 'A':ret+=10; break;
      case 'b':
      case 'B':ret+=11; break;
      case 'c':
      case 'C':ret+=12; break;
      case 'd':
      case 'D':ret+=13; break;
      case 'e':
      case 'E':ret+=14; break;
      case 'f':
      case 'F':ret+=15; break;
      }
    buffer++;
    }
  *adr=ret;
  return buffer;
  }

void addRam(char* buffer) {
  long i;
  word start,end;
  buffer = convHex(buffer, &start);
  buffer++;
  buffer = convHex(buffer, &end);
  for (i=start; i<=end; i++)
      ramMap[i >> 3] |= (1<<(i & 7));
  }

void read_intel(char* buffer,char mode) {
  int i;
  word count;
  word addrs;
  word value;
  char buf2[255];
  buffer++;
  strncpy(buf2,buffer,2);
  buffer += 2;
  buf2[2] = 0;
  convHex(buf2,&count);
  strncpy(buf2,buffer,4);
  buffer += 4;
  buf2[4] = 0;
  convHex(buf2,&addrs);
  strncpy(buf2,buffer,2);
  buffer += 2;
  buf2[2] = 0;
  convHex(buf2,&value);
  if (value != 0) return;
  for (i=0; i<count; i++) {
    strncpy(buf2,buffer,2);
    buffer += 2;
    buf2[2] = 0;
    convHex(buf2,&value);
    ram[addrs] = (byte)value;
    if (mode == 'R') {
      romMap[addrs >> 3] |= (1<<(addrs & 7));
      }
    addrs++;
    }
  }

void read_program(char* filename,char mode)
{
  int  i;
  int  spaces;
  word addrs;
  word value;
  char buffer[1024];
  char *pBuffer;
  FILE *infile;
  if ((infile=fopen(filename,"r"))==0)
    printf("File not found - %s\n",buffer);
  else {
    addrs=0x0000;
    while ((fgets(buffer,1000,infile)) != NULL) {
      spaces = 0;
      for (i=0; i<6; i++) if (buffer[i] == 32) spaces++;
      if (spaces == 0) read_intel(buffer,mode);
      else {
        pBuffer=buffer;
        while (*pBuffer != 0) {
          if (pBuffer[0] == ':') {
            pBuffer=convHex(&pBuffer[1],&addrs);
            } else {
            pBuffer=convHex(pBuffer,&value);
            value &= 255;
            ram[addrs]=(byte)value;
            if (mode == 'R') {
              romMap[addrs >> 3] |= (1<<(addrs & 7));
              }
            addrs++;
            }
          while (*pBuffer != 0 && !isxdigit(*pBuffer)) pBuffer++;
          }
        }
      }
    fclose(infile);
    }
}

int figureRam(char* buffer) {
  int ret;
  buffer++;
  ret = 0;
  while (*buffer != 0) {
    if (*buffer >='0' && *buffer <='9') {
      ret = ret*10 + (*buffer - '0');
      }
    if (*buffer == 'k' || *buffer == 'K') {
      ret *= 1024;
      if (ret >= 256 && ret <=65536) return ret;
      return 256;
      }
    buffer++;
    }
  return 256;
  }

void configureBaud(char* buffer) {
  buffer+=6;
  if (strcmp(buffer,"2400") == 0) baud = 104;
  if (strcmp(buffer,"1200") == 0) baud = 208;
  if (strcmp(buffer,"600") == 0) baud = 416;
  if (strcmp(buffer,"300") == 0) baud = 832;
  }

void read_st2(char *fname) {
  int file;
  int i;
  struct {
    char header[4];
    byte numBlocks;
    byte format;
    byte video;
    byte notUsed1;
    char author[2];
    char dumper[2];
    byte notUsed2[4];
    char catalogue[10];
    byte notUsed3[6];
    char title[32];
    byte offsets[64];
    byte notUsed4[128];
    } st2Header;
  file = open(fname,O_RDONLY);
  if (file == -1) {
    printf("Could not read rom %s\n",fname);
    return;
    }
  read(file,&st2Header,256);
  for (i=1; i<st2Header.numBlocks; i++)
    read(file,&(ram[st2Header.offsets[i-1] << 8]),256);
  close(file);
  }

int main(int argc,char *argv[]) {
  FILE* config;
  double clk;
  char   *displayname;
  int i;
  for (i=0; i<17; i++) strcpy(debugLines[i],"");
  for (i=0; i<8192; i++) romMap[i] = 0;
  for (i=0; i<8192; i++) ramMap[i] = 0;
  for (i=0; i<7; i++) {
    inPorts[i].in = NULL;
    inPorts[i].out = NULL;
    inPorts[i].init = NULL;
    inPorts[i].cycle = NULL;
    inPorts[i].q = NULL;
    inPorts[i].ef1 = NULL;
    inPorts[i].ef2 = NULL;
    inPorts[i].ef3 = NULL;
    inPorts[i].ef4 = NULL;
    outPorts[i].in = NULL;
    outPorts[i].out = NULL;
    outPorts[i].init = NULL;
    outPorts[i].cycle = NULL;
    outPorts[i].q = NULL;
    outPorts[i].ef1 = NULL;
    outPorts[i].ef2 = NULL;
    outPorts[i].ef3 = NULL;
    outPorts[i].ef4 = NULL;
    }
  for (i=0; i<4; i++) {
    efDevices[i].in = NULL;
    efDevices[i].out = NULL;
    efDevices[i].init = NULL;
    efDevices[i].cycle = NULL;
    efDevices[i].q = NULL;
    efDevices[i].ef1 = NULL;
    efDevices[i].ef2 = NULL;
    efDevices[i].ef3 = NULL;
    efDevices[i].ef4 = NULL;
    }
  qDevice.in = NULL;
  qDevice.out = NULL;
  qDevice.init = NULL;
  qDevice.cycle = NULL;
  qDevice.q = NULL;
  qDevice.ef1 = NULL;
  qDevice.ef2 = NULL;
  qDevice.ef3 = NULL;
  qDevice.ef4 = NULL;
  inPorts[3].in = getData;
  outPorts[3].out = showData;
  i=1;
  cid1 = 0xff;
  cid2 = 0xcf;
  cid4 = 0xff;
  cidEF2 = 0;
  revQ = 0;
  eprom='N';
memTrap = 0;
  debugLog = 'N';
  keypad='N';
  debugMode='N';
  ledModule='N';
  hexOut='N';
  graphics = 'N';
  graphicsOn = 'N';
  showAddress = 'Y';
  useKeyboard = 'N';
  usePrinter = 'N';
  usePS2 = 'N';
  use1870 = 'N';
  computer = '1';
  keyboard = 0;
  useVt100 = 'N';
  maxRam=256;
  baud = 208;
  iCount = 0;
  parPort = 0x80;
  cpu.cpuMode = '2';
  forceUC = 'N';
  usePager = 'N';
  fdcMode = 0;
  startTime = 0;
  endTime = 0;
  vScale = 1;
  extPort = 0;
  romLo = 0xffff;
  romHi = 0;
  trace = 'N';
  use6845 = 'N';
  steps = -1;
  numBP = 0;
  numTraps = 0;
  keyFile = 0;
  cidelsa = 'N';
  charLine = 64;
  debugLine[0] = 0;
  debugAsm = 'N';
  ef1Sense = 1;
  ef2Sense = 1;
  ef3Sense = 1;
  ef4Sense = 1;
  fixedClock = 0;
  serialLog = 'N';
  superBoard = 'N';
  ps2Event.type = 0;
  useTMS9918 = 'N';
  useGray = 'N';
  comxVidMode = 0;
  useZoom = 'N';
  useEMS = 'N';
  v1870Pcb = 'N';
  emsPage = 0;
  strcpy(diskName,"disk1.ide");
  ram = (byte*)malloc(65536);
  for (i=0; i<32768; i++) ram[i] = 0;
  i = 1;
  while (i<argc) {
    if (strncmp(argv[i],"-kb",3) == 0) configureKeyboard(argv[i]);
    if (strncmp(argv[i],"-ps2",4) == 0) configurePs2Keyboard(argv[i]);
    if (strcmp(argv[i],"-a") == 0) showAddress = 'N';
    if (strcmp(argv[i],"-cid") == 0) cidelsa = 'Y';
    if (strcmp(argv[i],"-v") == 0) printf("RC/Elf v2.6\n");
    if (strcmp(argv[i],"-d") == 0) { debugMode='Y'; steps = 0; }
    if (strncmp(argv[i],"-e",2) == 0) configurePortExt(argv[i]);
    if (strcmp(argv[i],"-m") == 0) usePager = 'Y';
    if (strcmp(argv[i],"-k") == 0) keypad='Y';
    if (strcmp(argv[i],"-ref1") == 0) ef1Sense = 0;
    if (strcmp(argv[i],"-ref2") == 0) ef2Sense = 0;
    if (strcmp(argv[i],"-ref3") == 0) ef3Sense = 0;
    if (strcmp(argv[i],"-ref4") == 0) ef4Sense = 0;
    if (strcmp(argv[i],"-pal") == 0) comxVidMode = 1;
    if (strcmp(argv[i],"-pcb") == 0) v1870Pcb = 'Y';
    if (strcmp(argv[i],"-rq") == 0) revQ = 1;
    if (strcmp(argv[i],"-z") == 0) useZoom = 'Y';
    if (strncmp(argv[i],"-1861",5) == 0) configurePixie(argv[i]);
    if (strcmp(argv[i],"-6847") == 0) use6845 = 'Y';
    if (strcmp(argv[i],"-1870") == 0) v1870_configure(argv[i]);
    if (strcmp(argv[i],"-slog") == 0) serialLog = 'Y';
    if (strcmp(argv[i],"-dlog") == 0) debugLog = 'Y';
    if (strcmp(argv[i],"-ems") == 0) useEMS = 'Y';
    if (strncmp(argv[i],"-disk=",6) ==0) {
      strcpy(diskName,argv[i]+6);
      ide.drive[0] = diskName;
      }
    if (strcmp(argv[i],"-6847=32") == 0) {
      use6845 = 'Y';
      charLine = 32;
      }
    if (strncmp(argv[i],"-9918",5) == 0) tms9918_configure(argv[i]);
    if (strncmp(argv[i],"-9918g",6) == 0) {
      useGray = 'Y';
      tms9918_configure(argv[i]);
      }
    if (strncmp(argv[i],"-p",2) == 0 &&
      argv[i][2] != 's') configurePrinter(argv[i]);
    if (strncmp(argv[i],"-lm",3) == 0) configureLedModule(argv[i]);
    if (argv[i][0] == '-' && argv[i][1] >= '0' && argv[i][1] <= '9' &&
      (strchr(argv[i],'K') != NULL || strchr(argv[i],'k') != NULL)) 
      maxRam = figureRam(argv[i]);
    if (strcmp(argv[i],"-elf2") == 0) computer='2';
    if (strcmp(argv[i],"-super") == 0) computer='S';
    if (strcmp(argv[i],"-studio2") == 0) {
      configurePixie("-1861");
      configurePortStudio2();
      computer='T';
      maxRam = 4096;
      showAddress = 'N';
      fixedClock = 224;
#ifdef WIN32
      fixedClock *= 50;
#endif
      }
    if (strncmp(argv[i],"-vt100",6) == 0) configureVt100(argv[i]);
    if (strcmp(argv[i],"-1805") == 0) cpu.cpuMode = '5';
    if (strncmp(argv[i],"-fdc",4) == 0) configureFdc(argv[i]);
    if (strncmp(argv[i],"-ide",4) == 0) configureIde(argv[i]);
    if (strcmp(argv[i],"-uc") == 0) forceUC = 'Y';
    if (strcmp(argv[i],"-sb") == 0) {
      superBoard = 'Y';
      maxRam = 4096;
      addRam("9800-98ff");
      read_program("supermon.hex",'R');
      }
    if (strncmp(argv[i],"-baud=",6) == 0) configureBaud(argv[i]);
    if (strcmp(argv[i],"-c") == 0) {
      ++i;
      clk = atof(argv[i]);
      clk *= 125;
      fixedClock = (int)clk;
#ifdef WIN32
      fixedClock *= 50;
#endif
      }
    if (strcmp(argv[i],"-ram") == 0) addRam(argv[++i]);
    if (strcmp(argv[i],"-r") == 0) {
      read_program(argv[++i],' ');
      }
    if (strcmp(argv[i],"-R") == 0) {
      read_program(argv[++i],'R');
      eprom='Y';
      }
    if (strcmp(argv[i],"-st2") == 0) {
      read_st2(argv[++i]);
      }
    if (strcmp(argv[i],"-t") == 0) {
      keyFile = open(argv[++i],O_RDONLY);
      if (keyFile < 0) keyFile = 0;
      }
    if (strcmp(argv[i],"-comx") == 0) {
      maxRam = 65536;
      addRam("4000-bfff");
      read_program("comx35.rom",'R');
      showAddress = 'N';
      v1870_configure(argv[i]);
      configureComx(argv[i]);
      computer='X';
      v1870Pcb = 'Y';
      fixedClock = 224;
#ifdef WIN32
      fixedClock *= 50;
#endif
      }
    if (strcmp(argv[i],"-sc") == 0) {
      vScale = atoi(argv[++i]);
      if (vScale < 1) vScale = 1;
      }
    i++;
    }
  for (i=0; i<(maxRam>>3); i++) ramMap[i]=255;
  if (fdcMode == 'R') {
    fdc_reset(&fdc);
    strcpy(fdc.drives[0],"disk1.dsk");
    strcpy(fdc.drives[1],"disk2.dsk");
    strcpy(fdc.drives[2],"disk3.dsk");
    strcpy(fdc.drives[3],"disk4.dsk");
    fdc.registerSelect = 0;
    }
  if (usePager == 'Y') ram = (byte*)realloc(ram,1048576);
  if (useEMS == 'Y') ems = (byte*)malloc(524288);
  displayname = getenv("DISPLAY");
  if (displayname == NULL) strcpy(displayname,":0.0");
  display=rcs_openDisplay(displayname);
  wx = 353; wy = 480;
  mainX = 5; mainY = 25;
  if (computer == '2') { wx = 523; wy = 386; }
  if (computer == 'S') { wx = 523; wy = 386; }
  if (computer == 'T') { wx = 414; wy = 209; }
  if (computer == 'X') { wx = 528; wy = 230; }
  hexX = mainX + wx + 10;
  hexY = mainY;
  pixieX = mainX + wx + 10 + ((keypad == 'Y') ? 188 : 0);
  pixieY = mainY;
  vt100X = mainX;
#ifdef WIN32
  vt100Y = mainY + wy + 10;
#else
  vt100Y = mainY + wy + 30;
#endif
  if (computer == '1') {
    vt100X = mainX + wx + 10;
    vt100Y = mainY;
    hexX = mainX;
#ifdef WIN32
    hexY = mainY + wy + 10;
#else
    hexY = mainY + wy + 30;
#endif
    pixieX = mainX + ((keypad == 'Y') ? 188 : 0);
    pixieY = hexY;
    }
#ifdef WIN32
  config = fopen("elfrc","r");
#else
  config = fopen("./elfrc","r");
#endif
  if (config != NULL) {
    fscanf(config,"%d %d %d %d %d %d %d %d\n",&pixieX,&pixieY,&hexX,&hexY,
                                       &vt100X,&vt100Y,&mainX,&mainY);
    fclose(config);
    }
  window=rcs_createWindow(display,rcs_rootWindow(display),10,10,wx,wy);
  rcs_setWindowName(display,window,"X-Elf");
  rcs_showWindow(display,window);
  rcs_moveWindow(display,window,mainX,mainY);
  screen=rcs_createPixmap(display,window,wx,wy);

  gc=rcs_openGC(display,screen);
  if (computer == '2') rcs_drawPixmap(display,screen,gc,0,0,elf2_xpm);
  else if (computer == 'S') rcs_drawPixmap(display,screen,gc,0,0,superelf_xpm);
  else if (computer == 'T') rcs_drawPixmap(display,screen,gc,0,0,studio2_xpm);
  else if (computer == 'X') rcs_drawPixmap(display,screen,gc,0,0,comx35_xpm);
  else rcs_drawPixmap(display,screen,gc,0,0,elf_xpm);
  til311 = til311Init(display,window);
  if (eprom == 'Y' && computer == '1')
    rcs_drawPixmap(display,screen,gc,13,15,eprom_xpm);

  rcs_copyArea(display,screen,window,gc,0,0,wx,wy,0,0);

  red=rcs_openGC(display,window);
  black=rcs_openGC(display,window);
  white=rcs_openGC(display,window);
  rcs_namedForeground(display,black,"black");
  rcs_namedForeground(display,red,"red");
  rcs_namedForeground(display,white,"white");
  init();
  debugWindow=rcs_createWindow(display,rcs_rootWindow(display),10,10,640,480);
  rcs_setWindowName(display,debugWindow,"Elf Debugger");
  debugBuffer=rcs_createPixmap(display,debugWindow,640,480);
  rcs_drawPixmap(display,debugBuffer,gc,0,0,debug_xpm);
  if (debugMode == 'Y') {
    rcs_showWindow(display,debugWindow);
    rcs_copyArea(display,debugBuffer,debugWindow,gc,0,0,353,480,0,0);
    debugOpen = 'Y';
    }

  clockWindow=rcs_createWindow(display,rcs_rootWindow(display),10,10,260,70);
  rcs_setWindowName(display,clockWindow,"Last Run");
  clockBuffer=rcs_createPixmap(display,clockWindow,200,100);
  clockOpen = 'N';
  if (use1870 == 'Y' && cidelsa != 'Y') {
    v1870Window=rcs_createWindow(display,rcs_rootWindow(display),10,10,240*vScale,224*vScale);
    rcs_setWindowName(display,v1870Window,"Elf 1870");
    rcs_showWindow(display,v1870Window);
    v1870Buffer=rcs_createPixmap(display,v1870Window,240*vScale,224*vScale);
    rcs_namedForeground(display,gc,"black");
    rcs_drawFilledBox(display,v1870Buffer,gc,0,0,240*vScale,224*vScale);
    }
  if (use1870 == 'Y' && cidelsa == 'Y') {
    v1870Window=rcs_createWindow(display,rcs_rootWindow(display),10,10,224*vScale,240*vScale);
    rcs_setWindowName(display,v1870Window,"Elf 1870");
    rcs_showWindow(display,v1870Window);
    v1870Buffer=rcs_createPixmap(display,v1870Window,224*vScale,240*vScale);
    rcs_namedForeground(display,gc,"black");
    rcs_drawFilledBox(display,v1870Buffer,gc,0,0,224*vScale,240*vScale);
    }

  if (use6845 == 'Y') {
    v6845Window=rcs_createWindow(display,rcs_rootWindow(display),10,10,512,256);
    rcs_setWindowName(display,v6845Window,"Elf 6847");
    rcs_showWindow(display,v6845Window);
    v6845Buffer=rcs_createPixmap(display,v6845Window,512,256);
    rcs_namedForeground(display,gc,"black");
    rcs_drawFilledBox(display,v6845Buffer,gc,0,0,512,256);
    }

  if (keypad=='Y') {
    hexWindow=rcs_createWindow(display,rcs_rootWindow(display),10,10,178,229);
    rcs_setWindowName(display,hexWindow,"Elf Keypad");
    rcs_showWindow(display,hexWindow);
    rcs_moveWindow(display,hexWindow,hexX,hexY);
    hexBuffer=rcs_createPixmap(display,hexWindow,178,229);
    rcs_drawPixmap(display,hexBuffer,gc,0,0,hexpad_xpm);
    rcs_copyArea(display,hexBuffer,hexWindow,gc,0,0,353,480,0,0);
    nextNybble='H';
    keypadValue=0;
    keypadHi=redLedNew();
    redLedSetPosition(&keypadHi,20,174);
    redLedSetState(&keypadHi,1);
    redLedPaint(&keypadHi,display,hexWindow);
    keypadLo=redLedNew();
    redLedSetPosition(&keypadLo,116,174);
    redLedSetState(&keypadLo,0);
    redLedPaint(&keypadLo,display,hexWindow);
    }
  if (ledModule=='Y') {
    ledWindow=rcs_createWindow(display,rcs_rootWindow(display),10,10,156,116);
    rcs_setWindowName(display,ledWindow,"Elf LED Module");
    rcs_showWindow(display,ledWindow);
    ledBuffer=rcs_createPixmap(display,ledWindow,156,116);
    rcs_drawPixmap(display,ledBuffer,gc,0,0,led_xpm);
    rcs_copyArea(display,ledBuffer,ledWindow,gc,0,0,156,116,0,0);
    for (i=0;i<8;i++) {
      led[i]=redLedNew();
      redLedSetPosition(&led[i],20+16*(7-i),16);
      redLedSetState(&led[i],0);
      redLedPaint(&led[i],display,ledWindow);
      }
    }
  if (hexOut=='Y') {
    hexOutWindow=rcs_createWindow(display,rcs_rootWindow(display),10,10,178,102);
    rcs_setWindowName(display,hexOutWindow,"Elf Hex Module");
    rcs_showWindow(display,hexOutWindow);
    hexOutBuffer=rcs_createPixmap(display,hexOutWindow,178,102);
    rcs_drawPixmap(display,hexOutBuffer,gc,0,0,hex_xpm);
    rcs_copyArea(display,hexOutBuffer,hexOutWindow,gc,0,0,178,102,0,0);
    for (i=0;i<4;i++) {
      hexDisp[i]=til311New();
      til311SetPosition(&hexDisp[i],10+40*(3-i),12);
      til311Paint(&hexDisp[i],display,hexOutWindow);
      }
    }
  if (graphics =='Y') {
    pixieWindow=rcs_createWindow(display,rcs_rootWindow(display),10,10,256*vScale,256*vScale);
    rcs_setWindowName(display,pixieWindow,"Elf Pixie Graphics");
    rcs_showWindow(display,pixieWindow);
    rcs_moveWindow(display,pixieWindow,pixieX,pixieY);
    pixieBuffer=rcs_createPixmap(display,pixieWindow,256*vScale,256*vScale);
    rcs_drawFilledBox(display,pixieBuffer,black,0,0,256*vScale,256*vScale);
    rcs_copyArea(display,pixieBuffer,pixieWindow,gc,0,0,256*vScale,256*vScale,0,0);
    }
  if (useTMS9918 =='Y') {
    vdp = tms9918_init();
    }
  if (use1870 =='Y') {
    v1870 = v1870_init();
    }
  if (useVt100 == 'Y') vt100Init();
  resetDisplay();
  cpu.cycles = 0;
  startTime = time(NULL);
  runMachine();
  if (useTMS9918 =='Y') {
    tms9918_destroy(&vdp);
    }
  if (keypad=='Y') {
    rcs_closePixmap(display,hexBuffer);
    rcs_closeWindow(display,hexWindow);
    }
  if (ledModule=='Y') {
    rcs_closePixmap(display,ledBuffer);
    rcs_closeWindow(display,ledWindow);
    }
  if (graphics=='Y') {
    rcs_closePixmap(display,pixieBuffer);
    rcs_closeWindow(display,pixieWindow);
    }
  if (use1870=='Y') {
    rcs_closePixmap(display,v1870Buffer);
    rcs_closeWindow(display,v1870Window);
    }
  if (use6845=='Y') {
    rcs_closePixmap(display,v6845Buffer);
    rcs_closeWindow(display,v6845Window);
    }
  rcs_closeGC(display,red);
  rcs_closeGC(display,black);
  rcs_closeGC(display,white);

  rcs_closePixmap(display,screen);
  rcs_closeWindow(display,window);
  rcs_closeDisplay(display);
  free(ram);
  if (useEMS == 'Y') free(ems);
  show_time();
  return 0;
  }

#ifdef WIN32
int APIENTRY WinMain(HINSTANCE hInstance,
                                         HINSTANCE hPrevInstance,
                                         LPSTR     lpCmdLine,
                                         int       nCmdShow) {
        int argc;
		char **argv;
		char buffer[1024];
		char *pBuffer;
        rcs_initWin32(hInstance);
		strcpy(buffer,"elf");
		argv=(char**)malloc(sizeof(char*));
		argv[0] = (char*)malloc(strlen(buffer) + 1);
		strcpy(argv[0],buffer);
		argc = 1;
		pBuffer = buffer;
		while (*lpCmdLine != 0) {
			if (*lpCmdLine <= ' ') {
				if (pBuffer != buffer) {
					argc += 1;
					argv=(char**)realloc(argv,sizeof(char*) * argc);
					argv[argc-1] = (char*)malloc(strlen(buffer) + 1);
					strcpy(argv[argc-1],buffer);
					pBuffer = buffer;
				}
				lpCmdLine++;
			} else {
				*pBuffer++ = *lpCmdLine++;
				*pBuffer = 0;
			}
		}
		if (pBuffer != buffer) {
			argc += 1;
			argv=(char**)realloc(argv,sizeof(char*) * argc);
			argv[argc-1] = (char*)malloc(strlen(buffer) + 1);
			strcpy(argv[argc-1],buffer);
			pBuffer = buffer;
		}
	
        main(argc,argv);
        return 0;
}
#endif

