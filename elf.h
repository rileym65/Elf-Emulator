#include "librcs.h"
#include <time.h>

#undef LINK
#define LINK extern

#ifdef  WIN32
typedef unsigned char byte;
typedef unsigned short word;
#endif

#ifdef MAIN
#undef LINK
#define LINK
#endif

#define CLOCK    1789773
#define CPS      CLOCK/8
#define CPMS     CPS/1000
#define OFF    1


#define MSG_BP        1
#define MSG_INST_TRAP 2
#define MSG_REG_TRAP  3

typedef struct {
  byte  regs[8];
  byte  mem[16384];
  byte  mode;
  word  nameAddr;
  word  colorAddr;
  word  patternAddr;
  word  spAttrAddr;
  word  spPatternAddr;
  word  curAddr;
  rcs_Window window;
  rcs_GC colors[16];
  byte   toggle;
  byte   value;
  } TMS9918;

typedef struct {
  word cylinders;
  word heads;
  word sectors;
  long maxLba;
  } DRIVE_GEOMETRY;

typedef struct {
  byte secBuf[512];
  word bufPos;
  byte rError;
  byte rSecCount;
  byte rStartSec;
  word rCylinder;
  byte rHeadDev;
  byte rStatus;
  byte rFeatures;
  byte rCommand;
  byte command;
  byte rAStatus;
  byte rInt;
  char* drive[2];
  DRIVE_GEOMETRY geometry[2];
  int registerSelect;
  byte dataMode;
#ifdef WIN32
  long cycles;
#else
  long long cycles;
#endif
  } IDE;

typedef struct {
  byte inPorts[7];
  byte outPorts[7];
  byte q;
  byte ef[4];
  byte intr;
  } portStruct;

typedef struct {
  byte idam;
  byte track;
  byte trackI;
  byte sector;
  byte drive;
  byte status;
  byte readStatus;
  byte command;
  byte data;
  byte exec;
  byte density;
  byte buffer[1024];
  long count;
  byte hld;
  byte drq;
  byte *pointer;
  char drives[4][80];
  char driveTypes[4];
  int  driveTrack[4];
  char stepDir;
  byte ddsl;
  byte ddsls[4];
  int  fdcType;
  byte fmtTrack;
  byte fmtSector;
  int  fmtCount;
  int  registerSelect;
#ifdef WIN32
  long cycles;
#else
  long long cycles;
#endif
  } FDC ;

typedef struct {
  int state;
  int x;
  int y;
  } PUSH;
  
typedef struct {
  int state;
  int x;
  int y;
  } TOGGLE;
  
typedef struct {
  int state;
  int x;
  int y;
  } REDLED;


typedef struct {
  int  number;
  char blanked;
  int  x;
  int  y;
  int  dpstate;
  word last;
  char segments[16];
  } TIL311;

typedef struct {
  word r[16];
  byte d;
  byte df;
  byte b;
  byte p;
  byte x;
  byte n;
  byte i;
  byte t;
  byte ie;
  byte ci;
  byte xi;
  byte cie;
  byte xie;
  byte q;
  byte ef;
  byte nlines;
  word address;
  byte idle;
  byte counter;
  byte ch;
  byte ctrMode;
  byte ctrRunning;
  byte ctrPre;
  byte tq;
  char mode;
  int  wait;
  int  clear;
  int  cycleCount;
#ifdef WIN32
  long cycles;
#else
  long long  cycles;
#endif
  char cpuMode;
  void (*writeMem)(word addr,byte value);
  void (*writePort)(byte port,byte value);
  byte (*readMem)(word addr);
  byte (*readPort)(byte port);
  void (*debug)(char* buffer);
  } CDP1802;

typedef struct {
  CDP1802*  cpu;
  void (*init)();
  void (*event)(rcs_Event event);
  } MACHINE;

typedef struct {
  byte (*in)();
  void (*out)(byte value);
  void (*init)();
  void (*cycle)();
  void (*q)(byte value);
  byte (*ef1)();
  byte (*ef2)();
  byte (*ef3)();
  byte (*ef4)();
  } DEVICE;

