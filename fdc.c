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
#include <stdlib.h>
#include <stdio.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
#include <fcntl.h>
#include "elf.h"

byte fdcEF;

void configureFdc(char* arg) {
  int port;
  portStruct ports;
  ports = parseArgs(arg);
  port = (ports.outPorts[0] == 0) ? 1 : ports.outPorts[0] - 1;
  outPorts[port].out = fdcRegisterSelect;
  outPorts[port].cycle = fdcCycle;
  port = (ports.outPorts[1] == 0) ? 2 : ports.outPorts[1] - 1;
  outPorts[port].out = fdcOut;
  port = (ports.inPorts[0] == 0) ? 2 : ports.inPorts[0] - 1;
  inPorts[port].in = fdcIn;
  port = (ports.ef[0] == 0) ? 1 : ports.ef[0] - 1;
  switch (port) {
    case 0:efDevices[0].ef1 = getFdcEF; break;
    case 1:efDevices[1].ef2 = getFdcEF; break;
    case 2:efDevices[2].ef3 = getFdcEF; break;
    case 3:efDevices[3].ef4 = getFdcEF; break;
    }
  fdcMode = 'R';
  }

void fdcRegisterSelect(byte value) {
  fdc.registerSelect = value;
  }

void fdcOut(byte value) {
  switch (fdc.registerSelect) {
    case 0:fdc_command(&fdc,value,&cpu); break;
    case 1:fdc.track = value; break;
    case 2:fdc.sector = value; break;
    case 3:fdc_writeData(&fdc,value,&cpu); break;
    case 4:fdc.drive = value; break;
    }
  }

byte fdcIn() {
  switch (fdc.registerSelect) {
    case 0:return fdc.status;
    case 1:return fdc.track;
    case 2:return fdc.sector;
    case 3:return fdc_readData(&fdc,&cpu);
    }
  return 0;
  }

byte getFdcEF() {
  return fdcEF;
  }

void fdcCycle() {
  fdc_cycle(&fdc,&cpu);
  }

int fdc_drive(int drive) {
  switch (drive) {
    case 1:return 0;
    case 2:return 1;
    case 4:return 2;
    default:return 3;
    }
  }

int fdc_stepRate(int rate) {
  switch (rate) {
    case 0:return CPMS * 6;
    case 1:return CPMS * 12;
    case 2:return CPMS * 20;
    case 3:return CPMS * 30;
    }
  return CPMS * 12;
  }

byte _fdc_readSector(FDC* fdc) {
  byte drive;
  int  disk;
  long offset;
  byte status;
  status=0;
  switch (fdc->drive) {
    case 1:drive=0; break;
    case 2:drive=1; break;
    case 4:drive=2; break;
    default:drive=3; break;
    }
  disk=open(fdc->drives[drive],O_RDONLY | O_CREAT, 0666);
  if (disk == -1) status=0x80;
    else {
    offset=(fdc->exec == 0xf4) ? fdc->fmtTrack : fdc->track;
    offset *= ((fdc->density != 'S') ? 18 : 10);
    offset += ((fdc->exec == 0xf4) ? fdc->fmtSector : fdc->sector);
    offset *= 256;
    lseek(disk,offset,SEEK_SET);
    read(disk,fdc->buffer,256);
    close(disk);
    status = 0;
    }
  return status;
  }

byte _fdc_writeSector(FDC* fdc) {
  byte drive;
  int  disk;
  long offset;
  byte status;
  status=0;
  switch (fdc->drive) {
    case 1:drive=0; break;
    case 2:drive=1; break;
    case 4:drive=2; break;
    default:drive=3; break;
    }
  disk=open(fdc->drives[drive],O_WRONLY | O_CREAT, 0666);
  if (disk == -1) status=0x80;
    else {
    offset=(fdc->exec == 0xf4) ? fdc->fmtTrack : fdc->track;
    offset *= ((fdc->density != 'S') ? 18 : 10);
    offset += ((fdc->exec == 0xf4) ? fdc->fmtSector : fdc->sector);
    offset *= 256;
    lseek(disk,offset,SEEK_SET);
    write(disk,fdc->buffer,256);
    close(disk);
    }
  return status;
  }

