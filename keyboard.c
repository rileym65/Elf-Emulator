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
#include <stdlib.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
#include "elf.h"

char         keyboardValue;
byte         keyboardEF;
int          keyCycles;
byte         lastCode;

int next;

void configureKeyboard(char* arg) {
  int port;
  portStruct ports;
printf("configuring normal keyboard\n");
  ports = parseArgs(arg);
  port = (ports.inPorts[0] == 0) ? 6 : ports.inPorts[0] - 1;
  keyboardEF = 1;
  useKeyboard = 'Y';
  inPorts[port].in = keyboardIn;
  port = (ports.ef[0] == 0) ? 2 : ports.ef[0] - 1;
  switch (port) {
    case 0:efDevices[0].ef1 = getKeyboardEF; break;
    case 1:efDevices[1].ef2 = getKeyboardEF; break;
    case 2:efDevices[2].ef3 = getKeyboardEF; break;
    case 3:efDevices[3].ef4 = getKeyboardEF; break;
    }
  inPorts[port].cycle = keyboardCycle;
  keyCycles = 500000;
next = 128;
  }

void keyboardCycle() {
  if (keyFile != 0 && keyboardEF == 1 && keyCycles == 0) {
    if (read(keyFile,&keyboard,1) == 0) {
      close(keyFile);
      keyFile = 0;
      }
    keyCycles = 1000;
    if (keyboard == 10) keyboard = 13;
    if (keyboard == 13 && lastCode == 13) keyboard = 0;
    lastCode = keyboard;
    if (keyboard == 13) keyCycles = 130000;
    }
  if (keyCycles > 0 && keyboardEF == 1) keyCycles--;
  if (keyboard<0) keyboard = 0;
  if (keyboard != 0 && (byte)keyboard <= 127) {
    if (forceUC == 'Y' && keyboard >= 'a' && keyboard <= 'z')
      keyboard -= 32;
    keyboardValue = keyboard;
    keyboard = 0;
    keyboardEF = 0;
    }
  }

byte keyboardIn() {
  byte ret;
  keyboardEF = 1;
  ret = keyboardValue;
  return ret;
  }

byte getKeyboardEF() {
  return keyboardEF;
  }