typedef struct {
  word addr;
  word reg3;
  word reg4;
  word reg5;
  word reg6;
  word reg7;
  int  cycle;
  byte cram[4096];
  byte pram[4096];
  byte pcb[4096];
  } V1870;

LINK DEVICE inPorts[7];
LINK DEVICE outPorts[7];
LINK DEVICE qDevice;
LINK DEVICE efDevices[4];

LINK rcs_Display* display;
LINK rcs_Window   window;
LINK rcs_Pixmap   screen;

LINK rcs_Window   hexWindow;
LINK rcs_Pixmap   hexBuffer;

LINK rcs_Window   ledWindow;
LINK rcs_Pixmap   ledBuffer;

LINK rcs_Window   hexOutWindow;
LINK rcs_Pixmap   hexOutBuffer;

LINK rcs_Window   pixieWindow;
LINK rcs_Pixmap   pixieBuffer;

LINK rcs_Window   vt100Window;
LINK rcs_Pixmap   vt100Buffer;

LINK rcs_Window   v1870Window;
LINK rcs_Pixmap   v1870Buffer;

LINK rcs_Window   v6845Window;
LINK rcs_Pixmap   v6845Buffer;

LINK rcs_Window   debugWindow;
LINK rcs_Pixmap   debugBuffer;

LINK rcs_Window   clockWindow;
LINK rcs_Pixmap   clockBuffer;

LINK rcs_Window  tms9918w;

