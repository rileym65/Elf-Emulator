/*
 *******************************************************************
 *** This software is copyright 2006 by Michael H Riley          ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "elf.h"

word lastAddr = 0;

char* nextWord(char* buffer) {
  while (*buffer != 0 && *buffer != ' ') buffer++;
  while (*buffer == 32) buffer++;
  return buffer;
  }

char* toHex(char* buffer,word* dest) {
  *dest = 0;
  while((*buffer>='0' && *buffer<='9') ||
        (*buffer>='A' && *buffer<='F')) {
    *dest <<= 4;
    *dest |= (*buffer<'A') ? *buffer-'0' : *buffer-'A'+10;
    buffer++;
    }
  return buffer;
  }

char* getAddress(char* buffer,word* ret) {
  buffer = nextWord(buffer);
  if (*buffer == 'P') {
    *ret = cpu.r[cpu.p];
    buffer++;
    }
  else if (*buffer == 'X') {
    *ret = cpu.r[cpu.x];
    buffer++;
    }
  else if (*buffer == 'R') {
    buffer++;
    switch (*buffer) {
      case '0':*ret = cpu.r[0]; break;
      case '1':*ret = cpu.r[1]; break;
      case '2':*ret = cpu.r[2]; break;
      case '3':*ret = cpu.r[3]; break;
      case '4':*ret = cpu.r[4]; break;
      case '5':*ret = cpu.r[5]; break;
      case '6':*ret = cpu.r[6]; break;
      case '7':*ret = cpu.r[7]; break;
      case '8':*ret = cpu.r[8]; break;
      case '9':*ret = cpu.r[9]; break;
      case 'A':*ret = cpu.r[10]; break;
      case 'B':*ret = cpu.r[11]; break;
      case 'C':*ret = cpu.r[12]; break;
      case 'D':*ret = cpu.r[13]; break;
      case 'E':*ret = cpu.r[14]; break;
      case 'F':*ret = cpu.r[15]; break;
      default:buffer--;
      }
    buffer++;
    }
  else if (*buffer != ',' && *buffer>' ') {
    buffer = toHex(buffer,ret);
    }
  return buffer;
  }

void resetDisplay() {
  int i;
  for (i=0; i<16; i++) lastR[i] = cpu.r[i]+1;
  for (i=0; i<8; i++) lastOUT[i] = outValues[i] + 1;
  for (i=0; i<8; i++) lastIN[i] = inValues[i] + 1;
  lastD = cpu.d + 1;
  lastP = cpu.p + 1;
  lastX = cpu.x + 1;
  lastT = cpu.t + 1;
  lastDF = cpu.df + 1;
  lastQ = cpu.q + 1;
  lastIE = cpu.ie + 1;
  lastEF1 = 99;
  lastEF2 = 99;
  lastEF3 = 99;
  lastEF4 = 99;
  }

void updateDebugger() {
  int i;
  rcs_GC gc;
  char buffer[16];
  gc=rcs_openGC(display,debugWindow);
  for (i=0; i<16; i++) {
    if (cpu.r[i] != lastR[i]) {
      sprintf(buffer,"%04X",cpu.r[i]);
      rcs_drawFilledBox(display,debugWindow,white,170+((i>7)?100:0),15+(i&7)*18,45,13);
#ifdef WIN32
      rcs_drawString(display,debugWindow,gc,175+((i>7)?100:0),30+(i&7)*18,buffer);
#else
      rcs_drawString(display,debugWindow,gc,175+((i>7)?100:0),26+(i&7)*18,buffer);
#endif
      lastR[i] = cpu.r[i];
      }
    }
  for (i=1; i<8; i++) {
    if (outValues[i] != lastOUT[i]) {
      sprintf(buffer,"%02X",outValues[i]);
      rcs_drawFilledBox(display,debugWindow,white,i*57-29,183,25,13);
#ifdef WIN32
      rcs_drawString(display,debugWindow,gc,i*57-27,198,buffer);
#else
      rcs_drawString(display,debugWindow,gc,i*57-27,194,buffer);
#endif
      lastOUT[i] = outValues[i];
      }
    if (inValues[i] != lastIN[i]) {
      sprintf(buffer,"%02X",inValues[i]);
      rcs_drawFilledBox(display,debugWindow,white,i*57-29,165,25,13);
#ifdef WIN32
      rcs_drawString(display,debugWindow,gc,i*57-27,180,buffer);
#else
      rcs_drawString(display,debugWindow,gc,i*57-27,176,buffer);
#endif
      lastIN[i] = inValues[i];
      }
    }
  if (cpu.d != lastD) {
    sprintf(buffer,"%02X",cpu.d);
    rcs_drawFilledBox(display,debugWindow,white,30,16,40,13);
#ifdef WIN32
    rcs_drawString(display,debugWindow,gc,35,31,buffer);
#else
    rcs_drawString(display,debugWindow,gc,35,27,buffer);
#endif
    lastD = cpu.d;
    }
  if (cpu.p != lastP) {
    sprintf(buffer,"%02X",cpu.p);
    rcs_drawFilledBox(display,debugWindow,white,30,34,21,13);
#ifdef WIN32
    rcs_drawString(display,debugWindow,gc,35,49,buffer);
#else
    rcs_drawString(display,debugWindow,gc,35,45,buffer);
#endif
    lastP = cpu.p;
    }
  if (cpu.x != lastX) {
    sprintf(buffer,"%02X",cpu.x);
    rcs_drawFilledBox(display,debugWindow,white,30,52,21,13);
#ifdef WIN32
    rcs_drawString(display,debugWindow,gc,35,67,buffer);
#else
    rcs_drawString(display,debugWindow,gc,35,63,buffer);
#endif
    lastX = cpu.x;
    }
  if (cpu.t != lastT) {
    sprintf(buffer,"%02X",cpu.t);
    rcs_drawFilledBox(display,debugWindow,white,30,70,21,13);
#ifdef WIN32
    rcs_drawString(display,debugWindow,gc,35,85,buffer);
#else
    rcs_drawString(display,debugWindow,gc,35,81,buffer);
#endif
    lastT = cpu.t;
    }
  if (cpu.df != lastDF) {
    rcs_drawFilledBox(display,debugWindow,(cpu.df)?black:white,118,16,11,11);
    lastDF = cpu.df;
    }
  if (cpu.q != lastQ) {
    rcs_drawFilledBox(display,debugWindow,(cpu.q)?black:white,118,34,11,11);
    lastQ = cpu.q;
    }
  if (cpu.ie != lastIE) {
    rcs_drawFilledBox(display,debugWindow,(cpu.ie)?black:white,118,52,11,11);
    lastIE = cpu.ie;
    }
  if ((cpu.ef & 1) != lastEF1) {
    rcs_drawFilledBox(display,debugWindow,(cpu.ef&1)?white:black,118,70,11,11);
    lastEF1 = cpu.ef & 8;
    }
  if ((cpu.ef & 2) != lastEF2) {
    rcs_drawFilledBox(display,debugWindow,(cpu.ef&2)?white:black,118,88,11,11);
    lastEF2 = cpu.ef & 2;
    }
  if ((cpu.ef & 4) != lastEF3) {
    rcs_drawFilledBox(display,debugWindow,(cpu.ef&4)?white:black,118,106,11,11);
    lastEF3 = cpu.ef & 4;
    }
  if ((cpu.ef & 8) != lastEF4) {
    rcs_drawFilledBox(display,debugWindow,(cpu.ef&8)?white:black,118,124,11,11);
    lastEF4 = cpu.ef & 8;
    }
/*
  for (i=0; i<14; i++) {
    rcs_drawString(display,debugWindow,black,10,i*16+222,
      "+123456789+123456789+123456789+123456789+123456789+123456789+123");
    }
*/
  rcs_closeGC(display,gc);
  }

