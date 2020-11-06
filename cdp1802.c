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
#include <string.h>
#include <stdlib.h>
#include "elf.h"

CDP1802 cdp1802New() {
  CDP1802 ret;
  ret.readMem=NULL;
  ret.readPort=NULL;
  ret.writeMem=NULL;
  ret.writePort=NULL;
  ret.debug=NULL;
  ret.mode='I';
  ret.clear=0;
  ret.wait=1;
  ret.d=0;
  ret.df=0;
  ret.t=0;
  cdp1802Reset(&ret);
  return ret;
  }

void cdp1802SetMode(CDP1802* cpu) {
  if (cpu->clear == 0 && cpu->wait==1) { cpu->mode='I'; cdp1802Reset(cpu); }
  if (cpu->clear == 1 && cpu->wait==1) cpu->mode='R';
  if (cpu->clear == 0 && cpu->wait==0) cpu->mode='L';
  if (cpu->clear == 1 && cpu->wait==0) cpu->mode='P';
  if (cpu->mode != 'R' && picoelf != 0) memSwitch = 0x8000;
  }

void cdp1802SetClear(CDP1802* cpu,int value) {
  cpu->clear=(value)?1:0;
  cdp1802SetMode(cpu);
  }

void cdp1802SetWait(CDP1802* cpu,int value) {
  cpu->wait=(value)?1:0;
  cdp1802SetMode(cpu);
  }

void cdp1802DmaIn(CDP1802* cpu,byte value) {
  if (trace == 'Y') {
    debugDisplay("DMA In");
      debugTrace("DMA In");
    }
  if (cpu->mode != 'R' && cpu->mode !='L') return;
  if (cpu->writeMem != NULL) cpu->writeMem(cpu->r[0],value);
  cpu->address = cpu->r[0]++;
  cpu->idle=0;
  }

void decCounter(CDP1802* cpu) {
  if (--cpu->counter == 0) {
    if (cpu->tq) {
      cpu->q = (cpu->q) ? 0 : 1;
      }
    cpu->ci = (cpu->cie) ? 1 : 0;
    cpu->counter = cpu->ch;
    }
  }
void cdp1802SetEF(CDP1802* cpu,int flag,int value) {
/*
  if (cpu->debug != NULL) {
    sprintf(buffer," EF%d=%d \n",flag,value);
    cpu->debug(buffer);
    }
*/
  if (flag == 1) {
    if (cpu->ctrMode == 2 && cpu->ctrRunning) {
      if (value == 0 && (cpu->ef & 1)) decCounter(cpu);
      }
    if (value) cpu->ef |= 1; else cpu->ef &= 0xe;
    }
  if (flag == 2) {
    if (cpu->ctrMode == 3 && cpu->ctrRunning) {
      if (value == 0 && (cpu->ef & 2)) decCounter(cpu);
      }
    if (value) cpu->ef |= 2; else cpu->ef &= 0xd;
    }
  if (flag == 3) {
    if (value) cpu->ef |= 4; else cpu->ef &= 0xb;
    }
  if (flag == 4) {
    if (value) cpu->ef |= 8; else cpu->ef &= 7;
    }
  }

byte cdp1802DmaOut(CDP1802* cpu) {
  byte ret;
  ret = 255;
  if (trace == 'Y') {
    debugDisplay("DMA Out");
      debugTrace("DMA Out");
    }
  if (cpu->readMem != NULL) ret=cpu->readMem(cpu->r[0]);
  cpu->r[0]++;
  cpu->idle=0;
  cpu->cycles++;
  cpu->cycleCount++;
  return ret;
  }

void cdp1802Int(CDP1802* cpu) {
  if (cpu->ie) {
    if (trace == 'Y') {
      debugDisplay("Interrupt");
      debugTrace("Interrupt");
      }
    cpu->t=(cpu->x<<4) | cpu->p;
    cpu->x=2;
    cpu->p=1;
    cpu->ie=0;
    cpu->cycles++;
    cpu->cycleCount++;
    }
  cpu->idle=0;
  }

void cdp1802Reset(CDP1802* cpu) {
  cpu->i=0;
  cpu->n=0;
  cpu->q=0 ^ revQ;
  cpu->ie=1;
  cpu->ci=0;
  cpu->xi=0;
  cpu->x=0;
  cpu->p=0;
  cpu->r[0]=0;
  cpu->ctrPre = 0;
  cpu->tq = 0;
  cpu->ctrRunning = 0;
  if (picoelf) memSwitch = 0x8000;
/*
  for (i=1; i<16; i++) cpu->r[i] = 0;
*/
  cpu->idle=0;
  cpu->address=0;
  }