LINK TIL311       addr[4];
LINK TIL311       data[2];
LINK TIL311       hexDisp[4];
LINK TOGGLE       datasw[8];
LINK TOGGLE       ef[4];
LINK TOGGLE       load;
LINK TOGGLE       run;
LINK TOGGLE       mp;
LINK TOGGLE       power;
LINK PUSH         dmain;
LINK CDP1802      cpu;
LINK REDLED       q;
LINK REDLED       keypadHi;
LINK REDLED       keypadLo;
LINK REDLED       led[8];
LINK REDLED       superLeds[8];
LINK byte*        ram;
LINK char         debugMode;
LINK int          maxRam;
LINK char         use6845;
LINK char         eprom;
LINK char         keypad;
LINK char         hexOut;
LINK char         ledModule;
LINK char         nextNybble;
LINK char         useKeyboard;
LINK char         usePrinter;
LINK char         useVt100;
LINK char         usePS2;
LINK char         useTMS9918;
LINK rcs_Event    ps2Event;
LINK byte         keypadValue;
LINK byte         graphics;
LINK byte         graphicsOn;
LINK byte         graphicsMode;
LINK long         graphicsNext;
LINK byte         showAddress;
LINK rcs_GC       red;
LINK rcs_GC       black;
LINK rcs_GC       white;
LINK rcs_GC       gc;
LINK long         iCount;
LINK rcs_Pixmap   til311;
LINK char         computer;
LINK int          wx;
LINK int          wy;
LINK byte         switches;
LINK byte         monitor;
LINK char         keyboard;
LINK long         vtCount;
LINK int          baud;
LINK int          vtBits;
LINK byte         rs232;
LINK int          vtOutBits;
LINK long         vtOutCount;
LINK int          vtOut;
LINK byte         parPort;
LINK FDC          fdc;
LINK char         fdcMode;
LINK word         romLo;
LINK word         romHi;
LINK time_t       startTime;
LINK time_t       endTime;
LINK byte         forceUC;
LINK int          mainX;
LINK int          mainY;
LINK int          pixieX;
LINK int          pixieY;
LINK int          vt100X;
LINK int          vt100Y;
LINK int          hexX;
LINK int          hexY;
LINK int          extPort;
LINK int          pager[16];
LINK char         usePager;
LINK char         trace;
LINK long         steps;
LINK word         breakPoints[64];
LINK int          numBP;
LINK byte         traps[64][4];
LINK int          numTraps;
LINK word         tregs[64][2];
LINK int          numTregs;
LINK IDE          ide;
LINK int          keyFile;
LINK byte         pixieEF;
LINK int          charLine;
LINK byte         debugOpen;
LINK byte         clockOpen;
LINK word         lastR[16];
LINK byte         lastD;
LINK byte         lastP;
LINK byte         lastX;
LINK byte         lastT;
LINK byte         lastDF;
LINK byte         lastQ;
LINK byte         lastIE;
LINK byte         lastEF1;
LINK byte         lastEF2;
LINK byte         lastEF3;
LINK byte         lastEF4;
LINK byte         lastOUT[8];
LINK byte         outValues[8];
LINK byte         lastIN[8];
LINK byte         inValues[8];
LINK char         debugLine[255];
LINK byte         debugAsm;
LINK word         debugAddr;
LINK int          lastMsg;
#ifdef WIN32
LINK DWORD        lastMS;
#else
LINK long         lastMS;
#endif
LINK long         instCount;
LINK int          fixedClock;
LINK byte         studioKeys[2][10];
LINK int          studioPort;
LINK byte         ef1Sense;
LINK byte         ef2Sense;
LINK byte         ef3Sense;
LINK byte         ef4Sense;
LINK byte         romMap[16385];
LINK byte         ramMap[16385];
LINK MACHINE      machine;
LINK char      pressed;
LINK char      inPressed;
LINK int       control;
LINK char      lastMode2;
LINK char      singleStep;
LINK char      state;
LINK char      serialLog;
LINK char      superBoard;
LINK char      revQ;
LINK TMS9918   vdp;
LINK char      useGray;
LINK char      useZoom;
LINK char      ps2Intr;
LINK char      use1870;
LINK byte      cycle0;
LINK byte      memTrap;
LINK byte      useEMS;
LINK byte*     ems;
LINK byte      emsPage;
LINK char      diskName[256];
LINK V1870     v1870;
LINK char      debugLines[17][40];
LINK int          ctrlState;
LINK byte      comxVidMode;
LINK byte      debugLog;
LINK byte      v1870Pcb;
LINK char      cidelsa;
LINK byte      cid1;
LINK byte      cid2;
LINK byte      cid4;
LINK byte      cidEF2;
LINK int       vScale;
LINK int       memSwitch;
LINK byte      picoelf;

extern byte readMem(word addr);

extern rcs_Pixmap til311Init(rcs_Display* d,rcs_Window w);
extern TIL311 til311New();
extern void til311SetNumber(TIL311* til,int n);
extern void til311SetPosition(TIL311* til,int px,int py);
extern void til311Paint(TIL311* til,rcs_Display* display,rcs_Window win);
extern void til311Update(TIL311* til,rcs_Display* d,rcs_Window win);

extern void configureLedModule(char* arg);
extern void ledWrite(byte value);
extern void ledEvent(rcs_Event event);

extern REDLED  redLedNew();
extern void    redLedSetPosition(REDLED*,int,int);
extern void    redLedSetState(REDLED*,int);
extern int     redLedGetState(REDLED*);
extern void    redLedPaint(REDLED*,rcs_Display*,rcs_Window);
extern CDP1802 cdp1802New();
extern void    cdp1802Cycle(CDP1802* cpu);
extern void    cdp1802DmaIn(CDP1802* cpu,byte value);
extern byte    cdp1802DmaOut(CDP1802* cpu);
extern void    cdp1802Int(CDP1802* cpu);
extern void    cdp1802Reset(CDP1802* cpu);
extern void    cdp1802SetEF(CDP1802* cpu,int flag,int value);
extern void    cdp1802SetClear(CDP1802* cpu,int value);
extern void    cdp1802SetWait(CDP1802* cpu,int value);