void fdc_reset(FDC* fdc) {
  int i;
  fdc->track=0;
  fdc->trackI=0;
  fdc->sector=0;
  fdc->drive=1;
  fdc->status=4;
  fdc->command=255;
  fdc->exec=255;
  fdc->stepDir='I';
  fdc->density='D';
  fdc->cycles = 0;
  for (i=0; i<4; i++) fdc->driveTrack[i] = 0;
  }

void _fdc_endCmd(FDC* fdc,byte status) {
  fdc->status=status;
  fdc->command=0xff;
  fdc->exec=0xff;
  }

void fdc_command(FDC* fdc,byte command,CDP1802* cpu) {
  fdc->command=command;
  if ((command & 0xf0) == 0) {
    fdc->exec = fdc->command;
    fdc->cycles = cpu->cycles + fdc_stepRate(command & 3);
    fdc->hld = (command & 0x04) ? 1 : 0;
    fdc->status = 1;
    }
  else if ((command & 0xf0) == 0x10) {           /* seek */
    fdc->exec = fdc->command;
    fdc->cycles = cpu->cycles + fdc_stepRate(command & 3);
    fdc->hld = (command & 0x04) ? 1 : 0;
    fdc->status = 1;
    }
  else if ((command & 0xe0) == 0x20) {           /* step */
    fdc->exec = fdc->command;
    fdc->cycles = cpu->cycles + fdc_stepRate(command & 3);
    fdc->hld = (command & 0x04) ? 1 : 0;
    fdc->status = 1;
    }
  else if ((command & 0xe0) == 0x40) {           /* step in */
    fdc->exec = fdc->command;
    fdc->cycles = cpu->cycles + fdc_stepRate(command & 3);
    fdc->hld = (command & 0x04) ? 1 : 0;
    fdc->stepDir = 'I';
    fdc->status = 1;
    }
  else if ((command & 0xe0) == 0x60) {           /* step out */
    fdc->exec = fdc->command;
    fdc->cycles = cpu->cycles + fdc_stepRate(command & 3);
    fdc->hld = (command & 0x04) ? 1 : 0;
    fdc->stepDir = 'O';
    fdc->status = 1;
    }

  else if ((command & 0xf0) == 0x80) {           /* Read Sector */
    fdc->exec = 0x80;
    fdc->cycles = cpu->cycles + 22000;
    fdc->drq = 0;
    fdcEF = 1;
    fdc->pointer=fdc->buffer;
    fdc->status = 1;
    fdc->readStatus=_fdc_readSector(fdc);
    if (fdc->readStatus == 0x80) {
      fdc->status=0x80;
      fdc->exec=0xff;
      }
    }
  else if ((command & 0xf0) == 0xa0) {           /* Write Sector */
    fdc->exec = 0xa0;
    fdc->cycles = cpu->cycles + 22000;
    fdc->drq = 0;
    fdcEF = 1;
    fdc->pointer=fdc->buffer;
    fdc->status = 1;
    }

  else if ((command & 0xf0) == 0x70) {           /* Interrupt set */
    _fdc_endCmd(fdc,0);
    }
  else if ((command & 0xf0) == 0xc0) {           /* Read Address */
    _fdc_endCmd(fdc,0);
    }
  else if ((command & 0xf0) == 0xe0) {           /* Read track */
    _fdc_endCmd(fdc,0);
    }
  else if (command == 0xf4) {           /* Write track */
    fdc->exec=command;
    fdc->pointer=fdc->buffer;
    fdc->command=0xff;
    fdc->status=0x03;
    fdc->idam=0;
    fdc->fmtCount=0;
    }
  else {
    }
  }

byte fdc_readData(FDC* fdc,CDP1802* cpu) {
  if (fdc->exec == 0x81) {
    fdc->drq = 0;
    fdc->status &=0xfd;
    fdcEF = 1;
    }
  return fdc->data;
  }

void fdc_writeData(FDC* fdc,byte data,CDP1802* cpu) {
  fdc->data = data;
  if (fdc->exec == 0xa1) {
    fdc->drq = 0;
    fdc->status &=0xfd;
    fdcEF = 1;
    }
  }