void inst1805(CDP1802* cpu) {
  char buffer[80];
  byte i,h,l;
  int  a,b;
  int w;
  w = 0;
  i=cpu->readMem(cpu->r[cpu->p]);
  cpu->n = i & 15;
  cpu->i = i>>4;
  cpu->r[cpu->p]++;
  switch (cpu->i) {
    case 0: switch (cpu->n) {
              case 0:cpu->ctrRunning = 0;
                     cpu->ctrPre = 0;
                     cpu->ctrMode = 0;
                      if (cpu->debug != NULL) {
                        sprintf(buffer," STPC");
                        cpu->debug(buffer);
                        }
                      break;
              case 1:decCounter(cpu);
                      if (cpu->debug != NULL) {
                        sprintf(buffer," DTC");
                        cpu->debug(buffer);
                        }
                      break;
              case 2:cpu->ctrMode = 5;
                     cpu->ctrRunning=1;
                      if (cpu->debug != NULL) {
                        sprintf(buffer," SPM2");
                        cpu->debug(buffer);
                        }
                      break;
              case 3:cpu->ctrMode = 3;
                     cpu->ctrRunning=1;
                      if (cpu->debug != NULL) {
                        sprintf(buffer," SCM2");
                        cpu->debug(buffer);
                        }
                      break;
              case 4:cpu->ctrMode = 4;
                     cpu->ctrRunning=1;
                      if (cpu->debug != NULL) {
                        sprintf(buffer," SPM1");
                        cpu->debug(buffer);
                        }
                      break;
              case 5:cpu->ctrMode = 2;
                     cpu->ctrRunning=1;
                      if (cpu->debug != NULL) {
                        sprintf(buffer," SCM1");
                        cpu->debug(buffer);
                        }
                      break;
              case 6:cpu->ch = cpu->d;
                     if (!cpu->ctrRunning) {
                       cpu->counter = cpu->d;
                       cpu->ci = 0;
                       }
                      if (cpu->debug != NULL) {
                        sprintf(buffer," LDC");
                        cpu->debug(buffer);
                        }
                      break;
              case 7:cpu->ctrMode = 1;
                     cpu->ctrRunning = 1;
                      if (cpu->debug != NULL) {
                        sprintf(buffer," STM");
                        cpu->debug(buffer);
                        }
                      break;
              case 8:cpu->d = cpu->counter;
                      if (cpu->debug != NULL) {
                        sprintf(buffer," GEC");
                        cpu->debug(buffer);
                        }
                      break;
              case 9:cpu->tq = 1;
                      if (cpu->debug != NULL) {
                        sprintf(buffer," ETQ");
                        cpu->debug(buffer);
                        }
                      break;
              case 10:cpu->xie = 1;
                      if (cpu->debug != NULL) {
                        sprintf(buffer," XIE");
                        cpu->debug(buffer);
                        }
                      break;
              case 11:cpu->xie = 0;
                      if (cpu->debug != NULL) {
                        sprintf(buffer," XID");
                        cpu->debug(buffer);
                        }
                      break;
              case 12:cpu->cie = 1;
                      if (cpu->debug != NULL) {
                        sprintf(buffer," CIE");
                        cpu->debug(buffer);
                        }
                      break;
              case 13:cpu->cie = 0;
                      if (cpu->debug != NULL) {
                        sprintf(buffer," CID");
                        cpu->debug(buffer);
                        }
                      break;
              }
            break;
    case 2:h=cpu->readMem(cpu->r[cpu->p]);
           l=cpu->readMem((word)(cpu->r[cpu->p]+1));
           cpu->cycles += 1;
           w=(h<<8)|l;
           if (--cpu->r[cpu->n] != 0) cpu->r[cpu->p] = w;
             else cpu->r[cpu->p]+=2;
           if (cpu->debug != NULL) {
             sprintf(buffer," DBNZ %02x%02x ",h,l);
             cpu->debug(buffer);
             }
           break;
    case 3: switch (cpu->n) {
              case 14:if (cpu->ci) {
                       i=cpu->readMem(cpu->r[cpu->p]);
                       cpu->r[cpu->p]=(cpu->r[cpu->p]&0xff00) | i;
                       } else cpu->r[cpu->p]++;
                     if (cpu->debug != NULL) {
                       sprintf(buffer," BCI %02x ",i);
                       cpu->debug(buffer);
                       }
                     break;
              case 15:if (cpu->xi) {
                       i=cpu->readMem(cpu->r[cpu->p]);
                       cpu->r[cpu->p]=(cpu->r[cpu->p]&0xff00) | i;
                       } else cpu->r[cpu->p]++;
                     if (cpu->debug != NULL) {
                       sprintf(buffer," BXI %02x ",i);
                       cpu->debug(buffer);
                       }
                     break;
              }
            break;
    case 6: w = cpu->readMem(cpu->r[cpu->x]++) << 8;
             w |= cpu->readMem(cpu->r[cpu->x]++);
             cpu->r[cpu->n] = w;
            if (cpu->debug != NULL) {
              sprintf(buffer," RLXA %x ",cpu->n);
              cpu->debug(buffer);
              }
             break;
    case 7: switch (cpu->n) {
              case 4:w = cpu->readMem(cpu->r[cpu->x]);
                     a = (((w >> 4) > 9) ? 9 : w >> 4) *10;
                     a |= ((w & 0xf) > 9) ? 9 : (w & 0xf);
                     w = cpu->d;
                     b = (((w >> 4) > 9) ? 9 : w >> 4) *10;
                     b |= ((w & 0xf) > 9) ? 9 : (w & 0xf);
                     a += b + cpu->df;
                     cpu->df = (a > 99) ? 1 : 0;
                     cpu->d = ((a / 10) << 4) | a % 10;
                      if (cpu->debug != NULL) {
                        sprintf(buffer," DADC ");
                        cpu->debug(buffer);
                        }
                      break;
              case 6:cpu->writeMem(--cpu->r[cpu->x],cpu->t);
                     cpu->writeMem(--cpu->r[cpu->x],cpu->d);
                     cpu->writeMem(--cpu->r[cpu->x],(byte)((cpu->d << 1) | cpu->df));
                     if (cpu->debug != NULL) {
                       sprintf(buffer," DSAV %x ",cpu->n);
                       cpu->debug(buffer);
                       }
                      break;
              case 7:w = cpu->readMem(cpu->r[cpu->x]);
                     a = (((w >> 4) > 9) ? 9 : w >> 4) *10;
                     a |= ((w & 0xf) > 9) ? 9 : (w & 0xf);
                     w = cpu->d;
                     b = (((w >> 4) > 9) ? 9 : w >> 4) *10;
                     b |= ((w & 0xf) > 9) ? 9 : (w & 0xf);
                     b -= (a + (cpu->df)?0:1);
                     a = b;
                     cpu->df = (a < 0) ? 0 : 1;
                     a = abs(a);
                     cpu->d = ((a / 10) << 4) | a % 10;
                      if (cpu->debug != NULL) {
                        sprintf(buffer," DADD ");
                        cpu->debug(buffer);
                        }
                      break;
              case 12:w = cpu->readMem(cpu->r[cpu->p]++);
                     a = (((w >> 4) > 9) ? 9 : w >> 4) *10;
                     a |= ((w & 0xf) > 9) ? 9 : (w & 0xf);
                     w = cpu->d;
                     b = (((w >> 4) > 9) ? 9 : w >> 4) *10;
                     b |= ((w & 0xf) > 9) ? 9 : (w & 0xf);
                     a += b + cpu->df;
                     cpu->df = (a > 99) ? 1 : 0;
                     cpu->d = ((a / 10) << 4) | a % 10;
                      if (cpu->debug != NULL) {
                        sprintf(buffer," DACI ");
                        cpu->debug(buffer);
                        }
                      break;
              case 15:w = cpu->readMem(cpu->r[cpu->p]++);
                     a = (((w >> 4) > 9) ? 9 : w >> 4) *10;
                     a |= ((w & 0xf) > 9) ? 9 : (w & 0xf);
                     w = cpu->d;
                     b = (((w >> 4) > 9) ? 9 : w >> 4) *10;
                     b |= ((w & 0xf) > 9) ? 9 : (w & 0xf);
                     b -= (a + (cpu->df)?0:1);
                     a = b;
                     cpu->df = (a < 0) ? 0 : 1;
                     a = abs(a);
                     cpu->d = ((a / 10) << 4) | a % 10;
                      if (cpu->debug != NULL) {
                        sprintf(buffer," DADD ");
                        cpu->debug(buffer);
                        }
                      break;
              }
            break;
    case 8: cpu->writeMem(cpu->r[cpu->x]--,(byte)(cpu->r[cpu->n] & 0xff));
            cpu->writeMem(cpu->r[cpu->x]--,(byte)(cpu->r[cpu->n] >> 8));
            cpu->r[cpu->n] = cpu->r[cpu->p];
            w = cpu->readMem(cpu->r[cpu->n]++) << 8;
            w |= cpu->readMem(cpu->r[cpu->n]++);
            cpu->r[cpu->p] = w;
            if (cpu->debug != NULL) {
              sprintf(buffer," SCAL %x ",cpu->n);
              cpu->debug(buffer);
              }
             break;
    case 9: cpu->r[cpu->p] = cpu->r[cpu->n];
            w = cpu->readMem(++cpu->r[cpu->x]) << 8;
            w |= cpu->readMem(++cpu->r[cpu->x]);
            if (cpu->debug != NULL) {
              sprintf(buffer," SRET %x ",cpu->n);
              cpu->debug(buffer);
              }
             break;
    case 10: cpu->writeMem(cpu->r[cpu->x]++,(byte)(cpu->r[cpu->n] >> 8));
             cpu->writeMem(cpu->r[cpu->x]++,(byte)(cpu->r[cpu->n] & 0xff));
             w |= cpu->readMem(cpu->r[cpu->x]++);
             cpu->r[cpu->n] = w;
            if (cpu->debug != NULL) {
              sprintf(buffer," RSXD %x ",cpu->n);
              cpu->debug(buffer);
              }
             break;
    case 11: cpu->r[cpu->x] = cpu->r[cpu->n];
            if (cpu->debug != NULL) {
              sprintf(buffer," RNX %x ",cpu->n);
              cpu->debug(buffer);
              }
             break;
    case 12: w = cpu->readMem(cpu->r[cpu->p]++) << 8;
             w |= cpu->readMem(cpu->r[cpu->p]++);
             cpu->r[cpu->n] = w;
            if (cpu->debug != NULL) {
              sprintf(buffer," RLDI %x ",cpu->n);
              cpu->debug(buffer);
              }
             break;
    case 15: switch (cpu->n) {
              case 4:w = cpu->readMem(cpu->r[cpu->x]);
                     a = (((w >> 4) > 9) ? 9 : w >> 4) *10;
                     a |= ((w & 0xf) > 9) ? 9 : (w & 0xf);
                     w = cpu->d;
                     b = (((w >> 4) > 9) ? 9 : w >> 4) *10;
                     b |= ((w & 0xf) > 9) ? 9 : (w & 0xf);
                     a += b;
                     cpu->df = (a > 99) ? 1 : 0;
                     cpu->d = ((a / 10) << 4) | a % 10;
                      if (cpu->debug != NULL) {
                        sprintf(buffer," DADD ");
                        cpu->debug(buffer);
                        }
                      break;
              case 7:w = cpu->readMem(cpu->r[cpu->x]);
                     a = (((w >> 4) > 9) ? 9 : w >> 4) *10;
                     a |= ((w & 0xf) > 9) ? 9 : (w & 0xf);
                     w = cpu->d;
                     b = (((w >> 4) > 9) ? 9 : w >> 4) *10;
                     b |= ((w & 0xf) > 9) ? 9 : (w & 0xf);
                     b -= a; a=b;
                     cpu->df = (a < 0) ? 0 : 1;
                     a = abs(a);
                     cpu->d = ((a / 10) << 4) | a % 10;
                      if (cpu->debug != NULL) {
                        sprintf(buffer," DADD ");
                        cpu->debug(buffer);
                        }
                      break;
              case 12:w = cpu->readMem(cpu->r[cpu->p]++);
                     a = (((w >> 4) > 9) ? 9 : w >> 4) *10;
                     a |= ((w & 0xf) > 9) ? 9 : (w & 0xf);
                     w = cpu->d;
                     b = (((w >> 4) > 9) ? 9 : w >> 4) *10;
                     b |= ((w & 0xf) > 9) ? 9 : (w & 0xf);
                     a += b;
                     cpu->df = (a > 99) ? 1 : 0;
                     cpu->d = ((a / 10) << 4) | a % 10;
                      if (cpu->debug != NULL) {
                        sprintf(buffer," DADI ");
                        cpu->debug(buffer);
                        }
                      break;
              case 15:w = cpu->readMem(cpu->r[cpu->p]++);
                     a = (((w >> 4) > 9) ? 9 : w >> 4) *10;
                     a |= ((w & 0xf) > 9) ? 9 : (w & 0xf);
                     w = cpu->d;
                     b = (((w >> 4) > 9) ? 9 : w >> 4) *10;
                     b |= ((w & 0xf) > 9) ? 9 : (w & 0xf);
                     b -= a; a=b;
                     cpu->df = (a < 0) ? 0 : 1;
                     a = abs(a);
                     cpu->d = ((a / 10) << 4) | a % 10;
                      if (cpu->debug != NULL) {
                        sprintf(buffer," DADD ");
                        cpu->debug(buffer);
                        }
                      break;
              }
    }
  }