void debugTrace(char* buffer) {
  int i;
  FILE *dlog;
  if (debugLog == 'Y') {
    dlog = fopen("debug.log","a+");
    fprintf(dlog,"[%X] %s\n",cpu.p,buffer);
    fclose(dlog);
    }
  for (i=0; i<16; i++) strcpy(debugLines[i],debugLines[i+1]);
  strcpy(debugLines[16],buffer);
/*
  rcs_drawFilledBox(display,debugWindow,black,409,166,221,273);
*/
  rcs_copyArea(display,debugWindow,debugWindow,gc,409,182,221,257,409,166);
  rcs_drawFilledBox(display,debugWindow,white,409,423,221,16);
  rcs_drawString(display,debugWindow,black,410,436,buffer);

/*
  rcs_copyArea(display,debugBuffer,debugWindow,gc,409,166,221,273,409,166);
*/
  }

void debugDisplay(char* buffer) {
/*
  rcs_drawFilledBox(display,debugWindow,black,9,209,391,230);
*/
#ifdef WIN32
  rcs_copyArea(display,debugWindow,debugWindow,gc,9,223,391,214,9,209);
  rcs_drawFilledBox(display,debugWindow,white,9,420,391,16);
  rcs_drawString(display,debugWindow,black,10,434,buffer);
#else
  rcs_copyArea(display,debugBuffer,debugBuffer,gc,9,223,391,214,9,209);
  rcs_drawFilledBox(display,debugBuffer,white,9,420,391,16);
  rcs_drawString(display,debugBuffer,black,10,430,buffer);
  rcs_copyArea(display,debugBuffer,debugWindow,gc,9,209,391,230,9,209);
#endif
  }

void debugDisplayBP() {
  int i;
  int x,y;
  char buffer[255];
  rcs_drawFilledBox(display,debugWindow,white,342,16,288,138);
  for (i=0; i<numBP; i++) {
    sprintf(buffer,"BP:%04X",breakPoints[i]);
    rcs_drawString(display,debugWindow,black,345+(i/9)*60,(i%9)*16+25,buffer);
    }
  for (i=0;i<numTraps; i++) {
    x = 345+((i+numBP)/9)*60;
    y = ((i+numBP)%9)*16+25;
    switch(traps[i][0]) {
      case 1:sprintf(buffer,"TR:%02X",traps[i][1]);
             break;
      case 2:sprintf(buffer,"TR:%02X%02X",traps[i][1],traps[i][2]);
             break;
      case 3:sprintf(buffer,"TR:%02X%02X%02X",traps[i][1],traps[i][2],traps[i][3]);
             break;
      }
    rcs_drawString(display,debugWindow,black,x,y,buffer);
    }
  for (i=0;i<numTregs; i++) {
    x = 345+((i+numBP+numTraps)/9)*60;
    y = ((i+numBP+numTraps)%9)*16+25;
    switch (tregs[i][0]) {
      case '0': case '1': case '2': case '3': case '4': case '5': case '6':
      case '7': case '8': case '9':
              sprintf(buffer,"R%X=%04X",tregs[i][0]-'0',tregs[i][1]); break;
      case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
              sprintf(buffer,"R%X=%04X",tregs[i][0]-87,tregs[i][1]); break;
      case 'D':sprintf(buffer," D=%02X",tregs[i][1]); break;
      case 'F':sprintf(buffer,"DF=%X",(tregs[i][1])?1:0); break;
      case 'Q':sprintf(buffer," Q=%X",(tregs[i][1])?1:0); break;
      case 'P':sprintf(buffer," P=%X",tregs[i][1]); break;
      case 'X':sprintf(buffer," P=%X",tregs[i][1]); break;
      case 'T':sprintf(buffer," D=%02X",tregs[i][1]); break;
      }
    rcs_drawString(display,debugWindow,black,x,y,buffer);
    }
  }

void uc(char* buffer) {
  int i;
  for (i=0; i<strlen(buffer); i++) {
    buffer[i]=(buffer[i]>='a' && buffer[i]<='z') ? buffer[i]-32 : buffer[i];
    if (buffer[i] > 0 && buffer[i] < ' ') buffer[i] = 0;
    }
  }

word getReg(char* buffer) {
  if (strcmp(buffer,"R0") == 0) return 0;
  if (strcmp(buffer,"R1") == 0) return 1;
  if (strcmp(buffer,"R2") == 0) return 2;
  if (strcmp(buffer,"R3") == 0) return 3;
  if (strcmp(buffer,"R4") == 0) return 4;
  if (strcmp(buffer,"R5") == 0) return 5;
  if (strcmp(buffer,"R6") == 0) return 6;
  if (strcmp(buffer,"R7") == 0) return 7;
  if (strcmp(buffer,"R8") == 0) return 8;
  if (strcmp(buffer,"R9") == 0) return 9;
  if (strcmp(buffer,"RA") == 0) return 10;
  if (strcmp(buffer,"RB") == 0) return 11;
  if (strcmp(buffer,"RC") == 0) return 12;
  if (strcmp(buffer,"RD") == 0) return 13;
  if (strcmp(buffer,"RE") == 0) return 14;
  if (strcmp(buffer,"RF") == 0) return 15;
  return 0;
  }

