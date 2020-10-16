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
#include "elf.h"

void configurePrinter(char* arg) {
  int port;
  portStruct ports;
  ports = parseArgs(arg);
  port = (ports.outPorts[0] == 0) ? 6 : ports.outPorts[0] - 1;
  usePrinter = 'Y';
  outPorts[port].out = printerOut;
  }

void printerOut(byte value) {
  FILE* printer;
  printer = fopen("printer.out","a");
  fprintf(printer,"%c",value);
  fclose(printer);
  }

