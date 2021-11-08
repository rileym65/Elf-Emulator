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
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#define O_BINARY 0
#endif
#include <fcntl.h>
#include "elf.h"

#define IDE_STAT_ERROR       1
#define IDE_STAT_INDEX       2
#define IDE_STAT_ECC         4
#define IDE_STAT_DRQ         8
#define IDE_STAT_SKC        16
#define IDE_STAT_WFT        32
#define IDE_STAT_RDY        64
#define IDE_STAT_BSY       128

#define IDE_CMD_RESET        1
#define IDE_CMD_SETF         2
#define IDE_CMD_WRITE        3
#define IDE_CMD_WRITE_1      4
#define IDE_CMD_READ         5
#define IDE_CMD_READ_1       6
#define IDE_CMD_ID           7

void initIde(IDE* ide);
void ide_writeReg(IDE* ide,int reg,word value);
word ide_readReg(IDE* ide,int reg);
void ide_command(IDE* ide);
void setGeometry(IDE* ide,int cyl,int hd,int sc);

void configureIde(char* arg) {
  int port;
  portStruct ports;
  int tr,hd,sc;
  tr = 512;
  hd = 4;
  sc = 26;
  if (arg[5] == '(') {
    sscanf(&arg[6],"%d,%d,%d",&tr,&hd,&sc);
printf("%d, %d, %d\n",tr,hd,sc);
    }
  ports = parseArgs(arg);
  port = (ports.outPorts[0] == 0) ? 1 : ports.outPorts[0] - 1;
  outPorts[port].out = ideRegisterSelect;
  outPorts[port].cycle = ideCycle;
  port = (ports.outPorts[1] == 0) ? 2 : ports.outPorts[1] - 1;
  outPorts[port].out = ideOut;
  port = (ports.inPorts[0] == 0) ? 2 : ports.inPorts[0] - 1;
  inPorts[port].in = ideIn;
  printf("Using disk geometry: %dT, %dH, %dS\n",tr,hd,sc);
  setGeometry(&ide,tr,hd,sc);
  initIde(&ide);
  }

void ideRegisterSelect(byte value) {
  ide.registerSelect = value;
  }

void ideOut(byte value) {
  ide_writeReg(&ide,ide.registerSelect,value);
  }

byte ideIn() {
  byte ret;
  ret = ide_readReg(&ide,ide.registerSelect);
  return ret;
  }

void ideCycle() {
  if (--ide.cycles > 0) return;
  if (ide.cycles < 0) ide.cycles = 0;
  if (ide.command != 0) ide_command(&ide);
  }

long ideGetOffset(IDE* ide) {
  long ret;
  int  drive;
  byte sec;
  drive = (ide->rHeadDev & 16) ? 1 : 0;
  if (ide->rHeadDev & 64) {
    ret = ide->rStartSec;
    ret |= (ide->rCylinder << 8);
    ret |= ((ide->rHeadDev & 0xf) << 24);
    if (ret >= ide->geometry[drive].maxLba) {
      ide->rError |= 4;
      ide->rStatus |= 1;
      return -1;
      }
    } else {
    ret = ide->rCylinder;
    if (ret >= ide->geometry[drive].cylinders) {
      ide->rError |= 4;
      ide->rStatus |= 1;
      return -1;
      }
    sec = ide->rStartSec;
    sec--;
    if (sec >= ide->geometry[drive].sectors) {
      ide->rError |= 4;
      ide->rStatus |= 1;
      return -1;
      }
    ret *= sec;
    sec = ide->rHeadDev & 0xf;
    if (sec >= ide->geometry[drive].heads) {
      ide->rError |= 4;
      ide->rStatus |= 1;
      return -1;
      }
    ret *= sec;
    }
  ret *= 512;
  return ret;
  }

void ideWriteSector(IDE* ide) {
  int fildes;
  int drive;
  long offset;
  offset = ideGetOffset(ide);
  if (offset < 0) {
    printf("seek error\n");
    return;
    }
  drive = (ide->rHeadDev & 16) ? 1 : 0;
  fildes = open(ide->drive[drive],O_WRONLY | O_CREAT | O_BINARY, 0666);
  if (fildes == -1) {
    ide->rError |= 1;
    ide->rStatus |= 1;
    return;
    }
  lseek(fildes,offset,SEEK_SET);
  write(fildes,ide->secBuf,512);
  close(fildes);
  }

void ideReadSector(IDE* ide) {
  int fildes;
  int drive;
  long offset;
  offset = ideGetOffset(ide);
  if (offset < 0) return;
  drive = (ide->rHeadDev & 16) ? 1 : 0;
  fildes = open(ide->drive[drive],O_RDONLY | O_CREAT | O_BINARY, 0666);
  if (fildes == -1) {
    ide->rError |= 1;
    ide->rStatus |= 1;
    return;
    }
  lseek(fildes,offset,SEEK_SET);
  read(fildes,ide->secBuf,512);
  close(fildes);
  }