void cdp1802Cycle(CDP1802* cpu) {
  char tr[256];
  byte i,h,l;
  int w;
  byte df1;
  byte tmp;
  char buffer[80];
  if (cpu->mode != 'R') return;
  if (cpu->readMem == NULL) return;
  if (cpu->idle) return;
  if (trace == 'Y') {
    sprintf(tr,"%04X: ",cpu->r[cpu->p]);
    }
  i=cpu->readMem(cpu->r[cpu->p]);
  cpu->n = i & 15;
  cpu->i = i>>4;
  cpu->r[cpu->p]++;
  switch (cpu->i) {
    case 0: if (cpu->n == 0) { cpu->idle=1;
              if (trace == 'Y') {
                strcat(tr," IDL");
                }
              }
              else {
              cpu->d=cpu->readMem(cpu->r[cpu->n]);
              if (trace == 'Y') {
                sprintf(buffer," LDN %X       D=%02X",cpu->n,cpu->d);
                strcat(tr,buffer);
                }
              }
            break;
    case 1: cpu->r[cpu->n]++;
            if (trace == 'Y') {
              sprintf(buffer," INC %X       R[%X]=%04X",cpu->n,cpu->n,cpu->r[cpu->n]);
              strcat(tr,buffer);
              }
            break;
    case 2: cpu->r[cpu->n]--;
            if (trace == 'Y') {
              sprintf(buffer," DEC %X       R[%X]=%04X",cpu->n,cpu->n,cpu->r[cpu->n]);
              strcat(tr,buffer);
              }
            break;
    case 3: switch (cpu->n) {
              case 0:i=cpu->readMem(cpu->r[cpu->p]);
                     cpu->r[cpu->p]=(cpu->r[cpu->p]&0xff00) | i;
                     if (trace == 'Y') {
                       sprintf(buffer," BR %02X ",i);
                       strcat(tr,buffer);
                       }
                     break;
              case 1:l=cpu->readMem(cpu->r[cpu->p]);
                     if (cpu->q) {
                       i=cpu->readMem(cpu->r[cpu->p]);
                       cpu->r[cpu->p]=(cpu->r[cpu->p]&0xff00) | i;
                       } else cpu->r[cpu->p]++;
                     if (trace == 'Y') {
                       sprintf(buffer," BQ %02X ",l);
                       strcat(tr,buffer);
                       }
                     break;
              case 2:l=cpu->readMem(cpu->r[cpu->p]);
                     if (!cpu->d) {
                       i=cpu->readMem(cpu->r[cpu->p]);
                       cpu->r[cpu->p]=(cpu->r[cpu->p]&0xff00) | i;
                       } else cpu->r[cpu->p]++;
                     if (trace == 'Y') {
                       sprintf(buffer," BZ %02X ",l);
                       strcat(tr,buffer);
                       }
                     break;
              case 3:l=cpu->readMem(cpu->r[cpu->p]);
                     if (cpu->df) {
                       i=cpu->readMem(cpu->r[cpu->p]);
                       cpu->r[cpu->p]=(cpu->r[cpu->p]&0xff00) | i;
                       } else cpu->r[cpu->p]++;
                     if (trace == 'Y') {
                       sprintf(buffer," BDF %02X ",l);
                       strcat(tr,buffer);
                       }
                     break;
              case 4:l=cpu->readMem(cpu->r[cpu->p]);
                     if (!(cpu->ef & 1)) {
                       i=cpu->readMem(cpu->r[cpu->p]);
                       cpu->r[cpu->p]=(cpu->r[cpu->p]&0xff00) | i;
                       } else cpu->r[cpu->p]++;
                     if (trace == 'Y') {
                       sprintf(buffer," B1 %02X ",l);
                       strcat(tr,buffer);
                       }
                     break;
              case 5:l=cpu->readMem(cpu->r[cpu->p]);
                     if (!(cpu->ef & 2)) {
                       i=cpu->readMem(cpu->r[cpu->p]);
                       cpu->r[cpu->p]=(cpu->r[cpu->p]&0xff00) | i;
                       } else cpu->r[cpu->p]++;
                     if (trace == 'Y') {
                       sprintf(buffer," B2 %02X ",l);
                       strcat(tr,buffer);
                       }
                     break;
              case 6:l=cpu->readMem(cpu->r[cpu->p]);
                     if (!(cpu->ef & 4)) {
                       i=cpu->readMem(cpu->r[cpu->p]);
                       cpu->r[cpu->p]=(cpu->r[cpu->p]&0xff00) | i;
                       } else cpu->r[cpu->p]++;
                     if (trace == 'Y') {
                       sprintf(buffer," B3 %02X ",l);
                       strcat(tr,buffer);
                       }
                     break;
              case 7:l=cpu->readMem(cpu->r[cpu->p]);
                     if (!(cpu->ef & 8)) {
                       i=cpu->readMem(cpu->r[cpu->p]);
                       cpu->r[cpu->p]=(cpu->r[cpu->p]&0xff00) | i;
                       } else cpu->r[cpu->p]++;
                     if (trace == 'Y') {
                       sprintf(buffer," B4 %02X ",l);
                       strcat(tr,buffer);
                       }
                     break;
              case 8:cpu->r[cpu->p]++;
                     if (trace == 'Y') {
                       sprintf(buffer," NBR");
                       strcat(tr,buffer);
                       }
                     break;
              case 9:l=cpu->readMem(cpu->r[cpu->p]);
                     if (!cpu->q) {
                       i=cpu->readMem(cpu->r[cpu->p]);
                       cpu->r[cpu->p]=(cpu->r[cpu->p]&0xff00) | i;
                       } else cpu->r[cpu->p]++;
                     if (trace == 'Y') {
                       sprintf(buffer," BNQ %02X ",l);
                       strcat(tr,buffer);
                       }
                     break;
              case 10:l=cpu->readMem(cpu->r[cpu->p]);
                      if (cpu->d) {
                       i=cpu->readMem(cpu->r[cpu->p]);
                       cpu->r[cpu->p]=(cpu->r[cpu->p]&0xff00) | i;
                       } else cpu->r[cpu->p]++;
                     if (trace == 'Y') {
                       sprintf(buffer," BNZ %02X ",l);
                       strcat(tr,buffer);
                       }
                     break;
              case 11:l=cpu->readMem(cpu->r[cpu->p]);
                      if (!cpu->df) {
                       i=cpu->readMem(cpu->r[cpu->p]);
                       cpu->r[cpu->p]=(cpu->r[cpu->p]&0xff00) | i;
                       } else cpu->r[cpu->p]++;
                     if (trace == 'Y') {
                       sprintf(buffer," BNF %02X ",l);
                       strcat(tr,buffer);
                       }
                     break;
              case 12:l=cpu->readMem(cpu->r[cpu->p]);
                      if (cpu->ef & 1) {
                       i=cpu->readMem(cpu->r[cpu->p]);
                       cpu->r[cpu->p]=(cpu->r[cpu->p]&0xff00) | i;
                       } else cpu->r[cpu->p]++;
                     if (trace == 'Y') {
                       sprintf(buffer," BN1 %02X ",l);
                       strcat(tr,buffer);
                       }
                     break;
              case 13:l=cpu->readMem(cpu->r[cpu->p]);
                      if (cpu->ef & 2) {
                       i=cpu->readMem(cpu->r[cpu->p]);
                       cpu->r[cpu->p]=(cpu->r[cpu->p]&0xff00) | i;
                       } else cpu->r[cpu->p]++;
                     if (trace == 'Y') {
                       sprintf(buffer," BN2 %02X ",l);
                       strcat(tr,buffer);
                       }
                     break;
              case 14:l=cpu->readMem(cpu->r[cpu->p]);
                      if (cpu->ef & 4) {
                       i=cpu->readMem(cpu->r[cpu->p]);
                       cpu->r[cpu->p]=(cpu->r[cpu->p]&0xff00) | i;
                       } else cpu->r[cpu->p]++;
                     if (trace == 'Y') {
                       sprintf(buffer," BN3 %02X ",l);
                       strcat(tr,buffer);
                       }
                     break;
              case 15:l=cpu->readMem(cpu->r[cpu->p]);
                      if (cpu->ef & 8) {
                       i=cpu->readMem(cpu->r[cpu->p]);
                       cpu->r[cpu->p]=(cpu->r[cpu->p]&0xff00) | i;
                       } else cpu->r[cpu->p]++;
                     if (trace == 'Y') {
                       sprintf(buffer," BN4 %02X ",l);
                       strcat(tr,buffer);
                       }
                     break;
              }
            break;
    case 4: cpu->d=cpu->readMem(cpu->r[cpu->n]++);
            if (trace == 'Y') {
              sprintf(buffer," LDA %X       D=M(%X)=%X",cpu->n,cpu->r[cpu->n]-1,cpu->d);
              strcat(tr,buffer);
              }
            break;
    case 5: if (cpu->writeMem == NULL) return;
            cpu->writeMem(cpu->r[cpu->n],cpu->d);
            if (trace == 'Y') {
              sprintf(buffer," STR %X       M(%X)=%X",cpu->n,cpu->r[cpu->n],cpu->d);
              strcat(tr,buffer);
              }
            break;
    case 6: if (cpu->n == 0) {
              cpu->r[cpu->x]++;
              if (trace == 'Y') {
                sprintf(buffer," IRX         R%X=%X",cpu->x,cpu->r[cpu->x]);
                strcat(tr,buffer);
                }
              break;
              }
            if (cpu->n == 8 && cpu->cpuMode == '5') {
              inst1805(cpu);
              break;
              }
            if (cpu->n <= 7) {
              if (cpu->writePort == NULL) return;
              v1870.addr = cpu->r[cpu->x];
              tmp = cpu->readMem(cpu->r[cpu->x]++);
              cpu->writePort(cpu->n,tmp);
              if (trace == 'Y') {
                sprintf(buffer," OUT %X       [%02X]",cpu->n,tmp);
                strcat(tr,buffer);
                }
              break;
              }
            if (cpu->n == 8) return;
            if (cpu->readPort == NULL) return;
            if (cpu->writeMem == NULL) return;
            i=cpu->readPort((byte)(cpu->n-8));
            cpu->writeMem(cpu->r[cpu->x],i);
            cpu->d=i;
            if (trace == 'Y') {
              sprintf(buffer," INP %X ",cpu->n-8);
              strcat(tr,buffer);
              }
            break;
    case 7: switch (cpu->n) {
              case 0: i=cpu->readMem(cpu->r[cpu->x]++);
                      cpu->p=i & 15;
                      cpu->x=(i>>4);
                      cpu->ie=1;
                      if (trace == 'Y') {
                        sprintf(buffer," RET");
                        strcat(tr,buffer);
                        }
                      break;
              case 1: i=cpu->readMem(cpu->r[cpu->x]++);
                      cpu->p=i & 15;
                      cpu->x=i>>4;
                      cpu->ie=0;
                      if (trace == 'Y') {
                        sprintf(buffer," DIS");
                        strcat(tr,buffer);
                        }
                      break;
              case 2: cpu->d = cpu->readMem(cpu->r[cpu->x]++);
                      if (trace == 'Y') {
                        sprintf(buffer," LDXA        m(%X)=%X",cpu->r[cpu->x]-1,cpu->d);
                        strcat(tr,buffer);
                        }
                      break;
              case 3: if (cpu->writeMem == NULL) return;
                      cpu->writeMem(cpu->r[cpu->x]--,cpu->d);
                      if (trace == 'Y') {
                        sprintf(buffer," STXD        m(%X)=%X",cpu->r[cpu->x]+1,cpu->d);
                        strcat(tr,buffer);
                        }
                      break;
              case 4: w=cpu->d + cpu->readMem(cpu->r[cpu->x]) + cpu->df;
                      if (w>255) {
                        cpu->d = w & 255;
                        cpu->df=1;
                        } else {
                        cpu->d=w;
                        cpu->df=0;
                        }
                      if (trace == 'Y') {
                        sprintf(buffer," ADC ");
                        strcat(tr,buffer);
                        }
                      break;
              case 5: w=cpu->readMem(cpu->r[cpu->x]) +((~cpu->d)&0xff) + cpu->df;
                      if (w>255) {
                        cpu->d = w&255;
                        cpu->df=1;
                        } else {
                        cpu->d=w&255;
                        cpu->df=0;
                        }
                      if (trace == 'Y') {
                        sprintf(buffer," SDB ");
                        strcat(tr,buffer);
                        }
                      break;
              case 6: df1=(cpu->df) ? 128 : 0;
                      cpu->df = (cpu->d & 1) ? 1 : 0;
                      cpu->d = (cpu->d >> 1) | df1;
                      if (trace == 'Y') {
                        sprintf(buffer," SHRC ");
                        strcat(tr,buffer);
                        }
                      break;
              case 7: w=cpu->d + ((~cpu->readMem(cpu->r[cpu->x]))&0xff) + cpu->df;
                      if (w>255) {
                        cpu->d = w & 0xff;
                        cpu->df=1;
                        } else {
                        cpu->d=w&255;
                        cpu->df=0;
                        }
                      if (trace == 'Y') {
                        sprintf(buffer," SMB ");
                        strcat(tr,buffer);
                        }
                      break;
              case 8: if (cpu->writeMem == NULL) return;
                      cpu->writeMem(cpu->r[cpu->x],cpu->t);
                      if (trace == 'Y') {
                        sprintf(buffer," SAV m(%X)",cpu->r[cpu->x]);
                        strcat(tr,buffer);
                        }
                      break;
              case 9: if (cpu->writeMem == NULL) return;
                      cpu->t=(cpu->x<<4) | cpu->p;
                      cpu->writeMem(cpu->r[2]--,cpu->t);
                      cpu->x=cpu->p;
                      if (trace == 'Y') {
                        sprintf(buffer," MARK ");
                        strcat(tr,buffer);
                        }
                      break;
              case 10: cpu->q=0 ^ revQ;
                      if (trace == 'Y') {
                        strcat(tr," REQ");
                        }
                      break;
              case 11: cpu->q=1 ^ revQ;
                      if (trace == 'Y') {
                        strcat(tr," SEQ");
                        }
                      break;
              case 12:i=cpu->readMem(cpu->r[cpu->p]++);
                      w=cpu->d + i + cpu->df;
                      if (w>255) {
                        cpu->d = w & 255;
                        cpu->df=1;
                        } else {
                        cpu->d=w;
                        cpu->df=0;
                        }
                      if (trace == 'Y') {
                        sprintf(buffer," ADC %02X ",i);
                        strcat(tr,buffer);
                        }
                      break;
              case 13:i=cpu->readMem(cpu->r[cpu->p]++);
                      w=i + ((~cpu->d)&0xff) + cpu->df;
                      if (w>255) {
                        cpu->d = w & 0xff;
                        cpu->df=1;
                        } else {
                        cpu->d=w&255;
                        cpu->df=0;
                        }
                      if (trace == 'Y') {
                        sprintf(buffer," SDBI %02X ",i);
                        strcat(tr,buffer);
                        }
                      break;
              case 14: df1=(cpu->df & 1) ? 1: 0;
                      cpu->df = (cpu->d & 128) ? 1 : 0;
                      cpu->d = (cpu->d << 1) | df1;
                      if (trace == 'Y') {
                        sprintf(buffer," SHLC ");
                        strcat(tr,buffer);
                        }
                      break;
              case 15:i=(~cpu->readMem(cpu->r[cpu->p]++)) & 0xff;
                      w=cpu->d + i + cpu->df;
                      if (w>255) {
                        cpu->d = w & 0xff;
                        cpu->df=1;
                        } else {
                        cpu->d=w&255;
                        cpu->df=0;
                        }
                      if (trace == 'Y') {
                        sprintf(buffer," SMBI %02X ",i);
                        strcat(tr,buffer);
                        }
                      break;
              }
            break;
    case 8: cpu->d=(cpu->r[cpu->n] & 255);
            if (trace == 'Y') {
              sprintf(buffer," GLO %X       D=%X",cpu->n,cpu->d);
              strcat(tr,buffer);
              }
            break;
    case 9: cpu->d=(cpu->r[cpu->n] >> 8);
            if (trace == 'Y') {
              sprintf(buffer," GHI %X       D=%X",cpu->n,cpu->d);
              strcat(tr,buffer);
              }
            break;
    case 10: cpu->r[cpu->n]=(cpu->r[cpu->n] & 0xff00) | cpu->d;
            if (trace == 'Y') {
              sprintf(buffer," PLO %X       R%X = %04X ",cpu->n,cpu->n,cpu->r[cpu->n]);
              strcat(tr,buffer);
              }
            break;
    case 11: cpu->r[cpu->n]=(cpu->r[cpu->n] & 0x00ff) | (cpu->d<<8);
            if (trace == 'Y') {
              sprintf(buffer," PHI %X       R%X = %04X ",cpu->n,cpu->n,cpu->r[cpu->n]);
              strcat(tr,buffer);
              }
            break;
    case 12:h=cpu->readMem(cpu->r[cpu->p]);
            l=cpu->readMem((word)(cpu->r[cpu->p]+1));
            cpu->cycles += 1;
            cpu->cycleCount += 1;
            w=(h<<8)|l;
            switch (cpu->n) {
              case 0:cpu->r[cpu->p]=w;
                     if (trace == 'Y') {
                       sprintf(buffer," LBR %02X%02X ",h,l);
                       strcat(tr,buffer);
                       }
                     break;
              case 1:if (cpu->q) cpu->r[cpu->p]=w;
                       else cpu->r[cpu->p]+=2;
                     if (trace == 'Y') {
                       sprintf(buffer," LBQ %02X%02X ",h,l);
                       strcat(tr,buffer);
                       }
                     break;
              case 2:if (!cpu->d) cpu->r[cpu->p]=w;
                       else cpu->r[cpu->p]+=2;
                     if (trace == 'Y') {
                       sprintf(buffer," LBZ %02X%02X ",h,l);
                       strcat(tr,buffer);
                       }
                     break;
              case 3:if (cpu->df) cpu->r[cpu->p]=w;
                       else cpu->r[cpu->p]+=2;
                     if (trace == 'Y') {
                       sprintf(buffer," LBDF %02X%02X ",h,l);
                       strcat(tr,buffer);
                       }
                     break;
              case 4:if (trace == 'Y') {
                       sprintf(buffer," NOP ");
                       strcat(tr,buffer);
                       }
                     break;
              case 5:if (!cpu->q) cpu->r[cpu->p]+=2;
                     if (trace == 'Y') {
                       sprintf(buffer," LSNQ ");
                       strcat(tr,buffer);
                       }
                     break;
              case 6:if (cpu->d) cpu->r[cpu->p]+=2;
                     if (trace == 'Y') {
                       sprintf(buffer," LSNZ ");
                       strcat(tr,buffer);
                       }
                     break;
              case 7:if (!cpu->df) cpu->r[cpu->p]+=2;
                     if (trace == 'Y') {
                       sprintf(buffer," LSNF ");
                       strcat(tr,buffer);
                       }
                     break;
              case 8:cpu->r[cpu->p]+=2;
                     if (trace == 'Y') {
                       sprintf(buffer," NLBR ");
                       strcat(tr,buffer);
                       }
                     break;
              case 9:if (!cpu->q) cpu->r[cpu->p]=w;
                       else cpu->r[cpu->p]+=2;
                     if (trace == 'Y') {
                       sprintf(buffer," LBNQ %02X%02X ",h,l);
                       strcat(tr,buffer);
                       }
                     break;
              case 10:if (cpu->d) cpu->r[cpu->p]=w;
                       else cpu->r[cpu->p]+=2;
                     if (trace == 'Y') {
                       sprintf(buffer," LBNZ %02X%02X ",h,l);
                       strcat(tr,buffer);
                       }
                     break;
              case 11:if (!cpu->df) cpu->r[cpu->p]=w;
                       else cpu->r[cpu->p]+=2;
                     if (trace == 'Y') {
                       sprintf(buffer," LBNF %02X%02X ",h,l);
                       strcat(tr,buffer);
                       }
                     break;
              case 12:if (cpu->ie) cpu->r[cpu->p]+=2;
                     if (trace == 'Y') {
                       sprintf(buffer," LSIE ");
                       strcat(tr,buffer);
                       }
                     break;
              case 13:if (cpu->q) cpu->r[cpu->p]+=2;
                     if (trace == 'Y') {
                       sprintf(buffer," LSQ ");
                       strcat(tr,buffer);
                       }
                     break;
              case 14:if (!cpu->d) cpu->r[cpu->p]+=2;
                     if (trace == 'Y') {
                       sprintf(buffer," LSZ ");
                       strcat(tr,buffer);
                       }
                     break;
              case 15:if (cpu->df) cpu->r[cpu->p]+=2;
                     if (trace == 'Y') {
                       sprintf(buffer," LSDF ");
                       strcat(tr,buffer);
                       }
                     break;
              }
            machineCycle(cpu,2);
            break;
    case 13: cpu->p=cpu->n;
            if (trace == 'Y') {
              sprintf(buffer," SEP %X ",cpu->n);
              strcat(tr,buffer);
              }
            break;
    case 14: cpu->x=cpu->n;
            if (trace == 'Y') {
              sprintf(buffer," SEX %X ",cpu->n);
              strcat(tr,buffer);
              }
            break;
    case 15:switch (cpu->n) {
              case 0:cpu->d=cpu->readMem(cpu->r[cpu->x]);
                     if (trace == 'Y') {
                       sprintf(buffer," LDX         D=m(%X)=%X",cpu->r[cpu->x],cpu->d);
                       strcat(tr,buffer);
                       }
                     break;
              case 1:cpu->d=cpu->readMem(cpu->r[cpu->x]) | cpu->d;
                     if (trace == 'Y') {
                       sprintf(buffer," OR         D=%02X",cpu->d);
                       strcat(tr,buffer);
                       }
                     break;
              case 2:cpu->d=cpu->readMem(cpu->r[cpu->x]) & cpu->d;
                     if (trace == 'Y') {
                       sprintf(buffer," AND         D=%02X",cpu->d);
                       strcat(tr,buffer);
                       }
                     break;
              case 3:cpu->d=cpu->readMem(cpu->r[cpu->x]) ^ cpu->d;
                     if (trace == 'Y') {
                       sprintf(buffer," XOR         D=%02X",cpu->d);
                       strcat(tr,buffer);
                       }
                     break;
              case 4: w=cpu->d + cpu->readMem(cpu->r[cpu->x]);
                      if (w>255) {
                        cpu->d = w & 255;
                        cpu->df=1;
                        } else {
                        cpu->d=w;
                        cpu->df=0;
                        }
                     if (trace == 'Y') {
                        sprintf(buffer," ADD         D=%02X",cpu->d);
                       strcat(tr,buffer);
                        }
                      break;
              case 5: w=cpu->readMem(cpu->r[cpu->x]) + ((~cpu->d)&0xff)+1;
                      if (w>255) {
                        cpu->d = w & 0xff;
                        cpu->df=1;
                        } else {
                        cpu->d=w&255;
                        cpu->df=0;
                        }
                     if (trace == 'Y') {
                        sprintf(buffer," SD ");
                       strcat(tr,buffer);
                        }
                      break;
              case 6: cpu->df=(cpu->d & 1)? 1 : 0;
                      cpu->d=cpu->d>>1;
                     if (trace == 'Y') {
                        sprintf(buffer," SHR         D=%02X",cpu->d);
                       strcat(tr,buffer);
                        }
                      break;
              case 7: w=cpu->d+((~cpu->readMem(cpu->r[cpu->x]))&0xff)+1;
                      if (w>255) {
                        cpu->d = w & 0xff;
                        cpu->df=1;
                        } else {
                        cpu->d=w&255;
                        cpu->df=0;
                        }
                     if (trace == 'Y') {
                        sprintf(buffer," SM ");
                       strcat(tr,buffer);
                        }
                      break;
              case 8: cpu->d=cpu->readMem(cpu->r[cpu->p]++);
                     if (trace == 'Y') {
                        sprintf(buffer," LDI %02X",cpu->d);
                       strcat(tr,buffer);
                        }
                      break;
              case 9: i=cpu->readMem(cpu->r[cpu->p]++);
                      cpu->d |= i;
                     if (trace == 'Y') {
                        sprintf(buffer," ORI %02X      D=%02X",i,cpu->d);
                       strcat(tr,buffer);
                        }
                      break;
              case 10: i=cpu->readMem(cpu->r[cpu->p]++);
                      cpu->d &= i;
                     if (trace == 'Y') {
                        sprintf(buffer," ANI %02X      D=%02X",i,cpu->d);
                       strcat(tr,buffer);
                        }
                      break;
              case 11: i=cpu->readMem(cpu->r[cpu->p]++);
                      cpu->d ^= i;
                     if (trace == 'Y') {
                        sprintf(buffer," XRI %02X      D=%02X",i,cpu->d);
                       strcat(tr,buffer);
                        }
                      break;
              case 12: i=cpu->readMem(cpu->r[cpu->p]++);
                       w=cpu->d + i;
                      if (w>255) {
                        cpu->d = w & 255;
                        cpu->df=1;
                        } else {
                        cpu->d=w;
                        cpu->df=0;
                        }
                     if (trace == 'Y') {
                        sprintf(buffer," ADI %02X      D=%02X",i,cpu->d);
                       strcat(tr,buffer);
                        }
                      break;
              case 13: i=cpu->readMem(cpu->r[cpu->p]++);
                       w=i+((~cpu->d)&0xff)+1;
                      if (w>255) {
                        cpu->d = w&0xff;
                        cpu->df=1;
                        } else {
                        cpu->d=w&255;
                        cpu->df=0;
                        }
                     if (trace == 'Y') {
                        sprintf(buffer," SDI %02X ",i);
                       strcat(tr,buffer);
                        }
                      break;
              case 14: cpu->df=(cpu->d & 128)? 1 : 0;
                      cpu->d=cpu->d<<1;
                     if (trace == 'Y') {
                        sprintf(buffer," SHL         D=%02X",cpu->d);
                       strcat(tr,buffer);
                        }
                      break;
              case 15: i=(~cpu->readMem(cpu->r[cpu->p]++))&0xff;
                       w=cpu->d+i+1;
                      if (w>255) {
                        cpu->d = w&0xff;
                        cpu->df=1;
                        } else {
                        cpu->d=w&255;
                        cpu->df=0;
                        }
                     if (trace == 'Y') {
                        sprintf(buffer," SMI %02X      D=%X",(~i)&0xff,cpu->d);
                       strcat(tr,buffer);
                        }
                      break;
              }
            break;
    }
  if (trace == 'Y') debugTrace(tr);
  }