extern TOGGLE  toggleNew();
extern void    toggleSetPosition(TOGGLE*,int,int);
extern void    toggleSetState(TOGGLE*,int);
extern int     toggleGetState(TOGGLE*);
extern void    togglePaint(TOGGLE*,rcs_Display*,rcs_Window);

extern PUSH    pushNew();
extern void    pushSetPosition(PUSH*,int,int);
extern void    pushSetState(PUSH*,int);
extern int     pushGetState(PUSH*);
extern void    pushPaint(PUSH*,rcs_Display*,rcs_Window);

extern void configureVt100(char* arg);
extern void vt100Cycle();
extern void vt100Out(byte value);
extern byte getVt100EF();
extern void vt100Display(int byt);
extern void vt100Event(rcs_Event event);
extern void vt100Init();
extern void vt100Close();
extern int vt100GetKey();
extern void vt100Reset();

extern void fdc_reset(FDC* fdc);
extern void fdc_command(FDC* fdc,byte command,CDP1802* cpu);
extern byte fdc_readData(FDC* fdc,CDP1802* cpu);
extern void fdc_writeData(FDC* fdc,byte data,CDP1802* cpu);
extern void fdc_cycle(FDC* fdc,CDP1802* cpu);
extern void configureFdc(char* arg);
extern void fdcRegisterSelect(byte value);
extern void fdcOut(byte value);
extern byte fdcIn();
extern byte getFdcEF();
extern void fdcCycle();

extern void pixiePlot(int x,int y,int c);
extern void pixieEvent(rcs_Event event);
extern void configurePixie(char* arg);
extern void pixieCycle();
extern void pixieOut(byte value);
extern byte pixieIn();
extern byte getPixieEF();

extern void ledEvent(rcs_Event event);
extern void keypadEvent(rcs_Event event);
extern void hexOutEvent(rcs_Event event);

extern void showData(byte val);
extern portStruct parseArgs(char* arg);
extern byte getData();
extern void showAddr(word adr);
extern void paint(int x1,int y1,int x2,int y2);
extern void machineCycle(CDP1802* cpu,int cycle);

extern void configurePrinter(char* arg);
extern void printerOut(byte value);

extern void configureKeyboard(char* arg);
extern byte keyboardIn();
extern byte getKeyboardEF();
extern void keyboardCycle();

extern void configurePortExt(char* arg);
extern void portExtSelect(byte value);
extern void portExtOut(byte value);
extern byte portExtIn();

extern void disassemble(word start,word count);

extern void configureIde(char* arg);
extern void ideRegisterSelect(byte value);
extern void ideOut(byte value);
extern byte ideIn();
extern void ideCycle();

extern void debugEvent(rcs_Event event);
extern void updateDebugger();
extern void resetDisplay();
extern void debugTrace(char* buffer);
extern void debugDisplay(char* buffer);

extern void clockEvent(rcs_Event event);
extern void showClock();

extern void configurePortStudio2();
extern void studio2Out(byte value);
extern byte studio2Ef3();
extern byte studio2Ef4();

extern MACHINE newHbElf();
extern MACHINE newElf2();
extern MACHINE newSuper();
extern MACHINE newStudio2();
extern MACHINE newComx35();

extern void configurePs2Keyboard(char* arg);
extern void ps2keyboardCycle();
extern void ps2keyboardOut(byte value);
extern byte ps2keyboardIn();
extern byte ps2getKeyboardEF();

extern void v6845Write(word addr,byte value);
extern void v6845OutEvent(rcs_Event event);

extern TMS9918 tms9918_init();
extern void tms9918_destroy(TMS9918* vdp);
extern void tms9918_configure(char* arg);
extern void tms9918_event(rcs_Event event);
extern void tms9918_modeLowOut(byte value);
extern void tms9918_modeHighOut(byte value);


extern void v1870_configure(char* arg);
extern V1870 v1870_init();
extern void v1870_event(rcs_Event event);
extern byte v1870_read(word addr);
extern void v1870_write(word addr,byte v);

extern void configureComx(char* arg);
extern void writeMem(word addr,byte value);