void ideReadID(IDE* ide) {
  int i;
  for (i=0; i<512; i++) ide->secBuf[i] = 0;
  ide->secBuf[2] = ide->geometry[0].cylinders % 256;
  ide->secBuf[3] = ide->geometry[0].cylinders / 256;
  ide->secBuf[6] = ide->geometry[0].heads % 256;
  ide->secBuf[7] = ide->geometry[0].heads / 256;
  ide->secBuf[12] = ide->geometry[0].sectors % 256;
  ide->secBuf[13] = ide->geometry[0].sectors / 256;
  ide->secBuf[108] = ide->geometry[0].cylinders % 256;
  ide->secBuf[109] = ide->geometry[0].cylinders / 256;
  ide->secBuf[110] = ide->geometry[0].heads % 256;
  ide->secBuf[111] = ide->geometry[0].heads / 256;
  ide->secBuf[112] = ide->geometry[0].sectors % 256;
  ide->secBuf[113] = ide->geometry[0].sectors / 256;
  ide->secBuf[114] = ide->geometry[0].maxLba & 255;
  ide->secBuf[115] = (ide->geometry[0].maxLba >> 8) & 255;
  ide->secBuf[116] = (ide->geometry[0].maxLba >> 16) & 255;
  ide->secBuf[117] = (ide->geometry[0].maxLba >> 24) & 255;
  ide->secBuf[120] = ide->geometry[0].maxLba & 255;
  ide->secBuf[121] = (ide->geometry[0].maxLba >> 8) & 255;
  ide->secBuf[122] = (ide->geometry[0].maxLba >> 16) & 255;
  ide->secBuf[123] = (ide->geometry[0].maxLba >> 24) & 255;
  }

void setGeometry(IDE* ide,int cyl,int hd,int sc) {
  ide->geometry[0].cylinders = cyl;
  ide->geometry[1].cylinders = cyl;
  ide->geometry[0].heads = hd;
  ide->geometry[1].heads = hd;
  ide->geometry[0].sectors = sc;
  ide->geometry[1].sectors = sc;
  ide->geometry[0].maxLba = ide->geometry[0].cylinders *
                            ide->geometry[0].heads *
                            ide->geometry[0].sectors;
  ide->geometry[1].maxLba = ide->geometry[1].cylinders *
                            ide->geometry[1].heads *
                            ide->geometry[1].sectors;
  }

void initIde(IDE* ide) {
/*
  ide->geometry[0].cylinders = 1201;
  ide->geometry[1].cylinders = 1201;
  ide->geometry[0].heads = 4;
  ide->geometry[1].heads = 4;
  ide->geometry[0].sectors = 26;
  ide->geometry[1].sectors = 26;
*/

/*
  ide->geometry[0].cylinders = 512;
  ide->geometry[1].cylinders = 512;
  ide->geometry[0].heads = 4;
  ide->geometry[1].heads = 4;
  ide->geometry[0].sectors = 26;
  ide->geometry[1].sectors = 26;
  ide->geometry[0].maxLba = ide->geometry[0].cylinders *
                            ide->geometry[0].heads *
                            ide->geometry[0].sectors;
  ide->geometry[1].maxLba = ide->geometry[1].cylinders *
                            ide->geometry[1].heads *
                            ide->geometry[1].sectors;
*/
  ide->rStatus = 0x40;
  ide->rAStatus = 1;
  ide->rError = 0;
  ide->rInt = 1;
  ide->rSecCount = 1;
  ide->rStartSec = 1;
  ide->rCylinder = 0;
  ide->rHeadDev = 0xe0;
  ide->dataMode = 16;
  ide->command = 0;
  ide->drive[0] = diskName;
  ide->drive[1] = "disk2.ide";
  }