void fdca_writeData(FDC* fdc,byte data) {
  fdc->data=data;
  if (fdc->exec == 0xf4) {
    if (++fdc->fmtCount == ((fdc->density != 'D') ? 3128 : 6256)) {
      fdc->status=0;
      }
    switch (fdc->idam) {
      case 0:                                    /* not in any fields */
           if (data == 0xfe) {                   /* begin idam */
             fdc->idam=1;
             }
           if (data >= 0xf8 && data <=0xfb) {    /* data field */
             fdc->pointer=fdc->buffer;
             fdc->idam = data;
             }
           break;
      case 1:                                    /* track of idam */
           fdc->fmtTrack=data;
           fdc->idam++;
           break;
      case 2:                                    /* should be zero */
           fdc->idam++;
           break;
      case 3:                                    /* sector of idam */
           fdc->fmtSector=data;
           fdc->idam++;
           break;
      case 4:                                    /* should be zero */
           fdc->idam++;
           break;
      case 5:                                    /* look for crc byte */
           if (data == 0xf7) {
             fdc->idam=0;
             }
      case 0xf8:                                /* in data field */
      case 0xf9:
      case 0xfa:
      case 0xfb:
           *(fdc->pointer++) = data;
           if (fdc->pointer == &fdc->buffer[256]) {
             _fdc_writeSector(fdc);
             fdc->idam = 0;
             }
           break;
      }
    }
  if (fdc->exec == 0xa0) {
    *(fdc->pointer++) = data;
    if (fdc->pointer == &fdc->buffer[256]) {
      fdc->exec=0xff;
      fdc->status=_fdc_writeSector(fdc);
      }
    }
  }