int assemble(char* buffer,byte* b1,byte* b2,byte* b3) {
  word v;
  if (strcmp(buffer,"ADC") == 0) { *b1 = 0x74; return 1; }
  if (strncmp(buffer,"ADCI",4) == 0) {
    buffer = nextWord(buffer); *b1 = 0x7c;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strcmp(buffer,"ADD") == 0) { *b1 = 0xf4; return 1; }
  if (strncmp(buffer,"ADI",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0xfc;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strcmp(buffer,"AND") == 0) { *b1 = 0xf2; return 1; }
  if (strncmp(buffer,"ANI",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0xfa;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strncmp(buffer,"B1",2) == 0) {
    buffer = nextWord(buffer); *b1 = 0x34;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strncmp(buffer,"B2",2) == 0) {
    buffer = nextWord(buffer); *b1 = 0x35;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strncmp(buffer,"B3",2) == 0) {
    buffer = nextWord(buffer); *b1 = 0x36;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strncmp(buffer,"B4",2) == 0) {
    buffer = nextWord(buffer); *b1 = 0x37;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strncmp(buffer,"BDF",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0x33;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strncmp(buffer,"BN1",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0x3c;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strncmp(buffer,"BN2",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0x3d;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strncmp(buffer,"BN3",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0x3e;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strncmp(buffer,"BN4",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0x3f;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strncmp(buffer,"BNF",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0x3b;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strncmp(buffer,"BNQ",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0x39;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strncmp(buffer,"BNZ",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0x3a;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strncmp(buffer,"BQ",2) == 0) {
    buffer = nextWord(buffer); *b1 = 0x31;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strncmp(buffer,"BR",2) == 0) {
    buffer = nextWord(buffer); *b1 = 0x30;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strncmp(buffer,"BZ",2) == 0) {
    buffer = nextWord(buffer); *b1 = 0x32;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strncmp(buffer,"DEC",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0x20 | getReg(buffer); return 1; }
  if (strcmp(buffer,"DIS") == 0) { *b1 = 0x71; return 1; }
  if (strncmp(buffer,"GHI",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0x90 | getReg(buffer); return 1; }
  if (strncmp(buffer,"GLO",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0x80 | getReg(buffer); return 1; }
  if (strcmp(buffer,"IDL") == 0) { *b1 = 0x00; return 1; }
  if (strncmp(buffer,"INC",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0x10 | getReg(buffer); return 1; }
  if (strncmp(buffer,"INP",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0x60;
    buffer = toHex(buffer,&v); *b1 |= (v & 0x7)+8; return 1; }
  if (strcmp(buffer,"IRX") == 0) { *b1 = 0x60; return 1; }
  if (strncmp(buffer,"LBDF",4) == 0) {
    buffer = nextWord(buffer); *b1 = 0xc3;
    buffer = toHex(buffer,&v); *b2 = v >> 8;
    *b3 = v & 0xff; return 3; }
  if (strncmp(buffer,"LBNF",4) == 0) {
    buffer = nextWord(buffer); *b1 = 0xcb;
    buffer = toHex(buffer,&v); *b2 = v >> 8;
    *b3 = v & 0xff; return 3; }
  if (strncmp(buffer,"LBNQ",4) == 0) {
    buffer = nextWord(buffer); *b1 = 0xc9;
    buffer = toHex(buffer,&v); *b2 = v >> 8;
    *b3 = v & 0xff; return 3; }
  if (strncmp(buffer,"LBNZ",4) == 0) {
    buffer = nextWord(buffer); *b1 = 0xca;
    buffer = toHex(buffer,&v); *b2 = v >> 8;
    *b3 = v & 0xff; return 3; }
  if (strncmp(buffer,"LBQ",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0xc1;
    buffer = toHex(buffer,&v); *b2 = v >> 8;
    *b3 = v & 0xff; return 3; }
  if (strncmp(buffer,"LBR",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0xc0;
    buffer = toHex(buffer,&v); *b2 = v >> 8;
    *b3 = v & 0xff; return 3; }
  if (strncmp(buffer,"LBZ",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0xc2;
    buffer = toHex(buffer,&v); *b2 = v >> 8;
    *b3 = v & 0xff; return 3; }
  if (strncmp(buffer,"LDA",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0x40 | getReg(buffer); return 1; }
  if (strncmp(buffer,"LDI",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0xf8;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strncmp(buffer,"LDN",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0x00 | getReg(buffer); return 1; }
  if (strcmp(buffer,"LDX") == 0) { *b1 = 0xf0; return 1; }
  if (strcmp(buffer,"LDXA") == 0) { *b1 = 0x72; return 1; }
  if (strcmp(buffer,"LSDF") == 0) { *b1 = 0xcf; return 1; }
  if (strcmp(buffer,"LSIE") == 0) { *b1 = 0xcc; return 1; }
  if (strcmp(buffer,"LSKP") == 0) { *b1 = 0xc8; return 1; }
  if (strcmp(buffer,"LSNF") == 0) { *b1 = 0xc7; return 1; }
  if (strcmp(buffer,"LSNQ") == 0) { *b1 = 0xc5; return 1; }
  if (strcmp(buffer,"LSNZ") == 0) { *b1 = 0xc6; return 1; }
  if (strcmp(buffer,"LSQ") == 0) { *b1 = 0xcd; return 1; }
  if (strcmp(buffer,"LSZ") == 0) { *b1 = 0xce; return 1; }
  if (strcmp(buffer,"MARK") == 0) { *b1 = 0x79; return 1; }
  if (strcmp(buffer,"NBR") == 0) { *b1 = 0x38; return 1; }
  if (strcmp(buffer,"NLBR") == 0) { *b1 = 0xc8; return 1; }
  if (strcmp(buffer,"NOP") == 0) { *b1 = 0xc4; return 1; }
  if (strcmp(buffer,"OR") == 0) { *b1 = 0xf1; return 1; }
  if (strncmp(buffer,"ORI",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0xf9;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strncmp(buffer,"OUT",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0x60;
    buffer = toHex(buffer,&v); *b1 |= (v & 0x7); return 1; }
  if (strncmp(buffer,"PHI",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0xb0 | getReg(buffer); return 1; }
  if (strncmp(buffer,"PLO",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0xa0 | getReg(buffer); return 1; }
  if (strcmp(buffer,"REQ") == 0) { *b1 = 0x7a; return 1; }
  if (strcmp(buffer,"RET") == 0) { *b1 = 0x70; return 1; }
  if (strcmp(buffer,"SAV") == 0) { *b1 = 0x78; return 1; }
  if (strcmp(buffer,"SD") == 0) { *b1 = 0xf5; return 1; }
  if (strcmp(buffer,"SDB") == 0) { *b1 = 0x75; return 1; }
  if (strncmp(buffer,"SDBI",4) == 0) {
    buffer = nextWord(buffer); *b1 = 0x7d;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strncmp(buffer,"SDI",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0xfd;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strncmp(buffer,"SEP",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0xd0 | getReg(buffer); return 1; }
  if (strcmp(buffer,"SEQ") == 0) { *b1 = 0x7b; return 1; }
  if (strncmp(buffer,"SEX",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0xe0 | getReg(buffer); return 1; }
  if (strcmp(buffer,"SHL") == 0) { *b1 = 0xfe; return 1; }
  if (strcmp(buffer,"SHLC") == 0) { *b1 = 0x7e; return 1; }
  if (strcmp(buffer,"SHR") == 0) { *b1 = 0xf6; return 1; }
  if (strcmp(buffer,"SHRC") == 0) { *b1 = 0x76; return 1; }
  if (strcmp(buffer,"SKP") == 0) { *b1 = 0x38; return 1; }
  if (strcmp(buffer,"SM") == 0) { *b1 = 0xf7; return 1; }
  if (strcmp(buffer,"SMB") == 0) { *b1 = 0x77; return 1; }
  if (strncmp(buffer,"SMBI",4) == 0) {
    buffer = nextWord(buffer); *b1 = 0x7f;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strncmp(buffer,"SMI",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0xff;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  if (strncmp(buffer,"STR",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0x50 | getReg(buffer); return 1; }
  if (strcmp(buffer,"STXD") == 0) { *b1 = 0x73; return 1; }
  if (strcmp(buffer,"XOR") == 0) { *b1 = 0xf3; return 1; }
  if (strncmp(buffer,"XRI",3) == 0) {
    buffer = nextWord(buffer); *b1 = 0xfb;
    buffer = toHex(buffer,&v); *b2 = v & 0xff; return 2; }
  return 0;
  }

void debug_dm(char* buffer) {
  char cbuffer[256];
  char sbuffer[20];
  word count;
  int lc;
  lc = 0;
  count = 256;
  cbuffer[0] = 0;
  buffer=getAddress(buffer,&lastAddr);
  if (*buffer == ',') {
    buffer++;
    buffer = toHex(buffer,&count);
    }
  while (count > 0) {
    if (lc == 0) {
      debugDisplay(cbuffer);
      sprintf(cbuffer,"%04x:",lastAddr);
      lc = 16;
      }
    sprintf(sbuffer," %02x",readMem(lastAddr++));
    strcat(cbuffer,sbuffer);
    lc--;
    count--;
    }
  debugDisplay(cbuffer);
  }

void debug_dem(char* buffer) {
  char cbuffer[256];
  char sbuffer[20];
  word count;
  int lc;
  if (useEMS != 'Y') {
    debugDisplay("EMS not configured");
    return;
    }
  lc = 0;
  count = 256;
  cbuffer[0] = 0;
  buffer=getAddress(buffer,&lastAddr);
  if (*buffer == ',') {
    buffer++;
    buffer = toHex(buffer,&count);
    }
  while (count > 0) {
    if (lc == 0) {
      debugDisplay(cbuffer);
      sprintf(cbuffer,"%04x:",lastAddr);
      lc = 16;
      }
    sprintf(sbuffer," %02x",ems[lastAddr++]);
    strcat(cbuffer,sbuffer);
    lc--;
    count--;
    }
  debugDisplay(cbuffer);
  }

void debug_sm(char* buffer) {
  word addr;
  word value;
  debugDisplay(buffer);
  buffer=getAddress(buffer,&addr);
  while (*buffer != 0) {
    buffer = nextWord(buffer);
    buffer = toHex(buffer,&value);
    writeMem(addr++,value & 0xff);
    }
  }

void debug_cp(char* buffer) {
  word dst;
  word src;
  word len;
  debugDisplay(buffer);
  buffer=getAddress(buffer,&src);
  buffer=nextWord(buffer);
  buffer=toHex(buffer,&dst);
  buffer=nextWord(buffer);
  buffer=toHex(buffer,&len);
  while (len > 0) {
    ram[dst++] = ram[src++];
    len--;
    }
  }

void debug_rom(char* buffer) {
  word dst;
  word src;
  word len;
  char flag;
  debugDisplay(buffer);
  buffer=getAddress(buffer,&src);
  buffer=nextWord(buffer);
  buffer=toHex(buffer,&dst);
  buffer=nextWord(buffer);
  len = src;
  flag = '*';
  while (flag == '*') {
    romMap[len>>3] |= (1<<(len & 7));
    ramMap[len>>3] &= ~(1<<(len & 7));
    len++;
    if (len == 0) flag=' ';
    if (len > dst) flag = ' ';
    }
  }

void debug_ram(char* buffer) {
  word dst;
  word src;
  word len;
  debugDisplay(buffer);
  buffer=getAddress(buffer,&src);
  buffer=nextWord(buffer);
  buffer=toHex(buffer,&dst);
  buffer=nextWord(buffer);
  for (len=src; len<=dst; len++) {
    ramMap[len>>3] |= (1<<(len & 7));
    romMap[len>>3] &= ~(1<<(len & 7));
    }
  }

void debug_as(char* buffer) {
  char sbuffer[256];
  buffer=getAddress(buffer,&debugAddr);
  sprintf(debugLine,"%04x: ",debugAddr);
  rcs_drawFilledBox(display,debugWindow,white,10,450,615,18);
  sprintf(sbuffer,"%s_",debugLine);
  rcs_drawString(display,debugWindow,black,12,464,sbuffer);
  debugAsm = 'Y';
  }

void disassemble(word start,word count) {
  int i,n;
  char buffer[256];
  char buffer2[256];
  char buffer3[20];
  while (count > 0) {
    i = ram[start];
    sprintf(buffer,"%04x: %02x ",start,i);
    strcpy(buffer2,"");
    n = i & 0xf;
    i >>= 4;
    start++;
    switch (i) {
      case 0x0:switch (n) {
                 case 0x0:strcat(buffer2,"IDL"); break;
                 default:sprintf(buffer2,"LDN  R%X",n);
                         break;
                 }
               break;
      case 0x1:sprintf(buffer2,"INC  R%X",n);
               break;
      case 0x2:sprintf(buffer2,"DEC  R%X",n);
               break;
      case 0x3:switch (n) {
                 case 0x0:sprintf(buffer2,"BR   %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 case 0x1:sprintf(buffer2,"BQ   %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 case 0x2:sprintf(buffer2,"BZ   %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 case 0x3:sprintf(buffer2,"BDF  %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 case 0x4:sprintf(buffer2,"B1   %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 case 0x5:sprintf(buffer2,"B2   %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 case 0x6:sprintf(buffer2,"B3   %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 case 0x7:sprintf(buffer2,"B4   %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 case 0x8:strcat(buffer2,"SKP"); break;
                 case 0x9:sprintf(buffer2,"BNQ  %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 case 0xa:sprintf(buffer2,"BNZ  %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 case 0xb:sprintf(buffer2,"BNF  %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 case 0xc:sprintf(buffer2,"BN1  %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 case 0xd:sprintf(buffer2,"BN2  %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 case 0xe:sprintf(buffer2,"BN3  %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 case 0xf:sprintf(buffer2,"BN4  %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 }
               break;
      case 0x4:sprintf(buffer2,"LDA  R%X",n);
               break;
      case 0x5:sprintf(buffer2,"STR  R%X",n);
               break;
      case 0x6:switch (n) {
                 case 0x0:strcat(buffer2,"IRX"); break;
                 case 0x1:
                 case 0x2:
                 case 0x3:
                 case 0x4:
                 case 0x5:
                 case 0x6:
                 case 0x7:sprintf(buffer2,"OUT  %X",n);
                          break;
                 case 0x9:
                 case 0xa:
                 case 0xb:
                 case 0xc:
                 case 0xd:
                 case 0xe:
                 case 0xf:sprintf(buffer2,"INP  %X",n-8);
                          break;
                 }
               break;
      case 0x7:switch (n) {
                 case 0x0:strcat(buffer2,"RET"); break;
                 case 0x1:strcat(buffer2,"DIS"); break;
                 case 0x2:strcat(buffer2,"LDXA"); break;
                 case 0x3:strcat(buffer2,"STXD"); break;
                 case 0x4:strcat(buffer2,"ADC"); break;
                 case 0x5:strcat(buffer2,"SDB"); break;
                 case 0x6:strcat(buffer2,"SHRC"); break;
                 case 0x7:strcat(buffer2,"SMB"); break;
                 case 0x8:strcat(buffer2,"SAV"); break;
                 case 0x9:strcat(buffer2,"MARK"); break;
                 case 0xa:strcat(buffer2,"REQ"); break;
                 case 0xb:strcat(buffer2,"SEQ"); break;
                 case 0xe:strcat(buffer2,"SHLC"); break;
                 case 0xc:sprintf(buffer2,"ADCI %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 case 0xd:sprintf(buffer2,"SDBI %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 case 0xf:sprintf(buffer2,"SMBI %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 }
               break;
      case 0x8:sprintf(buffer2,"GLO  R%X",n);
               break;
      case 0x9:sprintf(buffer2,"GHI  R%X",n);
               break;
      case 0xa:sprintf(buffer2,"PLO  R%X",n);
               break;
      case 0xb:sprintf(buffer2,"PHI  R%X",n);
               break;
      case 0xc:switch (n) {
                 case 0x0:sprintf(buffer2,"LBR  %04x",ram[start]<<8|ram[start+1]);
                          sprintf(buffer3,"%02x %02x ",ram[start],ram[start+1]);
                          strcat(buffer,buffer3);
                          start+=2; break;
                 case 0x1:sprintf(buffer2,"LBQ  %04x",ram[start]<<8|ram[start+1]);
                          sprintf(buffer3,"%02x %02x ",ram[start],ram[start+1]);
                          strcat(buffer,buffer3);
                          start+=2; break;
                 case 0x2:sprintf(buffer2,"LBZ  %04x",ram[start]<<8|ram[start+1]);
                          sprintf(buffer3,"%02x %02x ",ram[start],ram[start+1]);
                          strcat(buffer,buffer3);
                          start+=2; break;
                 case 0x3:sprintf(buffer2,"LBDF %04x",ram[start]<<8|ram[start+1]);
                          sprintf(buffer3,"%02x %02x ",ram[start],ram[start+1]);
                          strcat(buffer,buffer3);
                          start+=2; break;
                 case 0x4:strcat(buffer2,"NOP"); break;
                 case 0x5:strcat(buffer2,"LSNQ"); break;
                 case 0x6:strcat(buffer2,"LSNZ"); break;
                 case 0x7:strcat(buffer2,"LSNF"); break;
                 case 0x8:strcat(buffer2,"LSKP"); break;
                 case 0x9:sprintf(buffer2,"LBNQ %04x",ram[start]<<8|ram[start+1]);
                          sprintf(buffer3,"%02x %02x ",ram[start],ram[start+1]);
                          strcat(buffer,buffer3);
                          start+=2; break;
                 case 0xa:sprintf(buffer2,"LBNZ %04x",ram[start]<<8|ram[start+1]);
                          sprintf(buffer3,"%02x %02x ",ram[start],ram[start+1]);
                          strcat(buffer,buffer3);
                          start+=2; break;
                 case 0xb:sprintf(buffer2,"LBNF %04x",ram[start]<<8|ram[start+1]);
                          sprintf(buffer3,"%02x %02x ",ram[start],ram[start+1]);
                          strcat(buffer,buffer3);
                          start+=2; break;
                 case 0xc:strcat(buffer2,"LSIE"); break;
                 case 0xd:strcat(buffer2,"LSQ"); break;
                 case 0xe:strcat(buffer2,"LSZ"); break;
                 case 0xf:strcat(buffer2,"LSDF"); break;
                 }
               break;
      case 0xd:sprintf(buffer2,"SEP  R%X",n);
               break;
      case 0xe:sprintf(buffer2,"SEX  R%X",n);
               break;
      case 0xf:switch (n) {
                 case 0x0:strcat(buffer2,"LDX"); break;
                 case 0x1:strcat(buffer2,"OR"); break;
                 case 0x2:strcat(buffer2,"AND"); break;
                 case 0x3:strcat(buffer2,"XOR"); break;
                 case 0x4:strcat(buffer2,"ADD"); break;
                 case 0x5:strcat(buffer2,"SD"); break;
                 case 0x6:strcat(buffer2,"SHR"); break;
                 case 0x7:strcat(buffer2,"SM"); break;
                 case 0x8:sprintf(buffer2,"LDI  %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 case 0x9:sprintf(buffer2,"ORI  %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 case 0xa:sprintf(buffer2,"ANI  %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 case 0xb:sprintf(buffer2,"XRI  %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 case 0xc:sprintf(buffer2,"ADI  %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 case 0xd:sprintf(buffer2,"SDI  %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 case 0xe:strcat(buffer2,"SHL"); break;
                 case 0xf:sprintf(buffer2,"SMI  %02X",ram[start++]);
                          sprintf(buffer3,"%02x ",ram[start-1]);
                          strcat(buffer,buffer3);
                          break;
                 }

      }
    while (strlen(buffer) < 18) strcat(buffer," ");
    strcat(buffer,buffer2);
    debugDisplay(buffer);
    count--;
    }
  lastAddr = start;
  }

void debug_da(char* buffer) {
  word count;
  count = 16;
  buffer = getAddress(buffer,&lastAddr);
  if (*buffer == ',') {
    buffer++;
    buffer = toHex(buffer,&count);
    }
  disassemble(lastAddr,count);
  }

void debug_sr(char* buffer) {
  char reg[1024];
  char *pReg;
  word value;
  debugDisplay(buffer);
  buffer = nextWord(buffer);
  pReg = reg;
  while (*buffer != 0 && *buffer != ' ') *pReg++ = *buffer++;
  *pReg = 0;
  buffer = nextWord(buffer);
  buffer = toHex(buffer,&value);
  if (strcmp(reg,"P") == 0) cpu.p = value & 0xf;
  if (strcmp(reg,"X") == 0) cpu.x = value & 0xf;
  if (strcmp(reg,"D") == 0) cpu.d = value & 0xff;
  if (strcmp(reg,"DF") == 0) cpu.df = value & 0x1;
  if (strcmp(reg,"T") == 0) cpu.t = value & 0xff;
  if (strcmp(reg,"Q") == 0) cpu.q = value & 0x1;
  if (strcmp(reg,"R0") == 0) cpu.r[0] = value;
  if (strcmp(reg,"R1") == 0) cpu.r[1] = value;
  if (strcmp(reg,"R2") == 0) cpu.r[2] = value;
  if (strcmp(reg,"R3") == 0) cpu.r[3] = value;
  if (strcmp(reg,"R4") == 0) cpu.r[4] = value;
  if (strcmp(reg,"R5") == 0) cpu.r[5] = value;
  if (strcmp(reg,"R6") == 0) cpu.r[6] = value;
  if (strcmp(reg,"R7") == 0) cpu.r[7] = value;
  if (strcmp(reg,"R8") == 0) cpu.r[8] = value;
  if (strcmp(reg,"R9") == 0) cpu.r[9] = value;
  if (strcmp(reg,"RA") == 0) cpu.r[10] = value;
  if (strcmp(reg,"RB") == 0) cpu.r[11] = value;
  if (strcmp(reg,"RC") == 0) cpu.r[12] = value;
  if (strcmp(reg,"RD") == 0) cpu.r[13] = value;
  if (strcmp(reg,"RE") == 0) cpu.r[14] = value;
  if (strcmp(reg,"RF") == 0) cpu.r[15] = value;
  }

void debug_trap(char* buffer) {
  word count;
  byte b1,b2,b3;
  debugDisplay(buffer);
  buffer = nextWord(buffer);
  count = assemble(buffer,&b1,&b2,&b3);
  if (count > 0) {
    if (numTraps == 64) return;
    traps[numTraps][0] = count;
    traps[numTraps][1] = b1;
    traps[numTraps][2] = b2;
    traps[numTraps++][3] = b3;
    debugDisplayBP(); 
    }
  }

char* getRegister(char* buffer,char* dest) {
  *dest = 0;
  if (*buffer == 'D' && *(buffer+1) != 'F') { *dest = 'D'; buffer++; }
  else if (*buffer == 'D' && *(buffer+1) == 'F') { *dest = 'F'; buffer+=2; }
  else if (*buffer == 'P') { *dest = 'P'; buffer++; }
  else if (*buffer == 'X') { *dest = 'X'; buffer++; }
  else if (*buffer == 'T') { *dest = 'T'; buffer++; }
  else if (*buffer == 'Q') { *dest = 'Q'; buffer++; }
  else if (*buffer == 'R') {
    buffer++;
    switch (*buffer) {
      case '0': *dest = '0'; break;
      case '1': *dest = '1'; break;
      case '2': *dest = '2'; break;
      case '3': *dest = '3'; break;
      case '4': *dest = '4'; break;
      case '5': *dest = '5'; break;
      case '6': *dest = '6'; break;
      case '7': *dest = '7'; break;
      case '8': *dest = '8'; break;
      case '9': *dest = '9'; break;
      case 'A': *dest = 'a'; break;
      case 'B': *dest = 'b'; break;
      case 'C': *dest = 'c'; break;
      case 'D': *dest = 'd'; break;
      case 'E': *dest = 'e'; break;
      case 'F': *dest = 'f'; break;
      }
    buffer++;
    }
  return buffer;
  }

void debug_treg(char* buffer) {
  char reg;
  word value;
  debugDisplay(buffer);
  buffer = nextWord(buffer);
  buffer = getRegister(buffer,&reg);
  if (reg != 0) {
    buffer=getAddress(buffer,&value);
    if (numTregs == 64) return;
    tregs[numTregs][0] = reg;
    tregs[numTregs++][1] = value;
    debugDisplayBP(); 
    }
  }

void debug_tr(char* buffer) {
  buffer = nextWord(buffer);
  if (strcmp(buffer,"ON") == 0) trace = 'Y';
  if (strcmp(buffer,"OFF") == 0) trace = 'N';
  if (trace == 'Y') debugDisplay("Command trace enabled");
    else debugDisplay("Command trace disabled");
  }

void debug_st(char* buffer) {
  buffer = nextWord(buffer);
  steps = atol(buffer);
  if (steps <= 0) steps = 1;
  lastMsg = 0;
  }

void debug_rn(char* buffer) {
  word addr;
  buffer = nextWord(buffer);
  steps = -1;
  lastMsg = 0;
  if (*buffer == 0) return;
  buffer = toHex(buffer,&addr);
  cpu.r[cpu.p] = addr;
  }

void debug_sb(char* buffer) {
  word addr;
  debugDisplay(buffer);
  buffer=getAddress(buffer,&addr);
  if (numBP == 64) return;
  breakPoints[numBP++] = addr;
  debugDisplayBP(); 
  }

void debug_db(char* buffer) {
  int i;
  for (i=0; i<numBP; i++) disassemble(breakPoints[i],1);
  }

void debug_rb(char* buffer) {
  int i,j;
  word addr;
  debugDisplay(buffer);
  buffer=getAddress(buffer,&addr);
  for (i=0; i<numBP; i++) {
    if (breakPoints[i] == addr) {
      for (j=i; j<numBP-1; j++) breakPoints[j] = breakPoints[j+1];
      numBP--;
      }
    }
  debugDisplayBP(); 
  }

void debug_rt(char* buffer) {
  word count;
  byte b1,b2,b3;
  int i,j,f;
  debugDisplay(buffer);
  buffer = nextWord(buffer);
  count = assemble(buffer,&b1,&b2,&b3);
  if (count > 0) {
    f = -1;
    for (i=0; i<numTraps; i++) {
      if (traps[i][0] == 1 && traps[i][0] == count &&
          traps[i][1] == b1) f = i;
      if (traps[i][0] == 2 && traps[i][0] == count &&
          traps[i][1] == b1 && traps[i][2] == b2) f = i;
      if (traps[i][0] == 3 && traps[i][0] == count &&
          traps[i][1] == b1 && traps[i][2] == b2 && traps[i][3] == b3) f = i;
      }
      if (f >= 0) {
        for (j=f; j<numTraps-1; j++) {
          traps[j][0] = traps[j+1][0];
          traps[j][1] = traps[j+1][1];
          traps[j][2] = traps[j+1][2];
          traps[j][3] = traps[j+1][3];
          }
        numTraps--;
        }
    }
  debugDisplayBP(); 
  }

void debug_rr(char* buffer) {
  int f,i,j;
  char reg;
  word value;
  debugDisplay(buffer);
  buffer = nextWord(buffer);
  buffer = getRegister(buffer,&reg);
  if (reg != 0) {
    buffer=getAddress(buffer,&value);
    f = -1;
    for (i=0; i<numTregs; i++) {
      if (reg == tregs[i][0] && value == tregs[i][1]) f = i;
      }
    if (f >= 0) {
      for (j=f; j<numTregs-1; j++) {
        tregs[j][0] = tregs[j+1][0];
        tregs[j][1] = tregs[j+1][1];
        }
      numTregs--;
      }
    debugDisplayBP(); 
    }
  }

void debug_ds(char* buffer) {
  word adr;
  int i,j,k;
  int p,t,r;
  byte buf[256];
  adr = 0;
  p = 0;
  for (i=0; i<16; i++) {
    sprintf((char*)buf,"%04x: ",adr);
    adr+=4096;
    for (j=0; j<16; j++) {
      t = 0;
      r = 0;
      for (k=0; k<32; k++) {
        t |= romMap[p];
        r |= ramMap[p++];
        }
      if (t) strcat((char*)buf,"R ");
        else if (r) strcat((char*)buf,". ");
          else strcat((char*)buf,"  ");
      }
    debugDisplay((char*)buf);
    }
  }

void debug_help(char* buffer) {
  buffer=nextWord(buffer);
  debugDisplay("");
  if (strcmp(buffer,"?") == 0) debugDisplay("? [cmd] - Show help");
  else if (strcmp(buffer,"AS") == 0) debugDisplay("AS addr - Invoke Assembler");
  else if (strcmp(buffer,"CP") == 0) debugDisplay("CP src dst len - Copy len bytes of memory from src to dst");
  else if (strcmp(buffer,"CHR") == 0) debugDisplay("CHR xx - Display 1870 character pattern");
  else if (strcmp(buffer,"D1870") == 0) debugDisplay("D1870 - Display CDP1869/70 registers");
  else if (strcmp(buffer,"DA") == 0) debugDisplay("DA [addr][,cnt] - Dissassemble memory");
  else if (strcmp(buffer,"DB") == 0) debugDisplay("DB - Display Breakpoints");
  else if (strcmp(buffer,"DM") == 0) debugDisplay("DM [addr][,cnt] - Display memory");
  else if (strcmp(buffer,"DP") == 0) debugDisplay("DP - Display Current Memory Pointer");
  else if (strcmp(buffer,"DS") == 0) debugDisplay("DS - Display Storage Map");
  else if (strcmp(buffer,"INT") == 0) debugDisplay("INT - Trigger Interrupt");
  else if (strcmp(buffer,"RB") == 0) debugDisplay("RB addr - Remove Breakpoint");
  else if (strcmp(buffer,"RN") == 0) debugDisplay("RN [addr] - Run, addr as optional starting address");
  else if (strcmp(buffer,"SB") == 0) debugDisplay("SB addr - Set Breakpoint");
  else if (strcmp(buffer,"SM") == 0) debugDisplay("SM addr byte byte ... - Set Memory to byte list");
  else if (strcmp(buffer,"SR") == 0) debugDisplay("SR reg value - Set Register (P,X,D,DF,T,Q,R0-RF)");
  else if (strcmp(buffer,"ST") == 0) debugDisplay("ST [num] - Step 1 or num instructions (num is decimal)");
  else if (strcmp(buffer,"TR") == 0) debugDisplay("TR ON/OFF - Set Command trace ON or OFF");
  else if (strcmp(buffer,"TRAP") == 0) debugDisplay("TRAP asm_cmd - Trap on specified command");
  else if (strcmp(buffer,"RT") == 0) debugDisplay("RT asm_cmd - Remove specified command trap");
  else if (strcmp(buffer,"TREG") == 0) debugDisplay("TREG reg value - Set a register trap, traps when reg=value");
  else if (strcmp(buffer,"RR") == 0) debugDisplay("RR reg value - Remove a register trap");
  else if (strcmp(buffer,"DEM") == 0) debugDisplay("DEM addr - Display an EMS memory page");
  else if (strcmp(buffer,"ROM") == 0) debugDisplay("ROM start end - Specify memory from start-end as ROM");
  else if (strcmp(buffer,"RAM") == 0) debugDisplay("RAM start end - Specify memory from start-end as RAM");
  else {
    debugDisplay(buffer);
    debugDisplay("?      AS     CP     D1870  DA     DB     DM");
    debugDisplay("DP     DS     INT    RB     RN     RAM    ROM");
    debugDisplay("SB     SM     SR     ST     TR     TRAP   RT");
    debugDisplay("TREG   RR     DEM    CHR");
    debugDisplay("Use '? command' for a description of command");
    }
  }

void debugger() {
  int i;
  char buffer[1024];
  while (1) {
    printf("\n> ");
    fgets(buffer,1024,stdin);
    for (i=0; i<strlen(buffer); i++) {
      buffer[i]=(buffer[i]>='a' && buffer[i]<='z') ? buffer[i]-32 : buffer[i];
      if (buffer[i] > 0 && buffer[i] < ' ') buffer[i] = 0;
      }
    }
  }

void debug_dv(char* buffer) {
  word addr;
  FILE* fil;
  int  i;
  if (useTMS9918 == 'N') return;
  fil = fopen("vram.dmp","w");
  addr = 0;
  while (addr < 16384) {
    fprintf(fil,"%04x:",addr);
    for (i=0; i<16; i++) fprintf(fil," %02x",vdp.mem[addr+i]);
    fprintf(fil,"\n");
    addr += 16;
    }
  fclose(fil);
  }

void debug_d1870() {
  char buffer[64];
  sprintf(buffer,"REG3: %04x",v1870.reg3);
  debugDisplay(buffer);
  sprintf(buffer,"REG4: %04x",v1870.reg4);
  debugDisplay(buffer);
  sprintf(buffer,"REG5: %04x",v1870.reg5);
  debugDisplay(buffer);
  sprintf(buffer,"REG6: %04x",v1870.reg6);
  debugDisplay(buffer);
  sprintf(buffer,"REG7: %04x",v1870.reg7);
  debugDisplay(buffer);
  }

void debug_chr(char* buffer) {
  word addr;
  char pbuf[80];
  byte t;
  int i;
  int j;
  buffer=getAddress(buffer,&addr);
  sprintf(pbuf,"CHR %02X",addr);
  debugDisplay(pbuf);
  for (i=0; i<8; i++) {
    if (v1870.cram[addr*16+i] & 0x80) strcpy(pbuf,"1 ");
      else strcpy(pbuf,"0 ");
    if (v1870.cram[addr*16+i] & 0x40) strcat(pbuf,"1 ");
      else strcat(pbuf,"0 ");
    t = v1870.cram[addr*16+i] << 2;
    for (j=0; j<6; j++) {
      if (t & 0x80) strcat(pbuf,"*"); else strcat(pbuf," ");
      t <<= 1;
      }
    debugDisplay(pbuf);
    }
  }

void debug_mt(char* buffer) {
  memTrap ^= 1;
  if (memTrap) debugDisplay("memory trap on");
    else debugDisplay("memory trap off");
  }

void debugEvent(rcs_Event event) {
  int  i;
  word count;
  byte b1,b2,b3;
  rcs_GC gc;
  char buffer[1024];
  if (event.type == EVENT_EXPOSE) {
    gc=rcs_openGC(display,debugWindow);
    rcs_copyArea(display,debugBuffer,debugWindow,gc,0,0,640,480,0,0);
    for (i=0; i<17; i++) {
      rcs_drawString(display,debugWindow,black,410,436-(16-i)*16,debugLines[i]);
      }
    rcs_closeGC(display,gc);
    resetDisplay();
    updateDebugger();
    rcs_drawFilledBox(display,debugWindow,white,10,450,615,18);
    sprintf(buffer,"%s_",debugLine);
    rcs_drawString(display,debugWindow,black,12,464,buffer);
    debugDisplayBP();
    }
  if (event.type == EVENT_KEY_PRESS) {
    if (event.d1 == KEY_BACK_SP) {
      if (strlen(debugLine)>0) {
        debugLine[strlen(debugLine)-1] = 0;
        rcs_drawFilledBox(display,debugWindow,white,10,450,615,18);
        sprintf(buffer,"%s_",debugLine);
        rcs_drawString(display,debugWindow,black,12,464,buffer);
        }
      }
    if (event.d1 == 13) {
      uc(debugLine);
      if (debugAsm == 'Y') {
        debugDisplay(debugLine);
        count = assemble(debugLine+6,&b1,&b2,&b3);
        if (count >0) {
          ram[debugAddr++] = b1;
          if (count > 1) ram[debugAddr++] = b2;
          if (count > 2) ram[debugAddr++] = b3;
          sprintf(debugLine,"%04x: ",debugAddr);
          rcs_drawFilledBox(display,debugWindow,white,10,450,615,18);
          sprintf(buffer,"%s_",debugLine);
          rcs_drawString(display,debugWindow,black,12,464,buffer);
          return;
          } debugAsm = 'N';
        } else {
        if (debugLine[0] == 0) debug_st(debugLine);
        if (strncmp(debugLine,"?",1) == 0) debug_help(debugLine);
        if (strncmp(debugLine,"AS",2) == 0) { debug_as(debugLine); return; }
        if (strncmp(debugLine,"CP",2) == 0) debug_cp(debugLine);
        if (strncmp(debugLine,"DA",2) == 0) debug_da(debugLine);
        if (strncmp(debugLine,"DM",2) == 0) debug_dm(debugLine);
        if (strncmp(debugLine,"DS",2) == 0) debug_ds(debugLine);
        if (strncmp(debugLine,"SM",2) == 0) debug_sm(debugLine);
        if (strncmp(debugLine,"TRAP",4) == 0) debug_trap(debugLine);
        if (strncmp(debugLine,"TREG",4) == 0) debug_treg(debugLine);
        if (strncmp(debugLine,"RT",2) == 0) debug_rt(debugLine);
        if (strncmp(debugLine,"TR ",3) == 0) debug_tr(debugLine);
        if (strncmp(debugLine,"SR",2) == 0) debug_sr(debugLine);
        if (strncmp(debugLine,"SB",2) == 0) debug_sb(debugLine);
        if (strncmp(debugLine,"DB",2) == 0) debug_db(debugLine);
        if (strncmp(debugLine,"RB",2) == 0) debug_rb(debugLine);
        if (strncmp(debugLine,"ST",2) == 0) debug_st(debugLine);
        if (strncmp(debugLine,"RN",2) == 0) debug_rn(debugLine);
        if (strncmp(debugLine,"RR",2) == 0) debug_rr(debugLine);
        if (strncmp(debugLine,"DV",2) == 0) debug_dv(debugLine);
        if (strncmp(debugLine,"MT",2) == 0) debug_mt(debugLine);
        if (strncmp(debugLine,"DEM",3) == 0) debug_dem(debugLine);
        if (strncmp(debugLine,"ROM",3) == 0) debug_rom(debugLine);
        if (strncmp(debugLine,"RAM",3) == 0) debug_ram(debugLine);
        if (strcmp(debugLine,"INT") == 0) cdp1802Int(&cpu);
        if (strcmp(debugLine,"D1870") == 0) debug_d1870();
        if (strncmp(debugLine,"CHR",3) == 0) debug_chr(debugLine);
        if (strcmp(debugLine,"DP") == 0) {
          sprintf(buffer,"%04x:",lastAddr);
          debugDisplay(buffer);
          }
        }
      
      debugLine[0] = 0;
      rcs_drawFilledBox(display,debugWindow,white,10,450,615,18);
      sprintf(buffer,"%s_",debugLine);
      rcs_drawString(display,debugWindow,black,12,464,buffer);
      }
    if (event.d1 >= ' ' && event.d1<=127) {
      debugLine[strlen(debugLine)+1] = 0;
      debugLine[strlen(debugLine)] = event.d1;
      rcs_drawFilledBox(display,debugWindow,white,10,450,615,18);
      sprintf(buffer,"%s_",debugLine);
      rcs_drawString(display,debugWindow,black,12,464,buffer);
      }
    }
  }