void ide_writeReg(IDE* ide,int reg,word value) {
  switch (reg) {
    case 0x00:if (ide->bufPos >= 512) break;               /* Data */
              if (ide->dataMode == 16) {
                ide->secBuf[ide->bufPos++] = value & 0xff;        
                ide->secBuf[ide->bufPos++] = value >> 8;        
                } else {
                ide->secBuf[ide->bufPos++] = value & 0xff;        
                }
              if (ide->bufPos >= 512) ide->rStatus &= (~IDE_STAT_DRQ);
              break;
    case 0x01:ide->rFeatures = value;                      /* precomp */
              break;
    case 0x02:ide->rSecCount = value;                      /* sector count */
              break;
    case 0x03:ide->rStartSec = value;                      /* start sector */
    case 0x04:ide->rCylinder = (ide->rCylinder & 0xff00) | /* cylinder lo */
                   (value & 0xff);
              break;
    case 0x05:ide->rCylinder = (ide->rCylinder & 0xff) |   /* cylinder hi */
                   ((value & 0xff) << 8);
              break;
    case 0x06:ide->rHeadDev = value;                       /* head/device */
    case 0x07:if (!(ide->rStatus & 128)) {                    /* command */
                switch (value) {
 
                  case 0x20:ide->command = IDE_CMD_READ;
                            ide->cycles = 100;
                            ide->rStatus = IDE_STAT_RDY |
                                           IDE_STAT_BSY;
                            break;
                  case 0x30:ide->command = IDE_CMD_WRITE;
                            ide->cycles = 100;
                            ide->rStatus = IDE_STAT_RDY |
                                           IDE_STAT_BSY;
                            break;
                  case 0xec:ide->command = IDE_CMD_ID;
                            ide->cycles = 100;
                            ide->rStatus = IDE_STAT_RDY |
                                           IDE_STAT_BSY;
                            break;
                  case 0xef:ide->command = IDE_CMD_SETF;
                            ide->cycles = 100;
                            ide->rStatus = IDE_STAT_RDY |
                                           IDE_STAT_BSY;
                            break;
                  }
                }
              break;
    case 0x0e:ide->rInt = value;                           /* set inter */
printf("IDE reg E\n");
              if (value & 2) {
                ide->command = IDE_CMD_RESET;
                ide->cycles = 1000;
                ide->rStatus = IDE_STAT_RDY |
                               IDE_STAT_BSY;
                }
              break;
    case 0x0f:break;                                       /* unknown */
    }
  }

word ide_readReg(IDE* ide,int reg) {
  word ret;
  ret = 0;
  switch (reg) {
    case 0x00:if (ide->dataMode == 16) {                   /* Data */
                ret = ide->secBuf[ide->bufPos++];
                ret |= (ide->secBuf[ide->bufPos++] << 8);
                } else {
                ret = ide->secBuf[ide->bufPos++];
                }
              if (ide->bufPos >= 512) ide->rStatus &= (~IDE_STAT_DRQ);
              break;
    case 0x01:ret = ide->rError;                           /* Error */
              break;
    case 0x02:ret = ide->rSecCount;                        /* sector count */
              break;
    case 0x03:ret = ide->rStartSec;                        /* start sector */
              break;
    case 0x04:ret = ide->rCylinder & 0xff;                 /* cylinder lo */
              break;
    case 0x05:ret = ide->rCylinder >> 8;                   /* cylinder hi */
              break;
    case 0x06:ret = ide->rHeadDev;                         /* head/device */
              break;
    case 0x07:ret = ide->rStatus;                          /* status */
              if (ide->rStatus & 0x80) ide->rStatus &= 0x7f;
              break;
    case 0x0e:ret = ide->rStatus;                          /* status */
              if (ide->rStatus & 0x80) ide->rStatus &= 0x7f;
              break;
    case 0x0f:ret = ide->rAStatus;                         /* active status */
              break;
    }
  return ret;
  }

void ide_command(IDE* ide) {
  ide->rStatus &= 0xfe;
  switch (ide->command) {
    case IDE_CMD_ID:
         ide->command = IDE_CMD_READ_1;
         ideReadID(ide);
         ide->bufPos = 0;
         ide->rStatus |= IDE_STAT_DRQ;
         break;
    case IDE_CMD_READ:
         ide->command = IDE_CMD_READ_1;
         ideReadSector(ide);
         ide->bufPos = 0;
         ide->rStatus |= IDE_STAT_DRQ;
         break;
    case IDE_CMD_READ_1:
         if (ide->bufPos >= 512) {
           ide->rStatus &= (~IDE_STAT_BSY);
           ide->rStatus &= (~IDE_STAT_DRQ);
           ide->command = 0;
           }
         break;
    case IDE_CMD_WRITE:
         ide->command = IDE_CMD_WRITE_1;
         ide->bufPos = 0;
         ide->rStatus |= IDE_STAT_DRQ;
         break;
    case IDE_CMD_WRITE_1:
         if (ide->bufPos >= 512) {
           ide->rError = 0;
           ideWriteSector(ide);
           ide->rStatus &= (~IDE_STAT_BSY);
           ide->command = 0;
           }
         break;
    case IDE_CMD_RESET:
         ide->command = 0;
         ide->rStatus = IDE_STAT_RDY;
         initIde(ide); break;
 
    case IDE_CMD_SETF:switch (ide->rFeatures) {
                case 0x01:ide->dataMode = 8; break;
                case 0x81:ide->dataMode = 16; break;
                }
              ide->command = 0;
              ide->rStatus = IDE_STAT_RDY;
              break;
    }
  }