void fdc_cycle(FDC* fdc,CDP1802* cpu) {
  byte status;
  if (fdc->exec == 0xff) return;
  if (cpu->cycles < fdc->cycles) return;
  switch (fdc->exec & 0xf0) {
    case 0:                                        /* restore */
           if (fdc->driveTrack[fdc_drive(fdc->drive)] == 0) {
             fdc->track = 0;
             status = 4;
             status |= (fdc->hld) ? 0x20 : 0;
             _fdc_endCmd(fdc,status);
             return;
             }
           fdc->stepDir = 'O';
           fdc->driveTrack[fdc_drive(fdc->drive)]--;
           fdc->cycles = cpu->cycles + fdc_stepRate(fdc->exec & 3);
           break;
    case 0x10:                                     /* seek */
           if (fdc->track == fdc->data) {
             if (fdc->exec & 0x4) {
               fdc->exec = 0xf1; 
               fdc->cycles = cpu->cycles + 44744;
               return;
               }
             status = 0;
             if (fdc->driveTrack[fdc_drive(fdc->drive)] == 0) status += 4;
             status |= (fdc->hld) ? 0x20 : 0;
             _fdc_endCmd(fdc,status);
             return;
             }
           fdc->cycles = cpu->cycles + fdc_stepRate(fdc->exec & 3);
           if (fdc->track < fdc->data) {
             fdc->track++;
             fdc->stepDir = 'I';
             fdc->driveTrack[fdc_drive(fdc->drive)]++;
             if (fdc->driveTrack[fdc_drive(fdc->drive)] > 40) 
               fdc->driveTrack[fdc_drive(fdc->drive)]=40;
             } else { 
             fdc->stepDir = 'O';
             fdc->track--;
             fdc->driveTrack[fdc_drive(fdc->drive)]--;
             if (fdc->driveTrack[fdc_drive(fdc->drive)] < 0) 
               fdc->driveTrack[fdc_drive(fdc->drive)]=0;
             }
           break;
    case 0x20:                                     /* step, no update */
    case 0x30:                                     /* step, with update */
           if (fdc->stepDir == 'I') {
             fdc->driveTrack[fdc_drive(fdc->drive)]++;
             if (fdc->driveTrack[fdc_drive(fdc->drive)] > 40) 
               fdc->driveTrack[fdc_drive(fdc->drive)]=40;
             if ((fdc->exec & 0xf0) == 0x30) fdc->track++;
             if (fdc->track > 254) fdc->track = 254;
             } else {
             fdc->driveTrack[fdc_drive(fdc->drive)]--;
             if (fdc->driveTrack[fdc_drive(fdc->drive)] < 0) 
               fdc->driveTrack[fdc_drive(fdc->drive)]=0;
             if ((fdc->exec & 0xf0) == 0x30) fdc->track--;
             if (fdc->track == 255) fdc->track = 0;
             }
             if (fdc->exec & 0x4) {
               fdc->exec = 0xf1; 
               fdc->cycles = cpu->cycles + 44744;
               return;
               }
             status = 0;
             if (fdc->driveTrack[fdc_drive(fdc->drive)] == 0) status += 4;
             status |= (fdc->hld) ? 0x20 : 0;
             _fdc_endCmd(fdc,status);
             return;
           break;
    case 0x40:                                     /* step in, no update */
    case 0x50:                                     /* step in, with update */
             fdc->driveTrack[fdc_drive(fdc->drive)]++;
             if (fdc->driveTrack[fdc_drive(fdc->drive)] > 40) 
               fdc->driveTrack[fdc_drive(fdc->drive)]=40;
             if ((fdc->exec & 0xf0) == 0x50) fdc->track++;
             if (fdc->track > 254) fdc->track = 254;
             if (fdc->exec & 0x4) {
               fdc->exec = 0xf1; 
               fdc->cycles = cpu->cycles + 44744;
               return;
               }
             status = 0;
             if (fdc->driveTrack[fdc_drive(fdc->drive)] == 0) status += 4;
             status |= (fdc->hld) ? 0x20 : 0;
             _fdc_endCmd(fdc,status);
             return;
           break;
    case 0x60:                                     /* step out, no update */
    case 0x70:                                     /* step out, with update */
             fdc->driveTrack[fdc_drive(fdc->drive)]--;
             if (fdc->driveTrack[fdc_drive(fdc->drive)] < 0) 
               fdc->driveTrack[fdc_drive(fdc->drive)]=0;
             if ((fdc->exec & 0xf0) == 0x70) fdc->track--;
             if (fdc->track == 255) fdc->track = 0;
             if (fdc->exec & 0x4) {
               fdc->exec = 0xf1; 
               fdc->cycles = cpu->cycles + 44744;
               return;
               }
             status = 0;
             if (fdc->driveTrack[fdc_drive(fdc->drive)] == 0) status += 4;
             status |= (fdc->hld) ? 0x20 : 0;
             _fdc_endCmd(fdc,status);
             return;
           break;
      }
  switch (fdc->exec) {
    case 0x80:                                    /* initial portion of read */
             fdc->data = *(fdc->pointer++);
             fdc->exec = 0x81;
             fdc->drq = 1;
             fdc->status |=0x02;
             fdcEF = 0;
             fdc->cycles = cpu->cycles + 90;
             break;
    case 0x81:                                    /* main body of read */
             if (fdc->drq) {
               }
             fdc->cycles = cpu->cycles + 90;
             if (fdc->pointer == &fdc->buffer[256]) fdc->exec = 0x82;
               else {
                 fdc->data = *(fdc->pointer++);
                 fdc->drq = 1;
                 fdc->status |=0x02;
                 fdcEF = 0;
                 }
             break;
    case 0x82:                                    /* clean up read */
             fdc->drq = 0;
             fdc->status = 0;
             fdcEF = 1;
             fdc->exec = 0xff;
             break;
    case 0xa0:                                    /* initial portion of write */
             fdc->exec=0xa1;
             fdc->drq = 1;
             fdc->status |=0x02;
             fdcEF = 0;
             fdc->cycles = cpu->cycles + 90;
             break;
    case 0xa1:                                    /* main body of write */
             if (fdc->drq) {
printf("missed data\n");
               }
             fdc->cycles = cpu->cycles + 90;
             *(fdc->pointer++) = fdc->data;
             if (fdc->pointer == &fdc->buffer[256]) fdc->exec = 0xa2;
               else {
                 fdc->drq = 1;
                 fdc->status |=0x02;
                 fdcEF = 0;
                 }
             break;
    case 0xa2:                                    /* clean up write */
             fdc->drq = 0;
             fdc->status = 0;
             fdcEF = 1;
             fdc->exec = 0xff;
             _fdc_writeSector(fdc);
             break;
    }
  }

