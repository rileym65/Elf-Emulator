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

DEVICE extPortsOut[256];
DEVICE extPortsIn[256];

void configurePortExt(char* arg) {
  int port;
  portStruct ports;
  ports = parseArgs(arg);
  port = (ports.outPorts[0] == 0) ? 4 : ports.outPorts[0] - 1;
  outPorts[port].out = portExtSelect;
  port = (ports.outPorts[1] == 0) ? 5 : ports.outPorts[1] - 1;
  outPorts[port].out = portExtOut;
  port = (ports.inPorts[0] == 0) ? 5 : ports.inPorts[0] - 1;
  outPorts[port].in = portExtIn;
  for (port=0; port<256; port++) {
    extPortsOut[port].out = NULL;
    extPortsIn[port].in = NULL;
    }
  extPort = 0;
  }

void portExtSelect(byte value) {
  extPort = value;
  }

void portExtOut(byte value) {
  if (extPortsOut[extPort].out != NULL) extPortsOut[extPort].out(value);
  }

byte portExtIn() {
  if (extPortsIn[extPort].in != NULL) return extPortsIn[extPort].in();
  return 255;
  }

