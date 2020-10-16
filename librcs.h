#ifndef _LIBRCS_H
#define _LIBRCS_H

#include <stdio.h>
#include <sys/types.h>

#define LINK extern

#define _BYTE_
#define _WORD_

#ifdef SOLARIS
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;
#endif

#ifdef WIN32
#include <windows.h>
#undef LINK
#ifdef DLL
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif
typedef HWND rcs_Window;
typedef HWND rcs_Pixmap;
typedef HFONT rcs_Font;
typedef struct {
  HINSTANCE    hInstance;
  TCHAR        className[80];
  rcs_Window*  pixmapIDs;
  int          numPixmaps;
  } rcs_Display;
typedef struct _GC {
  COLORREF foreground;
  COLORREF background;
  HFONT    font;
  } *rcs_GC;
#endif

#ifdef X
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#define XK_MISCELLANY
#include <X11/keysymdef.h>
typedef Window rcs_Window;
typedef Pixmap rcs_Pixmap;
typedef Display rcs_Display;
typedef Font rcs_Font;
typedef GC rcs_GC;
#define KEY_F1        XK_F1
#define KEY_F2        XK_F2
#define KEY_F3        XK_F3
#define KEY_F4        XK_F4
#define KEY_F5        XK_F5
#define KEY_F6        XK_F6
#define KEY_F7        XK_F7
#define KEY_F8        XK_F8
#define KEY_F9        XK_F9
#define KEY_F10       XK_F10
#define KEY_F11       XK_F11
#define KEY_F12       XK_F12
#define KEY_LEFT      XK_Left
#define KEY_RIGHT     XK_Right
#define KEY_UP        XK_Up
#define KEY_DOWN      XK_Down
#define KEY_KP_LEFT   XK_KP_Left
#define KEY_KP_RIGHT  XK_KP_Right
#define KEY_KP_UP     XK_KP_Up
#define KEY_KP_DOWN   XK_KP_Down
#define KEY_KP_0      XK_KP_0   
#define KEY_KP_1      XK_KP_1   
#define KEY_KP_2      XK_KP_2   
#define KEY_KP_3      XK_KP_3   
#define KEY_KP_4      XK_KP_4   
#define KEY_KP_5      XK_KP_5   
#define KEY_KP_6      XK_KP_6   
#define KEY_KP_7      XK_KP_7   
#define KEY_KP_8      XK_KP_8   
#define KEY_KP_9      XK_KP_9   
#define KEY_KP_S5     XK_KP_5   
#define KEY_PAGE_UP   XK_Page_Up
#define KEY_PAGE_DN   XK_Page_Down
#define KEY_KP_PAGE_UP   XK_KP_Page_Up
#define KEY_KP_PAGE_DN   XK_KP_Page_Down
#define KEY_HOME      XK_Home
#define KEY_END       XK_End
#define KEY_KP_HOME      XK_KP_Home
#define KEY_KP_END       XK_KP_End
#define KEY_TAB       XK_Tab
#define KEY_BACK_SP   XK_BackSpace
#define KEY_INSERT    XK_Insert
#define KEY_DELETE    XK_Delete
#define KEY_CAPS_LK   XK_Caps_Lock
#define KEY_SHIFT_L   XK_Shift_L
#define KEY_SHIFT_R   XK_Shift_R
#define KEY_CTRL_L    XK_Control_L
#define KEY_CTRL_R    XK_Control_R
#define KEY_ALT_L     XK_Alt_L
#define KEY_ALT_R     XK_Alt_R
#define KEY_NUM_LK    XK_Num_Lock
#define KEY_PRINT     XK_Print
#define KEY_SCRL_LK   XK_Scroll_Lock
#define KEY_PAUSE     XK_Pause
#define KEY_KP_DIV    XK_KP_Divide
#define KEY_KP_MUL    XK_KP_Multiply
#define KEY_KP_SUB    XK_KP_Subtract
#define KEY_KP_ADD    XK_KP_Add
#define KEY_KP_ENTER  XK_KP_Enter
#endif

#ifdef WIN32
#define uid_t         unsigned int
#define gid_t         unsigned int
#define pid_t         unsigned int
#define mode_t        unsigned int
#define KEY_F1        (VK_F1+0xf000)
#define KEY_F2        (VK_F2+0xf000)
#define KEY_F3        (VK_F3+0xf000)
#define KEY_F4        (VK_F4+0xf000)
#define KEY_F5        (VK_F5+0xf000)
#define KEY_F6        (VK_F6+0xf000)
#define KEY_F7        (VK_F7+0xf000)
#define KEY_F8        (VK_F8+0xf000)
#define KEY_F9        (VK_F9+0xf000)
#define KEY_F10       (VK_F10+0xf000)
#define KEY_F11       (VK_F11+0xf000)
#define KEY_F12       (VK_F12+0xf000)
#define KEY_UP        (VK_UP+0xf000)
#define KEY_DOWN      (VK_DOWN+0xf000)
#define KEY_LEFT      (VK_LEFT+0xf000)
#define KEY_RIGHT     (VK_RIGHT+0xf000)
#define KEY_HOME      (VK_HOME+0xf000)
#define KEY_END       (VK_END+0xf000)
#define KEY_PAGE_UP   (VK_PRIOR+0xf000)
#define KEY_PAGE_DN   (VK_NEXT+0xf000)
#define KEY_INSERT    (VK_INSERT+0xf000)
#define KEY_DELETE    (VK_DELETE+0xf000)
#define KEY_KP_S5     (VK_CLEAR+0xf000)
#define KEY_KP_MUL    (VK_MULTIPLY+0xf000)
#define KEY_KP_DIV    (VK_DIVIDE+0xf000)
#define KEY_KP_ADD    (VK_ADD+0xf000)
#define KEY_KP_SUB    (VK_SUBTRACT+0xf000)
#define KEY_SHIF _L   (VK_SHIFT+0xf000)
#define KEY_SHIFT_R   (VK_SHIFT+0xf000)
#define KEY_CTRL_L    (VK_CONTROL+0xf000)
#define KEY_CTRL_R    (VK_CONTROL+0xf000)
#define KEY_KP_0      (VK_NUMPAD0+0xf000)
#define KEY_KP_1      (VK_NUMPAD1+0xf000)
#define KEY_KP_2      (VK_NUMPAD2+0xf000)
#define KEY_KP_3      (VK_NUMPAD3+0xf000)
#define KEY_KP_4      (VK_NUMPAD4+0xf000)
#define KEY_KP_5      (VK_NUMPAD5+0xf000)
#define KEY_KP_6      (VK_NUMPAD6+0xf000)
#define KEY_KP_7      (VK_NUMPAD7+0xf000)
#define KEY_KP_8      (VK_NUMPAD8+0xf000)
#define KEY_KP_9      (VK_NUMPAD9+0xf000)
#define KEY_TAB       9
#define KEY_BACK_SP   8
#endif

#ifdef WIN32
typedef unsigned short word;
typedef unsigned char  byte;
#endif

#ifdef MSDOS
typedef unsigned char byte;
typedef unsigned int  word;
typedef int           int16;
typedef long          int32;
#endif

#ifdef UNIX
#include <sys/time.h>
#include <termios.h>
typedef unsigned char byte;
typedef unsigned short  word;
typedef short         int16;
typedef int           int32;
typedef struct termios rcs_Termio;
#endif

#define MTERM_UP      257
#define MTERM_DOWN    258
#define MTERM_LEFT    259
#define MTERM_RIGHT   260

#define XML_ELEMENT   0
#define XML_ATTRIBUTE 1
#define XML_CDATA     2

#define EVENT_NONE           0
#define EVENT_MOTION         1
#define EVENT_BUTTON_PRESS   2
#define EVENT_BUTTON_RELEASE 3
#define EVENT_EXPOSE         4
#define EVENT_KEY_PRESS      5
#define EVENT_KEY_RELEASE    6
#define EVENT_CONFIGURE      7
#define EVENT_LEAVE          8
#define EVENT_ENTER          9
#define EVENT_FOCUSIN        10
#define EVENT_FOCUSOUT       11
#define EVENT_CLOSE          98
#define EVENT_QUIT           99

#define LN10                 2.30258509299405E+000
#define ONEOVERLN10          0.43429448190325E+000
#define PI                   3.1415927
#define PIOVER180            1.74532925199433E-002
#define PIUNDER180           5.72957795130823E+001

typedef struct {
  long recNum;
  union {
    char skey[80];
    long lkey;
    } data;
  } rcs_key;

typedef struct {
  long year;
  long month;
  long day;
  } rcs_dt;

typedef struct {
  byte type;
  char* name;
  int    numElements;
  void** elements;
  } XML_NODE;

typedef struct {
  int file;
  int status;
  } VMEM;

typedef struct {
  double r,i;
  } COMPLEX;
  
typedef struct {
  double m[4][4];
  } MATRIX;

typedef struct {
  double x;
  double y;
  double z;
  } VECTOR;

typedef struct {
  int x;
  int y;
  int z;
  } IVECTOR;

typedef struct {
  int *pointList;
  int pointCount;
  int maxPoints;
  } POLYGON;
  
typedef struct {
  VECTOR *vertexList;
  VECTOR *renderList;
  POLYGON *polygonList;
  double worldx;
  double worldy;
  double worldz;
  int maxVertex;
  int maxPoly;
  int vertexCount;
  int polyCount;
  } GOBJECT;

typedef struct {
  GOBJECT* objectList;
  int maxObjects;
  int objectCount;
  } WORLD;
  
#ifdef X
typedef Window AWWINDOW;
#else
struct AWWINDOW {
  int       x,y;
  int       w,h;
  char      visible;
  struct AWWINDOW* parent;
  struct AWWINSTACK* children;
  int       num_children;
  };
#endif

#ifndef X
struct AWWINSTACK {
  struct AWWINDOW* window;
  struct AWWINSTACK* next;
  };
#endif
    
#define rcs_GCForeground 1l<<2
#define rcs_GCBackground 1l<<3
#define rcs_GCLineWidth  1l<<4
#define rcs_GCLineStyle  1l<<5
#define AWRootWindow   0xffffffff

#ifdef X
typedef XGCValues rcs_GCValues;
#else
typedef struct {
  unsigned long foreground;
  unsigned long background;
  int           line_width;
  int           line_style;
  } rcs_GCValues;
#endif

typedef struct {
  int          type;
  rcs_Window   window;
  int          d1,d2,d3,d4,d5;
  } rcs_Event;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
LINK void rcs_initWin32(HINSTANCE hInstance);
#endif

/* **************** math ******************* */
LINK double  rcs_cosD(double a);
LINK double  rcs_degrees(double a);
LINK double  rcs_frac(double x);
LINK double  rcs_max(double a,double b);
LINK double  rcs_max3(double a,double b,double c);
LINK double  rcs_max4(double a,double b,double c,double d);
LINK double  rcs_min(double a,double b);
LINK double  rcs_min3(double a,double b,double c);
LINK double  rcs_min4(double a,double b,double c,double d);
LINK double  rcs_power(double base,int exp);
LINK int     rcs_powerInt(int base,int exp);
LINK double  rcs_radians(double a);
LINK int     rcs_round(double x);
LINK double  rcs_sign(double x);
LINK int     rcs_signInt(int x);
LINK double  rcs_sinD(double a);
LINK double  rcs_sqr(double x);
LINK int     rcs_sqrInt(int x);
LINK int     rcs_sqrtInt(int x);
LINK int     rcs_trunc(double x);
LINK double  rcs_acos(double n);
LINK double  rcs_acosec(double n);
LINK double  rcs_acosech(double n);
LINK double  rcs_acosh(double n);
LINK double  rcs_acot(double n);
LINK double  rcs_acoth(double n);
LINK double  rcs_asec(double n);
LINK double  rcs_asech(double n);
LINK double  rcs_asin(double n);
LINK double  rcs_asinh(double n);
LINK double  rcs_atan(double n);
LINK double  rcs_atanh(double n);
LINK double  rcs_cos(double angle);
LINK double  rcs_cosec(double n);
LINK double  rcs_cosech(double n);
LINK double  rcs_cosh(double angle);
LINK double  rcs_cot(double n);
LINK double  rcs_coth(double n);
LINK double  rcs_exp(double n);
LINK double  rcs_exp10(double n);
LINK double  rcs_log(double n);
LINK double  rcs_log10(double n);
LINK double  rcs_pow(double x,double y);
LINK double  rcs_root(double n,double rt);
LINK double  rcs_sec(double n);
LINK double  rcs_sech(double n);
LINK double  rcs_sin(double angle);
LINK double  rcs_sinh(double angle);
LINK double  rcs_sqrt(double n);
LINK double  rcs_tan(double n);
LINK double  rcs_tanh(double n);

/* **************** random **************** */
LINK int     rcs_random(int size);
LINK void    rcs_randomize();

/* **************** complex **************** */
LINK double rcs_compabs(COMPLEX a);
LINK COMPLEX rcs_compadd(COMPLEX a,COMPLEX b);
LINK COMPLEX rcs_compconj(COMPLEX a);
LINK COMPLEX rcs_compcos(COMPLEX a);
LINK COMPLEX rcs_compcosec(COMPLEX a);
LINK COMPLEX rcs_compcotan(COMPLEX a);
LINK COMPLEX rcs_compdiv(COMPLEX a,COMPLEX b);
LINK COMPLEX rcs_compexp(COMPLEX a);
LINK COMPLEX rcs_complex(double r,double i);
LINK COMPLEX rcs_complog(COMPLEX a);
LINK COMPLEX rcs_compmul(COMPLEX a,COMPLEX b);
LINK COMPLEX rcs_compneg(COMPLEX a);
LINK COMPLEX rcs_comppow(COMPLEX a,COMPLEX b);
LINK COMPLEX rcs_compsec(COMPLEX a);
LINK COMPLEX rcs_compsin(COMPLEX a);
LINK COMPLEX rcs_compsqr(COMPLEX a);
LINK COMPLEX rcs_compsqrt(COMPLEX a);
LINK COMPLEX rcs_compsub(COMPLEX a,COMPLEX b);
LINK COMPLEX rcs_comptan(COMPLEX a);
  
/* **************** matrix **************** */
LINK MATRIX rcs_matident();
LINK MATRIX rcs_matmul(MATRIX a,MATRIX b);
LINK MATRIX rcs_matscale(double sx,double sy,double sz);
LINK MATRIX rcs_mattran(double tx,double ty,double tz);
LINK MATRIX rcs_matrot(int axis,double angle);
LINK MATRIX rcs_matzero();
LINK MATRIX rcs_transMatrix(double tx,double ty,double tz,
                            double sx,double sy,double sz,
                            double rx,double ry,double rz);
LINK MATRIX rcs_transInvMatrix(double tx,double ty,double tz,
                            double sx,double sy,double sz,
                            double rx,double ry,double rz);
LINK VECTOR rcs_transform(VECTOR a,MATRIX m);

/* **************** vector **************** */
LINK IVECTOR rcs_ivector(int x,int y,int z);
LINK IVECTOR rcs_ivecsub(IVECTOR a,IVECTOR b);
LINK IVECTOR rcs_ivecscale(IVECTOR a,int b);
LINK IVECTOR rcs_iveczero();
LINK void    rcs_unvec(VECTOR v,double* x,double* y,double* z);
LINK void    rcs_univec(IVECTOR v,int* x,int* y,int* z);
LINK VECTOR rcs_vector(double x,double y,double z);
LINK void   rcs_vecadd(VECTOR a,VECTOR b,VECTOR* c);
LINK void   rcs_vecadd3(VECTOR a,VECTOR b,VECTOR c,VECTOR* d);
LINK void   rcs_veccrs(VECTOR a,VECTOR b,VECTOR* c);
LINK double rcs_vecdot(VECTOR a,VECTOR b);
LINK void   rcs_veccpy(VECTOR a,VECTOR* b);
LINK double rcs_veclen(VECTOR a);
LINK void   rcs_veclin(VECTOR a,double sa,VECTOR b,double sb,VECTOR* dv);
LINK void   rcs_vecmin(VECTOR a,VECTOR b,VECTOR* c);
LINK void   rcs_vecmax(VECTOR a,VECTOR b,VECTOR* c);
LINK VECTOR rcs_vecmulmat(VECTOR v,MATRIX m);
LINK void   rcs_vecneg(VECTOR a,VECTOR* b);
LINK void   rcs_vecnrm(VECTOR a,VECTOR* b);
LINK void   rcs_vecsadd(VECTOR a,double sa,VECTOR b,VECTOR* dv);
LINK void   rcs_vecscale(VECTOR a,double scale,VECTOR* b);
LINK void   rcs_vecsmul(VECTOR a,double sa,VECTOR b,VECTOR* c);
LINK void   rcs_vecsub(VECTOR a,VECTOR b,VECTOR* c);
LINK void   rcs_veczero(VECTOR* a);
LINK double rcs_vecelem(VECTOR v,int e);

/* ********* graph3d ********** */
LINK int     rcs_addobject(WORLD* world,GOBJECT obj);
LINK int     rcs_raddobject(WORLD* world,GOBJECT obj);
LINK int     rcs_addpoint(POLYGON* poly,int point);
LINK int     rcs_addpoly(GOBJECT* obj,POLYGON poly);
LINK int     rcs_addvertex(GOBJECT* obj,VECTOR vertex);
LINK void    rcs_clrobject(GOBJECT* obj);
LINK void    rcs_droppoly(POLYGON poly);
LINK GOBJECT rcs_gobject(int maxVertex,int maxPoly);
LINK void    rcs_objecttrans(GOBJECT* obj,MATRIX m);
LINK POLYGON rcs_polygon(int maxPoints);
LINK void    rcs_prepobj(GOBJECT* obj,int cx,int cy);
LINK void    rcs_prepworld(WORLD* world,int cx,int cy);
LINK void    rcs_rprepworld(WORLD* world,int cx,int cy);
LINK MATRIX  rcs_tranmat(double sx,double sy,double sz,
                           double rx,double ry,double rz,
                           double tx,double ty,double tz);
LINK VECTOR  rcs_vectrans(VECTOR v,MATRIX m);
LINK void    rcs_wireobj(rcs_Display* disp,rcs_Window win,rcs_GC gc,GOBJECT* obj);
LINK void    rcs_wireworld(rcs_Display* disp,rcs_Window win,rcs_GC gc,WORLD *world);
LINK WORLD   rcs_world(int maxObjects);
LINK void    rcs_worldtrans(WORLD* world,MATRIX m);

/* ********* graph ********** */
LINK int rcs_clip(int* x1,int* y1,int* x2,int* y2,int lx,int ly,int hx,int hy);
  
/* ********* awlib ********** */
LINK void        rcs_flush(rcs_Display* d);
LINK void        rcs_getDisplaySize(rcs_Display* d,int* width,int* height);
LINK void        rcs_getWindowSize(rcs_Display* d,rcs_Window w,int* width,int* height);
LINK void        rcs_getWindowPos(rcs_Display* d,rcs_Window w,int* x,int* y);
LINK void        rcs_background(rcs_Display* d,rcs_GC gc,
                                  unsigned long red,
                                  unsigned long green,
                                  unsigned long blue);
LINK void        rcs_closeDisplay(rcs_Display* display);
LINK void        rcs_closeGC(rcs_Display* d,rcs_GC gc);
LINK void        rcs_closeWindow(rcs_Display* d,rcs_Window win);
LINK void        rcs_copyArea(rcs_Display*,rcs_Window,rcs_Window,rcs_GC,
                                int sx,int sy,int w,int h,int dx,int dy);
LINK rcs_Pixmap  rcs_createPixmap(rcs_Display*,rcs_Window,int,int);
LINK rcs_Window  rcs_createWindow(rcs_Display* d,rcs_Window parent,
                                    int x,int y,int width,int height);
LINK void        rcs_drawBox(rcs_Display*,rcs_Window win,rcs_GC gc,
                                int x,int y,int w,int h);
LINK void        rcs_drawCircle(rcs_Display* d,rcs_Window win,rcs_GC gc,
                                int x,int y,int w,int h);
LINK void        rcs_drawArc(rcs_Display* d,rcs_Window win,rcs_GC gc,
                                int x,int y,int w,int h,int a1,int a2);
LINK void        rcs_drawFilledBox(rcs_Display*,rcs_Window win,rcs_GC gc,
                                int x,int y,int w,int h);
LINK void        rcs_drawFilledCircle(rcs_Display* d,rcs_Window win,rcs_GC gc,
                                int x,int y,int w,int h);
LINK void        rcs_drawLine(rcs_Display*,rcs_Window win,rcs_GC gc,
                                int x1,int y1,int x2,int y2);
LINK void        rcs_drawPixmap(rcs_Display* d,rcs_Window win,rcs_GC gc,int dx,int dy,const char** data);
LINK void        rcs_drawSubPixmap(rcs_Display* d,rcs_Window win,rcs_GC gc,int dx,int dy,char** data,int sx,int sy,int w,int h);
LINK void        rcs_drawPoint(rcs_Display*,rcs_Window win,rcs_GC gc,
                                 int x,int y);
LINK void        rcs_drawString(rcs_Display* d,rcs_Window win,rcs_GC gc,int x,int y,
                    char* buffer);
LINK void        rcs_foreground(rcs_Display* d,rcs_GC gc,
                                  unsigned long red,
                                  unsigned long green,
                                  unsigned long blue);
LINK void        rcs_closeFont(rcs_Display*,rcs_Font);
LINK void        rcs_closePixmap(rcs_Display*,rcs_Pixmap);
LINK rcs_Event   rcs_getEvent(rcs_Display*);
LINK void        rcs_hideWindow(rcs_Display* d,rcs_Window win);
LINK void        rcs_lowerWindow(rcs_Display* d,rcs_Window win);
LINK void        rcs_moveWindow(rcs_Display* d,rcs_Window w,int x,int y);
LINK void        rcs_namedBackground(rcs_Display* d,rcs_GC gc,const char* color);
LINK void        rcs_namedForeground(rcs_Display* d,rcs_GC gc,const char* color);
LINK rcs_Display *rcs_openDisplay(char* display_name);
LINK rcs_Font    rcs_openFont(rcs_Display*,char*);
LINK rcs_GC      rcs_openGC(rcs_Display* d,rcs_Window win);
LINK rcs_Event   rcs_pollEvent(rcs_Display*);
LINK void        rcs_raiseWindow(rcs_Display* d,rcs_Window win);
LINK void        rcs_resizeWindow(rcs_Display* d,rcs_Window win,int w,int h);
LINK rcs_Window  rcs_rootWindow(rcs_Display* d);
LINK void        rcs_setFont(rcs_Display*,rcs_GC,rcs_Font);
LINK void        rcs_setWindowName(rcs_Display*,rcs_Window,char*);
LINK void        rcs_setWindowBackground(rcs_Display* d,rcs_Window w,const char* color);
LINK void        rcs_setWindowBackgroundRGB(rcs_Display* d,rcs_Window w,
                 int r,int g,int b);
LINK void        rcs_showWindow(rcs_Display* d,rcs_Window win);
LINK rcs_Pixmap  rcs_xpmToPixmap(rcs_Display* d,rcs_Window win,const char** data);

/* *********** mstring ************ */
LINK char*  rcs_trimRight(char* ptr);
LINK char*  rcs_trimLeft(char* ptr);
LINK char*  rcs_trim(char* ptr);
LINK char*  rcs_getWord(char* dest,char* src);
LINK char*  rcs_trimLineEndings(char* ptr);
LINK char*  rcs_nextWord(char*);
LINK int    rcs_loadStrings(char*,char***);
                      
/* *********** mterm *************** */
LINK void   rcs_output(const char* Buffer);
LINK void   rcs_input(char* Buffer);
LINK int    rcs_inkey();
LINK void   rcs_clrScr();
LINK void   rcs_delay(int sec);
LINK void   rcs_gotoXY(int x,int y);
#ifdef UNIX
LINK int    rcs_getTerm(rcs_Termio*);
LINK int    rcs_setTerm(rcs_Termio*);
LINK int    rcs_echoOn(rcs_Termio*);
LINK int    rcs_echoOff(rcs_Termio*);
LINK int    rcs_buffered(rcs_Termio*);
LINK int    rcs_unBuffered(rcs_Termio*);
#endif

/* ************ vmem **************** */
LINK VMEM   rcs_newVmem(char* filename,long size);
LINK VMEM   rcs_openVmem(char* filename);
LINK long   rcs_sizeVmem(VMEM* vmem);
LINK int    rcs_closeVmem(VMEM* vmem);
LINK size_t rcs_readVmem(VMEM* vmem,void* buffer,long address,size_t length);
LINK size_t rcs_writeVmem(VMEM* vmem,void* buffer,long address,size_t length);

/* ************ xml ***************** */
LINK XML_NODE* rcs_xmlNew(char* n);
LINK int       rcs_xmlAddNode(XML_NODE* node,char* path,char** symbols);

/* ************ kernel ************** */
LINK int    rcs_chdir(const char* path);
LINK int    rcs_chown(const char* path,uid_t owner,gid_t group);
LINK int    rcs_close(int filedes);
LINK int    rcs_chroot(const char* path);
LINK int    rcs_dup(int filedes);
LINK int    rcs_fchown(int filedes,uid_t owner,gid_t group);
LINK int    rcs_fchroot(int filedes);
LINK uid_t  rcs_getegid(void);
LINK uid_t  rcs_geteuid(void);
LINK void   rcs_getFullName(char* buffer);
LINK void   rcs_getHomeDir(char* buffer);
LINK uid_t  rcs_getgid(void);
LINK uid_t  rcs_getuid(void);
LINK int    rcs_kill(pid_t pid,int sig);
LINK int    rcs_lchown(const char* path,uid_t owner,gid_t group);
LINK int    rcs_link(const char* existing,const char* nw);
LINK off_t  rcs_lseek(int filedes,off_t offset,int whence);
LINK int    rcs_nice(int incr);
LINK int    rcs_open(const char* path,int oflag,mode_t mode);
LINK size_t rcs_pread(int filedes,void* buf,size_t nbyte,off_t offset);
LINK size_t rcs_pwrite(int filedes,void* buf,size_t nbyte,off_t offset);
LINK size_t rcs_read(int filedes,void* buf,size_t nbyte);
LINK int    rcs_rename(const char* old,const char* nw);
LINK int    rcs_rmdir(const char* path);
LINK int    rcs_setegid(gid_t gid);
LINK int    rcs_seteuid(uid_t uid);
LINK int    rcs_setgid(gid_t gid);
LINK int    rcs_setuid(uid_t uid);
LINK void   rcs_sync(void);
LINK time_t rcs_time();
LINK mode_t rcs_umask(mode_t cmask);
LINK int    rcs_unlink(const char* path);
LINK size_t rcs_write(int filedes,void* buf,size_t nbyte);

/* ************************ file.c ******************** */
LINK int    rcs_exist(char* filename);
LINK long   rcs_numRecords(int fdes,int reclen);
LINK void   rcs_shiftFile(long offset,long shift,int fdes);

/* ************************ options.c ******************** */
LINK int rcs_getOption(char* option,int argc,char* argv[]);
LINK int rcs_getNonOption(int start,int argc,char* argv[]);

/* ************************ date.c ******************** */
LINK long   rcs_daysInYear(long year);
LINK long   rcs_daysInMonth(long year,long month);
LINK rcs_dt rcs_daysToDate(long t,rcs_dt baseDate);
LINK long   rcs_dateToDays(rcs_dt dt,rcs_dt base);
LINK void   rcs_daysToString(long t,char* buffer,rcs_dt base);
LINK double rcs_sidereal(double mo,double da,double yr,
                         double hr,double mi,double sc,
                         double lng,double* hrRet,double* miRet,double* scRet);
LINK long rcsgl_julianDay(int m,int d,int y);
LINK long   rcs_stringToDays(char* date,rcs_dt base);

/* ************************ db.c ******************** */
LINK int rcs_compareKey(int keyfile,long keylen,long pos,rcs_key sKey,char typ);
LINK long rcs_findKey(int keyfile,long keylen,rcs_key sKey,char typ);
LINK void rcs_deleteKey(int fdes,long recnum,long keylen);
LINK void rcs_insertKey(int fdes,rcs_key key,long keylen,char typ);
LINK void rcs_adjustKeys(int fdes,long record,long keylen);

/* ************************ rgb.c ******************** */
LINK int rcs_namedColor(char* clr,int *red,int *green,int *blue);

#ifdef __cplusplus
}
#endif

/* ************************ xhelpers.c ******************* */
#ifdef X
extern char* CheckArgs(int argc, char** argv, char* look_for);

extern Window CreateWindow(Display* display,
	Window parent,
	int x,
	int y,
	unsigned int width,
	unsigned int height,
	unsigned int border,
	unsigned long fore,
	unsigned long back,
	unsigned long event_mask);

extern void CheckGeometry(Display* display,
	int screen_number,
	int argc,
	char** argv,
	int* x,
	int* y,
	unsigned int* width,
	unsigned int* height);

extern void SetWMHints(Display* display,
	Window window,
	int x,
	int y,
	unsigned int width,
	unsigned int height,
	int argc,
	char** argv,
	char* window_name,
	char* icon_name,
	char* class_name,
	char* class_type);

extern Bool FilterEvent(XEvent* event,Bool want_exposes,int width, int height);
#endif

/* ************************ libapp ******************* */

#ifndef WIN32
#undef LINK
#define LINK
#endif

#ifdef __cplusplus

class rcs_CObject {
  protected:
  public:
    LINK rcs_CObject();
    LINK ~rcs_CObject();
  };
 
class rcs_CEvent :rcs_CObject {
  protected:
    rcs_Event event;
    rcs_CObject* subject;
  public:
    LINK rcs_CEvent(rcs_CObject* s,rcs_Event e);
    LINK virtual ~rcs_CEvent();
    LINK virtual rcs_Event getEvent();
    LINK virtual rcs_CObject* getSubject();
  };

class rcs_Listener : public rcs_CObject {
  protected:
  public:
    rcs_Listener();
    virtual ~rcs_Listener();
  };

class rcs_MouseListener : public rcs_Listener {
  protected:
  public:
    LINK rcs_MouseListener();
    LINK virtual ~rcs_MouseListener();
    LINK virtual void mouseDown(rcs_CEvent* e);
    LINK virtual void mouseUp(rcs_CEvent* e);
    LINK virtual void mouseMoved(rcs_CEvent* e);
    LINK virtual void focusIn(rcs_CEvent* e);
    LINK virtual void focusOut(rcs_CEvent* e);
    LINK virtual void enterWindow(rcs_CEvent* e);
    LINK virtual void leaveWindow(rcs_CEvent* e);
  };

class rcs_KeyListener : public rcs_Listener {
  protected:
  public:
    LINK rcs_KeyListener();
    LINK virtual ~rcs_KeyListener();
    LINK virtual void keyDown(rcs_CEvent* e);
    LINK virtual void keyUp(rcs_CEvent* e);
  };

class rcs_CComponent : public rcs_CObject {
  protected:
    int        width;
    int        height;
    int        x;
    int        y;
    rcs_Display*       display;
    rcs_Window         window;
    rcs_KeyListener*   keyListener;
    rcs_MouseListener* mouseListener;
    rcs_CComponent*    parent;
  public:
    LINK rcs_CComponent(rcs_Display* d,rcs_CComponent* p,int w,int h,int wx,int wy);
    LINK virtual ~rcs_CComponent();
    LINK virtual int getHeight();
    LINK virtual int getWidth();
    LINK virtual int getX();
    LINK virtual int getY();
    LINK virtual void setHeight(int h);
    LINK virtual void setWidth(int w);
    LINK virtual void setSize(int w,int h);
    LINK virtual void setX(int v);
    LINK virtual void setY(int v);
    LINK virtual void setPosition(int nx,int ny);
    LINK virtual void setKeyListener(rcs_KeyListener* l);
    LINK virtual void setMouseListener(rcs_MouseListener* l);
    LINK virtual rcs_Window getWindow();
    LINK virtual void event(rcs_CEvent* e);
    LINK virtual int  processEvent(rcs_Event e,int p);
    LINK virtual void raise();
    LINK virtual void lower();
    LINK virtual void show();
    LINK virtual void hide();
    LINK virtual void buttonDown(rcs_CEvent* e);
    LINK virtual void buttonUp(rcs_CEvent* e);
    LINK virtual void keyDown(rcs_CEvent* e);
    LINK virtual void keyUp(rcs_CEvent* e);
    LINK virtual void mouseMoved(rcs_CEvent* e);
    LINK virtual void gotFocus(rcs_CEvent* e);
    LINK virtual void lostFocus(rcs_CEvent* e);
    LINK virtual void windowEntered(rcs_CEvent* e);
    LINK virtual void windowLeft(rcs_CEvent* e);
    LINK virtual void paint(rcs_CEvent* e);
    LINK virtual void configure(rcs_CEvent* e);
    LINK virtual rcs_KeyListener* getKeyListener();
    LINK virtual rcs_MouseListener* getMouseListener();
    LINK virtual void setBackground(char* color);
    LINK virtual void setName(const char* name);
    LINK virtual void expose(rcs_CEvent* e);
  };

class rcs_CContainer : public rcs_CComponent {
  protected:
    rcs_CObject** objects;
    int           numObjects;
    int           pos;
  public:
    LINK rcs_CContainer(rcs_Display* d,rcs_CComponent* p,int w,int h,int wx,int wy);
    LINK virtual ~rcs_CContainer();
    LINK virtual int getNumObjects();
    LINK virtual void add(rcs_CObject* object);
    LINK virtual void remove(rcs_CObject* object);
    LINK virtual rcs_CObject* first();
    LINK virtual rcs_CObject* next();
    LINK virtual rcs_CObject* nth(int p);
    LINK virtual int includes(rcs_CObject* object);
  };

class rcs_CButton : public rcs_CComponent {
  protected:
    char* text;
    byte  state;
  public:
    LINK rcs_CButton(rcs_Display* d,rcs_CComponent* p,int w,int h,int wx,int wy,
                const char* t);
    LINK virtual ~rcs_CButton();
    LINK virtual void setText(const char* t);
    LINK virtual void paintUp();
    LINK virtual void paintDown();
    LINK virtual void paint(rcs_CEvent* e);
    LINK virtual void buttonDown(rcs_CEvent* e);
    LINK virtual void buttonUp(rcs_CEvent* e);
    LINK virtual byte getState();
  };

class rcs_CCheckBox : public rcs_CButton {
  protected:
  public:
    LINK rcs_CCheckBox(rcs_Display* d,rcs_CComponent* p,int w,int h,int wx,int wy,
                char* t);
    LINK virtual ~rcs_CCheckBox();
    LINK virtual void setText(char* t);
    LINK virtual void paintUp();
    LINK virtual void paintDown();
    LINK virtual void buttonDown(rcs_CEvent* e);
    LINK virtual void buttonUp(rcs_CEvent* e);
  };

class rcs_CRadioButton : public rcs_CCheckBox {
  protected:
  public:
    LINK rcs_CRadioButton(rcs_Display* d,rcs_CComponent* p,int w,int h,int wx,int wy,
                char* t);
    LINK virtual ~rcs_CRadioButton();
    LINK virtual void paintUp();
    LINK virtual void paintDown();
    LINK virtual void setState(byte s);
  };

class rcs_CLabel : public rcs_CComponent {
  protected:
    char* text;
  public:
    LINK rcs_CLabel(rcs_Display* d,rcs_CComponent* p,int w,int h,int wx,int wy,
                char* t);
    LINK virtual ~rcs_CLabel();
    LINK virtual void paint(rcs_CEvent* e);
  };

class rcs_CToggle : public rcs_CButton {
  protected:
  public:
    LINK rcs_CToggle(rcs_Display* d,rcs_CComponent* p,int w,int h,int wx,int wy,
                const char* t);
    LINK virtual ~rcs_CToggle();
    LINK virtual void buttonDown(rcs_CEvent* e);
    LINK virtual void buttonUp(rcs_CEvent* e);
  };

class rcs_CWindow : public rcs_CContainer {
  protected:
    char doExit;
  public:
    LINK rcs_CWindow(rcs_Display* d,rcs_CComponent* p,int w,int h,int wx,int wy);
    LINK virtual ~rcs_CWindow();
    LINK virtual int  processEvent(rcs_Event e,int p);
    LINK virtual void run();
    LINK virtual void cancel();
  };
 
class rcs_CGroup : public rcs_CContainer {
  protected:
    char* text;
  public:
    LINK rcs_CGroup(rcs_Display* d,rcs_CComponent* p,int w,int h,int wx,int wy,
                char* t);
    LINK virtual ~rcs_CGroup();
    LINK virtual void paint(rcs_CEvent* e);
    LINK virtual int  processEvent(rcs_Event e,int p);
  };

class rcs_CScrollBar : public rcs_CComponent {
  protected:
    int minimum;
    int maximum;
    int position;
    int maxPosition;
    int graphStart;
    int graphEnd;
    int puckSize;
    int puckGrabbed;
    int grabPosition;
    rcs_Pixmap       back;
    virtual void setPuckData(int min,int max,int size);
  public:
    LINK rcs_CScrollBar(rcs_Display* d,rcs_CComponent* p,int w,int h,int wx,int wy,
                int min,int max);
    LINK virtual ~rcs_CScrollBar();
    LINK virtual void setMinimum(int min);
    LINK virtual void setMaximum(int max);
    LINK virtual void setPosition(int pos);
    LINK virtual int  getMinimum();
    LINK virtual int  getMaximum();
    LINK virtual int  getPosition();
    LINK virtual void paint(rcs_CEvent* e);
    LINK virtual int  getValue();
    LINK virtual void setValue(int v);
    };

class rcs_CVScrollBar : public rcs_CScrollBar {
  protected:
    void drawPuck();
    void erasePuck();
  public:
    LINK rcs_CVScrollBar(rcs_Display* d,rcs_CComponent* p,int w,int h,int wx,int wy,
                int min,int max);
    LINK virtual ~rcs_CVScrollBar();
    LINK virtual void paint(rcs_CEvent* e);
    LINK virtual void buttonDown(rcs_CEvent* e);
    LINK virtual void buttonUp(rcs_CEvent* e);
    LINK virtual void mouseMoved(rcs_CEvent* e);
    LINK virtual void setMinimum(int min);
    LINK virtual void setMaximum(int max);
    LINK virtual void setHeight(int h);
    LINK virtual void setWidth(int w);
    LINK virtual void setSize(int w,int h);
  };

class rcs_CHScrollBar : public rcs_CScrollBar {
  protected:
    void drawPuck();
    void erasePuck();
  public:
    LINK rcs_CHScrollBar(rcs_Display* d,rcs_CComponent* p,int w,int h,int wx,int wy,
                int min,int max);
    LINK virtual ~rcs_CHScrollBar();
    LINK virtual void paint(rcs_CEvent* e);
    LINK virtual void buttonDown(rcs_CEvent* e);
    LINK virtual void buttonUp(rcs_CEvent* e);
    LINK virtual void mouseMoved(rcs_CEvent* e);
    LINK virtual void setMinimum(int min);
    LINK virtual void setMaximum(int max);
    LINK virtual void setHeight(int h);
    LINK virtual void setWidth(int w);
    LINK virtual void setSize(int w,int h);
  };

class rcs_CDialog : public rcs_CWindow, public rcs_MouseListener {
  protected:
    int          ret;
    rcs_CButton* ok;
    rcs_CButton* cancel;
  public:
    LINK rcs_CDialog(rcs_Display* d,rcs_CComponent* p,int w,int h,int wx,int wy);
    LINK virtual ~rcs_CDialog();
    LINK virtual int  run(rcs_CComponent* p,char modal);
    LINK virtual void mouseUp(rcs_CEvent* e);
    LINK virtual void okPressed();
    LINK virtual void cancelPressed();
  };

class rcs_CListBox : public rcs_CWindow, public rcs_MouseListener {
  protected:
    char** items;
    int    numItems;
    int*   itemsSelected;
    int    topItem;
    int    displayItems;
    int    itemPos;
    int    lastChanged;
    char   allowMultiple;
    rcs_Pixmap back;
    rcs_CVScrollBar* sb;
  public:
    LINK rcs_CListBox(rcs_Display* d,rcs_CComponent* p,int w,int h,int wx,int wy);
    LINK virtual ~rcs_CListBox();
    LINK virtual void addItem(char* item);
    LINK virtual void paint(rcs_CEvent* e);
    LINK virtual int  getNumItems();
    LINK virtual int  getFirstSelected();
    LINK virtual int  getNextSelected();
    LINK virtual int  getItemState(int i);
    LINK virtual char* getItemText(int i);
    LINK virtual int  getLastChanged();
    LINK virtual void select(int item);
    LINK virtual void unselect(int item);
    LINK virtual void multiplesAllowed(char flag);
    LINK virtual void buttonDown(rcs_CEvent* e);
    LINK virtual void mouseDown(rcs_CEvent* e);
    LINK virtual void mouseUp(rcs_CEvent* e);
    LINK virtual void mouseMoved(rcs_CEvent* e);
    LINK virtual void sort();
    LINK virtual void clear();
    LINK virtual void setSize(int w,int h);
  };

class rcs_CDropDown : public rcs_CWindow, rcs_MouseListener {
  protected:
    int selected;
    int droppedDown;
    int fullHeight;
    rcs_CListBox* lb;
  public:
    LINK rcs_CDropDown(rcs_Display* d,rcs_CComponent* p,int w,int h,int wx,int wy);
    LINK virtual ~rcs_CDropDown();
    LINK virtual void select(int item);
    LINK virtual void addItem(char* item);
    LINK virtual char* getSelectedText();
    LINK virtual void paint(rcs_CEvent* e);
    LINK virtual void buttonDown(rcs_CEvent* e);
    LINK virtual void mouseDown(rcs_CEvent* e);
    LINK virtual void mouseUp(rcs_CEvent* e);
    LINK virtual void mouseMoved(rcs_CEvent* e);
  };

class rcs_CTextBox : public rcs_CWindow, rcs_MouseListener,
                     rcs_KeyListener {
  protected:
    int  row;
    int  line;
    int  cx;
    int  cy;
    int  numLines;
    int  topLine;
    int  lineStart;
    int  lineWidth;
    char showCursor;
    char** text;
    char multiLine;
    rcs_Pixmap       back;
    rcs_CVScrollBar* sb;
    virtual void paintText();
  public:
    LINK rcs_CTextBox(rcs_Display* d,rcs_CComponent* p,int w,int h,int wx,int wy,
                 char multiLine);
    LINK virtual ~rcs_CTextBox();
    LINK virtual char* getText();
    LINK virtual char* getSelectedText();
    LINK virtual void setText(char* txt);
    LINK virtual void clear();
    LINK virtual void paint(rcs_CEvent* e);
    LINK virtual void buttonDown(rcs_CEvent* e);
    LINK virtual void mouseDown(rcs_CEvent* e);
    LINK virtual void mouseUp(rcs_CEvent* e);
    LINK virtual void mouseMoved(rcs_CEvent* e);
    LINK virtual void windowEntered(rcs_CEvent* e);
    LINK virtual void windowLeft(rcs_CEvent* e);
    LINK virtual void keyDown(rcs_CEvent* e);
    LINK virtual void keyUp(rcs_CEvent* e);
    LINK virtual void expose(rcs_CEvent* e);
  };

class rcs_CImageButton : public rcs_CButton {
  protected:
    rcs_Pixmap       back;
  public:
    LINK rcs_CImageButton(rcs_Display* d,rcs_CComponent* p,int w,int h,int wx,int wy,
                const char** img);
    LINK virtual ~rcs_CImageButton();
    LINK virtual void setImage(const char** img);
    LINK virtual void paintUp();
    LINK virtual void paintDown();
    LINK virtual void paint(rcs_CEvent* e);
  };

class rcs_CImageToggle : public rcs_CToggle {
  protected:
    rcs_Pixmap       back;
  public:
    LINK rcs_CImageToggle(rcs_Display* d,rcs_CComponent* p,int w,int h,int wx,int wy,
                const char** img);
    LINK virtual ~rcs_CImageToggle();
    LINK virtual void setImage(const char** img);
    LINK virtual void paintUp();
    LINK virtual void paintDown();
    LINK virtual void paint(rcs_CEvent* e);
  };

typedef struct _psdata {
  int        dataType;
  union {
    int      integer;
    double   real;
    char     boolean;
    int      token;
    char*    string;
    _psdata* array;
    _psdata* dict;
    } data;
  char**     name;
  int count;
  } PSDATA;

typedef struct {
  char          command;
  double        x;
  double        y;
  } PSPATHCMD;

typedef struct {
  PSPATHCMD    path[1500];
  int          numPath;
  PSPATHCMD    clip[1500];
  int          numClip;
  double       currentX;
  double       currentY;
  double       lineWidth;
  int          lineCap;
  int          lineJoin;
  double       miterLimit;
  double       red;
  double       green;
  double       blue;
  double       transform[6];
  double       gwidth;
  double       gheight;
  double       gllx;
  double       glly;
  double       gurx;
  double       gury;
  PSDATA       font;
  double       foffsx;
  double       foffsy;
  } PSGSTATE;

class rcs_CPostScript : public rcs_CObject {
  protected:
    PSDATA*          dicts;
    int              numDicts;
    PSDATA*          stack;
    int              stackPointer;
    PSDATA*          fonts;
    int              numFonts;
    int              braceCount;
    PSGSTATE*        gstack;
    int              numGstack;
    PSGSTATE         gstate;
    rcs_Display*     display;
    rcs_Pixmap       image;
    int              imageHeight;
    int              imageWidth;
    int              nextFontID;
    virtual void   push(PSDATA data);
    virtual PSDATA pop();
    virtual void    addDictionary();
    virtual PSDATA  copy(PSDATA obj);
    virtual void    dispose(PSDATA data);
    virtual void    addItem(const char* name,PSDATA data);
    virtual void    addToDict(const char* name,PSDATA data,PSDATA* dict);
    virtual void    outputData(PSDATA data);
    virtual PSDATA  searchDictionary(PSDATA* dict,const char* cmd);
    virtual PSDATA  resolve(char* name);
    virtual int     execute(PSDATA arg);
    virtual int     executeCommands(PSDATA* commands);
    virtual void    concat(double* m1,double* m2);
    virtual int     get1Arg(PSDATA* arg1,int type1);
    virtual int     get2Args(PSDATA* arg1,PSDATA* arg2,int type1,int type2);
    virtual int     get3Args(PSDATA* a1,PSDATA* a2,PSDATA* a3,int t1,int t2,int t3);
    virtual int     get4Args(PSDATA* a1,PSDATA* a2,PSDATA* a3,PSDATA* a4,int t1,int t2,int t3,int t4);
    virtual void    bind(PSDATA* d);
    virtual int     cmdAbs();
    virtual int     cmdAdd();
    virtual int     cmdAnd();
    virtual int     cmdAload();
    virtual int     cmdAnchorSearch();
    virtual int     cmdArray();
    virtual int     cmdAshow();
    virtual int     cmdAstore();
    virtual int     cmdBegin();
    virtual int     cmdBind();
    virtual int     cmdBitshift();
    virtual int     cmdCeiling();
    virtual int     cmdClear();
    virtual int     cmdClearToMark();
    virtual int     cmdClip();
    virtual int     cmdCloseBR();
    virtual int     cmdCloseSB();
    virtual int     cmdClosePath();
    virtual int     cmdConcat();
    virtual int     cmdCopy();
    virtual int     cmdCos();
    virtual int     cmdCount();
    virtual int     cmdCountDictStack();
    virtual int     cmdCountToMark();
    virtual int     cmdCurrentColor();
    virtual int     cmdCurrentDict();
    virtual int     cmdCurrentGray();
    virtual int     cmdCurrentLineCap();
    virtual int     cmdCurrentLineJoin();
    virtual int     cmdCurrentLineWidth();
    virtual int     cmdCurrentMatrix();
    virtual int     cmdCurrentMiterLimit();
    virtual int     cmdCurrentPoint();
    virtual int     cmdCvi();
    virtual int     cmdCvr();
    virtual int     cmdDef();
    virtual int     cmdDefaultMatrix();
    virtual int     cmdDefineFont();
    virtual int     cmdDict();
    virtual int     cmdDiv();
    virtual int     cmdDtransform();
    virtual int     cmdDup();
    virtual int     cmdEnd();
    virtual int     cmdEq();
    virtual int     cmdEqEq();
    virtual int     cmdErasePage();
    virtual int     cmdExch();
    virtual int     cmdExec();
    virtual int     cmdExit();
    virtual int     cmdExp();
    virtual int     cmdFalse();
    virtual int     cmdFill();
    virtual int     cmdFindFont();
    virtual int     cmdFloor();
    virtual int     cmdFor();
    virtual int     cmdForAll();
    virtual int     cmdGe();
    virtual int     cmdGet();
    virtual int     cmdGrestore();
    virtual int     cmdGsave();
    virtual int     cmdGt();
    virtual int     cmdIf();
    virtual int     cmdIfElse();
    virtual int     cmdIdiv();
    virtual int     cmdIndex();
    virtual int     cmdInitClip();
    virtual int     cmdInitGraphics();
    virtual int     cmdInitMatrix();
    virtual int     cmdInvertMatrix();
    virtual int     cmdItransform();
    virtual int     cmdKnown();
    virtual int     cmdLe();
    virtual int     cmdLength();
    virtual int     cmdLineTo();
    virtual int     cmdLn();
    virtual int     cmdLoad();
    virtual int     cmdLog();
    virtual int     cmdLoop();
    virtual int     cmdLt();
    virtual int     cmdMark();
    virtual int     cmdMatrix();
    virtual int     cmdMaxLength();
    virtual int     cmdMod();
    virtual int     cmdMoveTo();
    virtual int     cmdMul();
    virtual int     cmdNe();
    virtual int     cmdNeg();
    virtual int     cmdNewPath();
    virtual int     cmdNot();
    virtual int     cmdNull();
    virtual int     cmdOpenBR();
    virtual int     cmdOr();
    virtual int     cmdPop();
    virtual int     cmdPstack();
    virtual int     cmdPut();
    virtual int     cmdRepeat();
    virtual int     cmdRLineTo();
    virtual int     cmdRMoveTo();
    virtual int     cmdRoll();
    virtual int     cmdRotate();
    virtual int     cmdScale();
    virtual int     cmdScaleFont();
    virtual int     cmdSearch();
    virtual int     cmdSetCacheDevice();
    virtual int     cmdSetFont();
    virtual int     cmdSetGray();
    virtual int     cmdSetLineCap();
    virtual int     cmdSetLineJoin();
    virtual int     cmdSetLineWidth();
    virtual int     cmdSetMatrix();
    virtual int     cmdSetMiterLimit();
    virtual int     cmdSetRGBColor();
    virtual int     cmdShow();
    virtual int     cmdSin();
    virtual int     cmdSqrt();
    virtual int     cmdString();
    virtual int     cmdStroke();
    virtual int     cmdSub();
    virtual int     cmdTransform();
    virtual int     cmdTranslate();
    virtual int     cmdTrue();
    virtual int     cmdXor();
    virtual PSDATA  buildItem(char* item);
    virtual PSDATA* tokenize(char* cmd);
    virtual void    transform(double* x,double* y,double* mat);
    virtual void    dtransform(double* x,double* y,double* mat);
    virtual void    invert(double* in,double* out);
    virtual void    drawLine(rcs_GC gc,double x1,double y1,double x2,double y2);
    virtual void    drawPoint(rcs_GC gc,double x1,double y1);
    virtual int     show();
    virtual int     checkUsed(void* mem,PSDATA* obj);
    virtual int     intercept(double x1,double y1,double x2,double y2,
                              double sx1,double sy1,double sx2,double sy2);
    virtual int     inside(int x,int y,int ex,int ey,PSPATHCMD* path,int size);
  public:
    LINK rcs_CPostScript(rcs_Display* d,rcs_Pixmap canvas,int w,int h);
    LINK virtual ~rcs_CPostScript();
    LINK virtual int command(char* cmd);
  };

class rcs_CCanvas : public rcs_CObject {
  protected:
    int              width;
    int              height;
    int              cursorX;
    int              cursorY;
    word             red;
    word             green;
    word             blue;
    rcs_Pixmap       canvas;
    rcs_Display*     display;
    rcs_CComponent*  parent;
    rcs_CPostScript* postscript;
    void             resizeCanvas();
    int              clip(int* x1,int* y1,int* x2,int* y2,int sx,int sy,int ex,
                          int ey);
  public:
    LINK rcs_CCanvas(rcs_Display* d,rcs_CComponent* p,int w,int h);
    LINK virtual ~rcs_CCanvas();
    LINK virtual int        getHeight();
    LINK virtual int        getWidth();
    LINK virtual void       setHeight(int h);
    LINK virtual void       setWidth(int w);
    LINK virtual void       setSize(int w,int h);
    LINK virtual rcs_Pixmap getCanvas();
    LINK virtual void       setColor(word r,word g,word b);
    LINK virtual void       point(int x,int y);
    LINK virtual void       point();
    LINK virtual void       moveTo(int x,int y);
    LINK virtual void       moveBy(int x,int y);
    LINK virtual void       lineTo(int x,int y);
    LINK virtual void       lineBy(int x,int y);
    LINK virtual void       box(int w,int h);
    LINK virtual void       image(rcs_Pixmap img,int w,int h);
    LINK virtual void       text(char* msg);
    LINK virtual int        ps(char* cmd);
  };

class rcs_CViewer : public rcs_CWindow, rcs_MouseListener {
  protected:
    int              viewWidth;
    int              viewHeight;
    rcs_CCanvas*     canvas;
    rcs_CVScrollBar* vs;
    rcs_CHScrollBar* hs;
  public:
    LINK rcs_CViewer(rcs_Display* d,rcs_CComponent* p,int w,int h,int wx,int wy);
    LINK rcs_CViewer(rcs_Display* d,rcs_CComponent* p,int w,int h,int wx,int wy,
                rcs_CCanvas* c);
    LINK virtual ~rcs_CViewer();
    LINK virtual void paint(rcs_CEvent* e);
    LINK virtual void buttonDown(rcs_CEvent* e);
    LINK virtual void mouseDown(rcs_CEvent* e);
    LINK virtual void mouseUp(rcs_CEvent* e);
    LINK virtual void mouseMoved(rcs_CEvent* e);
    LINK virtual rcs_CCanvas* getCanvas();
    LINK virtual void home();
  };

class rcs_CFileDialog : public rcs_CDialog {
  protected:
    int           ret;
    rcs_CListBox* lb;
    void loadFiles(char* path,char* mask);
  public:
    LINK rcs_CFileDialog(rcs_Display* d,rcs_CComponent* p,char* path,char* mask,int x,int y);
    LINK virtual ~rcs_CFileDialog();
    LINK virtual char* getFilename();
  };

class rcs_CFileSaveDialog : public rcs_CFileDialog {
  protected:
    int           ret;
    rcs_CTextBox* filename;
  public:
    LINK rcs_CFileSaveDialog(rcs_Display* d,rcs_CComponent* p,char* path,char* mask,int x,int y);
    LINK virtual ~rcs_CFileSaveDialog();
    LINK virtual char* getFilename();
    LINK virtual void mouseDown(rcs_CEvent* e);
  };

class rcs_CMessageBox : public rcs_CDialog {
  protected:
    rcs_CLabel* message;
  public:
    LINK rcs_CMessageBox(rcs_Display* d,rcs_CComponent* p,char* title,char* msg,int x,int y);
    LINK virtual ~rcs_CMessageBox();
  };

class rcs_CTextViewer : public rcs_CWindow, public rcs_MouseListener {
  protected:
    char** items;
    int    numItems;
    int    viewable;
    int    displayItems;
    int    topLine;
    rcs_Pixmap back;
    rcs_CVScrollBar* sb;
  public:
    LINK rcs_CTextViewer(rcs_Display* d,rcs_CComponent* p,int w,int h,int wx,int wy);
    LINK virtual ~rcs_CTextViewer();
    LINK virtual void addLine(char* item);
    LINK virtual void paint(rcs_CEvent* e);
    LINK virtual void mouseDown(rcs_CEvent* e);
    LINK virtual void mouseUp(rcs_CEvent* e);
    LINK virtual void mouseMoved(rcs_CEvent* e);
    LINK virtual void clear();
    LINK virtual void setSize(int w,int h);
  };

#endif

/* ************************ game ******************* */
 
#ifdef __cplusplus

#define PK_NONE      (unsigned long)0l<<28
#define PK_PAIR      (unsigned long)1l<<28
#define PK_2_PAIR    (unsigned long)2l<<28
#define PK_3_PAIR    (unsigned long)3l<<28
#define PK_THREE     (unsigned long)4l<<28
#define PK_STRAIGHT  (unsigned long)5l<<28
#define PK_FLUSH     (unsigned long)6l<<28
#define PK_FULLHOUSE (unsigned long)7l<<28
#define PK_3_THREE   (unsigned long)8l<<28
#define PK_FOUR      (unsigned long)9l<<28
#define PK_STFLUSH   (unsigned long)10l<<28
#define PK_FIVE      (unsigned long)11l<<28
#define PK_ROYALWLD  (unsigned long)12l<<28
#define PK_FOURWLD   (unsigned long)13l<<28
#define PK_ROYAL     (unsigned long)14l<<28
#define PK_FIVEWLD   (unsigned long)15l<<28

class Bet {
  protected:
    long value;
    long won;
    int  x,y;
    int  hand;
    void paintChips(rcs_Display* display,rcs_Window win,rcs_GC gc,int cx,int cy,
                 long amount);
  public:
LINK    Bet(long bet);
LINK    void clear();
LINK    void place(long b);
LINK    long getValue();
LINK    int  getX();
LINK    int  getY();
LINK    int  getHand();
LINK    void setPosition(int nx,int ny);
LINK    void putHand(int h);
LINK    void paint(rcs_Display* display,rcs_Window win);
LINK    long winHandBet(int h,float mult);
LINK    void loseHandBet(int h);
LINK    long winXYBet(int x1,int y1,int x2,int y2,float mult);
LINK    void loseXYBet(int x1,int y1,int x2,int y2);
LINK    void clearNonWinner();
LINK    void clearWonBet();
LINK    void display();
  };

class Card {
  protected:
    byte val;
    char suit;
    char side;
    int  x,y;
  public:
LINK    Card(byte v,char s);
LINK    char getValue();
LINK    char getSuit();
LINK    byte value();
LINK    byte lValue();
LINK    void setSide(char s);
LINK    char getSide();
LINK    void setPosition(int px,int py);
LINK    int  getX();
LINK    int  getY();
LINK    void paint(rcs_Display* display,rcs_Window win,rcs_GC gc);
};

class Deck {
  protected:
    Card** cards;
    int    numDecks;
    int    numCards;
    int    numJokers;
    int    cardPos;
    void   newDeck();
  public:
LINK    Deck(int decks,int jokers);
LINK    ~Deck();
LINK    void  shuffle();
LINK    Card* dealCard();
LINK    Card* dealSpanishCard();
LINK    int   cardsLeft();
LINK    void  removeByValue(int v);
  };

class Die {
  protected:
    int    sides;
    int    number;
    int    x,y;
    int    size;
  public:
LINK    Die(int s);
LINK    ~Die();
LINK    int roll();
LINK    int getNumber();
LINK    void putNumber(int n);
LINK    void paint(rcs_Display* display,rcs_Window win,rcs_GC gc);
LINK    void setPosition(int px,int py);
LINK    void setSize(int s);
  };

class Hand {
  protected:
    byte straightCount;
    byte straightHigh;
    byte stFlushCount;
    byte stFlushHigh;
    int  wildCount;
    unsigned long computeHandValue(unsigned long value);
    unsigned long computePaigowValue(unsigned long value);
    int  numbers[15];
    int  suits[4];
    byte combinations[10];
  public:
LINK    Hand();
LINK    Hand(int);
LINK    ~Hand();
    Card** cards;
    int  numCards;
LINK    void addCard(Card* c);
LINK    void sort(int wild);
LINK    int  bjValue();
LINK    int  bjCmp(Hand* h);
LINK    int  baccValue();
LINK    int  baccCmp(Hand* h);
LINK    unsigned long  pokerValue(int maxCards,int wild);
LINK    unsigned long  paigowValue();
LINK    Hand* paigowSplit(int c1,int c2);
LINK    int  pokerCmp(Hand* h,int wild);
LINK    int  threePokerValue(int wild);
LINK    int  threePokerCmp(Hand* h);
LINK    int  cribbageValue();
LINK    void display(int hidden);
LINK    void paint(rcs_Display* display,rcs_Window win,rcs_GC gc,int hidden);
LINK    void paint(rcs_Display* display,rcs_Window win,rcs_GC gc);
LINK    void deleteHand(Hand* h);
};

class MahjonggTile {
  protected:
    int  value;
    char suit;
    int  x,y;
    char side;
  public:
LINK    MahjonggTile(int v,char s);
LINK    int getValue();
LINK    char getSuit();
LINK    void setPosition(int px,int py);
LINK    void setSide(char s);
LINK    void paint(rcs_Display* d,rcs_Window w);
  };

class MahjonggDeck {
  protected:
    MahjonggTile* tiles[144];
    int   tilePos;
  public:
LINK    MahjonggDeck();
LINK    ~MahjonggDeck();
LINK    void shuffle();
LINK    MahjonggTile* dealTile();
  };

class MahjonggHand {
  protected:
    MahjonggTile* tiles[144];
    int   numTiles;
    int   x,y;
  public:
LINK    MahjonggHand();
LINK    void addTile(MahjonggTile*);
LINK    void setPosition(int px,int py);
LINK    void sort();
LINK    void paint(rcs_Display*,rcs_Window);
  };

class PaigowTile {
  protected:
    int top;
    int bottom;
    char dir;
    char side;
    int  x;
    int  y;
    void vNumber(rcs_Display*,rcs_Window,rcs_GC,int,int,int,int,int);
    void hNumber(rcs_Display*,rcs_Window,rcs_GC,int,int,int,int,int);
  public:
LINK    PaigowTile(int,int);
LINK    ~PaigowTile();
LINK    int getPips();
LINK    int getValue();
LINK    void setDirection(char);
LINK    void setSide(char);
LINK    void setPosition(int,int);
LINK    void paint(rcs_Display* display,rcs_Window win,rcs_GC gc);
  };

class PaigowDeck {
  protected:
    PaigowTile** tiles;
    int    numTiles;
    int    tilePos;
    void   newDeck();
  public:
LINK    PaigowDeck();
LINK    ~PaigowDeck();
LINK    void  shuffle();
LINK    PaigowTile* dealTile();
LINK    int   tilesLeft();
  };

class PaigowHand {
  protected:
    PaigowTile** tiles;
    int          numTiles;
    int          dir;
    int          x;
    int          y;
    void         analyze();
    int          counts[13];
    int          supreme;
    int          findTile(int);
    int          findTileValue(int);
  public:
LINK    PaigowHand();
LINK    ~PaigowHand();
LINK    int getRank();
LINK    int compare(PaigowHand*);
LINK    void setDirection(char);
LINK    void setPosition(int,int);
LINK    void paint(rcs_Display* display,rcs_Window win,rcs_GC gc);
LINK    PaigowTile* removeTile(int);
LINK    void addTile(PaigowTile*);
LINK    PaigowTile* getTile(int);
LINK    int getNumTiles();
LINK    void splitHand(PaigowHand*);
LINK    void setSide(char);
  };

LINK void rcs_makeName(char* buffer);
LINK void cardInit(rcs_Display* display,rcs_Window win);
LINK void cardDone(rcs_Display* display);

#endif

/* ******************** image ******************************* */

#ifdef SOLARIS
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;
#endif

#ifdef WIN32
typedef unsigned short u_int16_t;
typedef unsigned long  u_int32_t;
typedef short          int16_t;
typedef long           int32_t;
typedef int32_t        ssize_t;
#define LOG0(x)     log(lbuffer)
#define LOG1(x,y)   sprintf(lbuffer,x,y); log(lbuffer)
#define LOG2(x,y,z) sprintf(lbuffer,x,y,z); log(lbuffer)
#else
#define LOG0(x)     printf(x)
#define LOG1(x,y)   printf(x,y)
#define LOG2(x,y,z) printf(x,y,z)
#define O_BINARY    0
#endif


#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE -1
#endif

#ifndef _BYTE_
typedef unsigned char byte;
#define _BYTE_
#endif

#ifndef _WORD_
typedef u_int16_t word;
#define _WORD_
#endif

#ifndef _SWORD_
typedef int16_t sword;
#define _SWORD_
#endif

#ifndef _DWORD_
typedef u_int32_t dword;
#define _DWORD_
#endif

#ifndef _SDWORD_
typedef int32_t sdword;
#define _SDWORD_
#endif

#define TIFF_BYTE      1
#define TIFF_ASCII     2
#define TIFF_SHORT     3
#define TIFF_LONG      4
#define TIFF_RATIONAL  5

#define TIFF_COMP_NONE   1
#define TIFF_COMP_G4     4
#define TIFF_COMP_LZW    5

#define TIFF_TAG_SUBTYPE      254
#define TIFF_TAG_WIDTH        256
#define TIFF_TAG_HEIGHT       257
#define TIFF_TAG_BITSSAMPLE   258
#define TIFF_TAG_COMPRESSION  259
#define TIFF_TAG_PHOTOINTERP  262
#define TIFF_TAG_THRESHHOLD   263
#define TIFF_TAG_FILLORDER    266
#define TIFF_TAG_STRIPOFFSETS 273
#define TIFF_TAG_ORIENTATION  274
#define TIFF_TAG_SMPLSPIXEL   277
#define TIFF_TAG_ROWSSTRIP    278
#define TIFF_TAG_STRIPBYTES   279
#define TIFF_TAG_XRES         282
#define TIFF_TAG_YRES         283
#define TIFF_TAG_PLANECONF    284
#define TIFF_TAG_GROUP3       292
#define TIFF_TAG_GROUP4       293
#define TIFF_TAG_RESUNIT      296
#define TIFF_TAG_PAGENUMBER   297
#define TIFF_TAG_SOFTWARE     305
#define TIFF_TAG_PALETTE      320
#define TIFF_TAG_BADFAXLINES  326
#define TIFF_TAG_CLEANFAXDATA 327
#define TIFF_TAG_CONSBADLINES 328

#define TIFF_EOL               -1
#define TIFF_ERR               -2
#define TIFF_G4_PASS           1001
#define TIFF_G4_HORIZ          1002
#define TIFF_G4_VERT           1003
#define TIFF_G4_VR_1           1004
#define TIFF_G4_VR_2           1005
#define TIFF_G4_VR_3           1006
#define TIFF_G4_VL_1           1007
#define TIFF_G4_VL_2           1008
#define TIFF_G4_VL_3           1009


#define IMAGE_BILEVEL          1
#define IMAGE_GRAY             2
#define IMAGE_PALETTE          3
#define IMAGE_RGB16            4
#define IMAGE_RGB24            5
#define IMAGE_RGB32            6

#define CCITT_MODE_31D         1
#define CCITT_MODE_32D         2
#define CCITT_MODE_4           4

#define LZW_GIF                0
#define LZW_TIFF               1

typedef struct {
  dword  literal[288];
  dword  lcodes[288];
  dword  dist[33];
  dword  dcodes[33];
  byte* lzWindow;
  word  wPos;
  word  cPos;
  char  huffmanMode;
  char  readMode;
  word  compression;
  word  window;
  word  check;
  word  preset;
  word  flevel;
  int   fildes;
  char  mode;
  word  final;
  word  length;
  byte  current;
  int  bpos;
  byte  (*readByte)();
  } FLATE_CODEC;

typedef struct {
  char      byteOrder[2];
  word      tiffVersion;
  byte      dirOffset[4];
  } TIFF_HEADER;

typedef struct {
  byte      tag[2];
  byte      fieldType[2];
  byte      count[4];
  byte      offset[4];
  } TIFF_IFD_ENTRY;

typedef struct {
  byte             numEntries[2];
  TIFF_IFD_ENTRY*  entries;
  byte             nextDir[4];
  } TIFF_IFD;

typedef struct {
  byte  red;
  byte  green;
  byte  blue;
  } RCSGL_COLOR;

typedef struct {
  dword width;
  dword height;
  word  resUnit;
  dword dpiX;
  dword dpiY;
  byte  imageType;
  RCSGL_COLOR palette[256];
  dword bytesPerLine;
  byte* imageData;
  } IMAGE;

typedef struct {
  void (*writeByte)(byte value);
  byte (*readByte)();
  byte readOrder;
  byte current;
  byte pos;
  byte fillOrder;
  int  file;
  int  bPos;
  int  buflen;
  byte* buffer;
  } BitStream;

/*
typedef struct {
  dword      prefix;
  byte       suffix;
  } _LZW_ENTRY_;
*/

typedef struct {
  BitStream*   stream;
  char         mode;
  unsigned long stringTable[4097];
  unsigned long string;
  int          his[4097];
  int          los[4097];
  int          numStrings;
  int          bitLength;
  int          maxBitLength;
  dword        clearCode;
  dword        EODCode;
  char         eod;
  byte          variant;
  unsigned long currentCode;
  unsigned long lastCode;
  } LZW_CODEC;

typedef struct {
  BitStream* stream;
  char       runColor;
  byte       mode;
  int      a0,a1,a2;
  int      b1,b2;
  int      nextRun;
  byte*      refLine;
  byte*      curLine;
  dword      width;
  byte       keepColor;
  } CCITT;

typedef struct {
  byte*  bits;
  dword* code;
  dword* value;
  dword  offsets[64];
  dword  numCodes;
  dword  maxBits;
  } HUFFMAN_TABLE;

typedef struct {
  int  code;
  int  bits;
  } HUFF_VALUE;

typedef struct {
  int  count;
  int  freq;
  HUFF_VALUE* codes;
  } HUFF_LIST;

typedef struct {
  int   numLists;
  HUFF_LIST* lists;
  int        counts[32];
  int*       values;
  } HUFFMAN_ENCODER;

typedef struct {
  byte _rcsgl_buffer[4096];
  int  _rcsgl_bsize;
  int  _rcsgl_bpos;
  byte _rcsgl_mode;
  int  fildes;
} BUFFERED_IO;

#ifdef __cplusplus
extern "C" {
#endif

/* asciihex.c */
LINK void rcsgl_closeAsciiHex(BitStream* stream);
LINK void rcsgl_writeAsciiHex(BitStream* stream,void* buffer,size_t len);

/* batestamp.c */
LINK void rcsgl_bateStamp(IMAGE* image,char* str1,char* str2,
                            int w,int h,int tol);

/* bitstream.c */
LINK BitStream newWriteBitStream(byte fill,void (*func)(byte value),byte ro);
LINK BitStream newReadBitStream(byte fill,byte (*func)(),byte ro);
LINK BitStream newFileWriteBitStream(byte fill,int file,int buflen,byte ro);
LINK BitStream newFileReadBitStream(byte fill,int file,byte ro);
LINK void      closeWriteBitStream(BitStream* stream);
LINK void      writeValue(BitStream* stream,byte bits,dword value);
LINK dword     readValue(BitStream* stream,byte bits);
LINK byte      reverseByte(byte in);

/* bmp.c */
LINK void writeBMP(IMAGE* image,char* fname);
LINK IMAGE rcsgl_readBMP(int fildes);

/* buffered.c */
LINK BUFFERED_IO rcsgl_bufferedOpen(char*fname,int flags);
LINK void rcsgl_bufferedRead(BUFFERED_IO* io,void* dest,int count);
LINK void rcsgl_bufferedWrite(BUFFERED_IO* io,const void* src,int count);
LINK void rcsgl_bufferedClose(BUFFERED_IO* io);
LINK void rcsgl_bufferedLseek(BUFFERED_IO* io,off_t offset,int whence);

/* flate.c */
LINK FLATE_CODEC rcsgl_newFlateCodec(int fildes,char mode,byte (*b)());
LINK void rcsgl_closeFlateCodec(FLATE_CODEC* codec);
LINK ssize_t rcsgl_readFlate(FLATE_CODEC* codec,void* buffer,size_t len);

/* gif.c */
LINK void  rcsgl_writeGIF(IMAGE* image,char* fname);
LINK IMAGE rcsgl_readGIF(int fildes);

/* huffman.c */
LINK HUFFMAN_TABLE rcsgl_newHuffmanTable();
LINK HUFFMAN_TABLE rcsgl_countsValuesToHuffman(dword* counts,dword* values);
LINK void          rcsgl_freeHuffmanTable(HUFFMAN_TABLE* ht);
LINK void          rcsgl_addHuffmanCode(HUFFMAN_TABLE* ht,byte bits,
                           dword code,dword value);
LINK dword rcsgl_readHuffmanValue(HUFFMAN_TABLE* table,BitStream* stream);
LINK dword rcsgl_getHuffmanCode(HUFFMAN_TABLE* table,dword value,dword* bits);
LINK void rcsgl_dumpHuffmanTable(HUFFMAN_TABLE* ht);
LINK HUFFMAN_ENCODER rcsgl_newHuffmanEncoder();
LINK void rcsgl_addCode(HUFFMAN_ENCODER* enc,int code);
LINK void rcsgl_freeHuffmanEncoder(HUFFMAN_ENCODER* enc);
LINK void ht();

/* image.c */
LINK void  rcsgl_blackBorderRemoval(IMAGE* image,dword maxLen,byte thresh);
LINK IMAGE rcsgl_loadImage(char* fname);
LINK dword rcsgl_findLeft(IMAGE* image,int range,int tol);
LINK dword rcsgl_findRight(IMAGE* image,int range,int tol);
LINK dword rcsgl_findTop(IMAGE* image,int range,int tol);
LINK dword rcsgl_findBottom(IMAGE* image,int range,int tol);
LINK void  rcsgl_hline(IMAGE* image,int x1,int x2,int y,byte r,byte g,byte b);
LINK void  rcsgl_vline(IMAGE* image,int x,int y1,int y2,byte r,byte g,byte b);
LINK IMAGE rcsgl_newImage(byte imageType,int width,int height);
LINK void  rcsgl_register(IMAGE* image,int range,int tol,dword border);
LINK void  rcsgl_whiteClip(IMAGE* image,int range,int tol,dword border);
LINK void  rcsgl_copyImage(IMAGE* dst,IMAGE* src,dword dx,dword dy,
                             dword w,dword h,dword sx,dword sy);
LINK void rcsgl_drawBox(IMAGE* image,int x,int y,int w,int h,
                          byte r,byte g,byte b);
LINK void rcsgl_drawString(IMAGE* image,int x,int y,const char* str,
                          byte r,byte g,byte b);
LINK int  rcsgl_hLineClear(IMAGE* image,int x1,int x2,int y,
                          byte r,byte g,byte b);
LINK int  rcsgl_boxClear(IMAGE* image,int x,int y,int w,int h,int tol,
                          byte r,byte g,byte b);
LINK int  rcsgl_getPixel(IMAGE* image,int x,int y);
LINK void rcsgl_setPixel(IMAGE* image,int x,int y,int color);
LINK IMAGE rcsgl_rotateImage(IMAGE* image,int a);
LINK void  rcsgl_deskew(IMAGE* image,int max);
LINK IMAGE scaleAvg(IMAGE* src,int scale);
LINK IMAGE rcsgl_colorReduce(IMAGE* image,int numColors);
LINK void rcsgl_closeImage(IMAGE* image);

/* jpeg.c */
LINK IMAGE rcsgl_readJpeg(int fildes);
LINK void  rcsgl_jpegInfo(int fildes);

/* log.c */
LINK void rcsgl_log(const char* buffer);
LINK void enableLog();
LINK void disableLog();
LINK void log_enter(const char* buffer);
LINK void log_exit(const char* buffer);
LINK void log_indent();
LINK void log_outdent();
LINK void setLogfile(const char* name);
LINK void writeLog(const char* buffer);
LINK void log_noindent(const char* buffer);
LINK void resetLogOffset();

extern char rcsgl_b[256];

/* lzw.c */
LINK LZW_CODEC rcsgl_newLzwEncoder(BitStream* stream,int numCodes,int maxBit);
LINK LZW_CODEC rcsgl_newLzwDecoder(BitStream* stream,int numCodes,int maxBit);
LINK void rcsgl_lzwWriteByte(LZW_CODEC* lzw,byte value);
LINK void rcsgl_lzwClose(LZW_CODEC* lzw);
LINK void rcsgl_writeLzw(LZW_CODEC* lzw,void* buffer,size_t len);
LINK int rcsgl_lzwReadByte(LZW_CODEC* lzw);
LINK size_t rcsgl_readLzw(LZW_CODEC* lzw,void* buffer,size_t len);

/* pbm.c */
LINK void writePBM(IMAGE* image,char* fname);

/* pgm.c */
LINK void writePGM(IMAGE* image,char* fname);

/* png.c */
LINK IMAGE rcsgl_readPNG(int fildes);

/* ppm.c */
LINK void rcsgl_writePPM(IMAGE* image,char* fname);

/* tiff.c */
LINK int   rcsgl_tiffPageCount(int fildes);
LINK IMAGE rcsgl_readTiff(int fildes);
IMAGE rcsgl_readTiffPage(int fildes,int page);
LINK int   rcsgl_writeTiff(IMAGE* image,char* fname,int comp);
LINK IMAGE rcsgl_readTiffByName(char* fname);

/* ccitt.c */
LINK dword ccitt_getValueForCode(BitStream* stream,char color);
LINK CCITT ccitt_newCCITTDecoder(BitStream* stream,byte mode);
LINK CCITT ccitt_newCCITTEncoder(BitStream* stream,byte mode,dword width);
LINK void  ccitt_closeCCITT(CCITT* decoder);
LINK void  ccitt_encode(CCITT* encoder,byte color);
LINK dword ccitt_nextRun(CCITT* decoder);
LINK void  ccitt_newLine(CCITT* decoder);

/* xbm.c */
LINK void writeXBM(IMAGE* image,char* fname);

/* ycbcr.c */
LINK void  rcsgl_rgbToYCbCr(byte red,byte green,byte blue,
                              byte* y,byte* cb,byte* cr);
LINK void  rcsgl_yCbCrToRgb(byte y,byte cb,byte cr,
                              byte* r,byte* g,byte* b);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#ifdef UNIX
#undef LINK
#define LINK
#endif

#define PDF_FONT_COURIER      "Courier"
#define PDF_FONT_COURIER_B    "Courier-Bold"
#define PDF_FONT_COURIER_I    "Courier-Oblique"
#define PDF_FONT_COURIER_BI   "Courier-BoldOblique"
#define PDF_FONT_HELVETICA    "Helvetica"
#define PDF_FONT_HELVETICA_B  "Helvetica-Bold"
#define PDF_FONT_HELVETICA_I  "Helvetica-Oblique"
#define PDF_FONT_HELVETICA_BI "Helvetica-BoldOblique"
#define PDF_FONT_TIMES        "Times-Roman"
#define PDF_FONT_TIMES_B      "Times-Bold"
#define PDF_FONT_TIMES_I      "Times-Italic"
#define PDF_FONT_TIMES_BI     "Times-BoldItalic"

#define PDF_ENC_ROMAN         "MacRomanEncoding"
#define PDF_ENC_EXPERT        "MacExpertEncoding"
#define PDF_ENC_WINANSI       "WinAnsiEncoding"
#define PDF_ENC_PDF           "PDFDocEncoding"
#define PDF_ENC_STANDARD      "StandardEncoding"

#define PDF_TYPE_0            "Type0"
#define PDF_TYPE_1            "Type1"
#define PDF_TYPE_3            "Type3"
#define PDF_TYPE_TT           "TrueType"

typedef struct {
  dword width;
  dword height;
  dword dpiX;
  dword dpiY;
  } rcspdf_imageData;

class rcspdf_Ref;

class rcspdf_Object {
  protected:
    int          objNum;
    int          genNum;
    char*        objectName;
    size_t       filePos;
    virtual void _print(int fildes);
  public:
    LINK rcspdf_Object();
    LINK virtual ~rcspdf_Object();
    LINK virtual void print(int fildes);
    LINK int getObjNum();
    LINK int getGenNum();
    LINK void setObjectNumber(int o,int g);
    LINK rcspdf_Ref* getRef();
    LINK size_t  getFilePos();
    LINK virtual char*   isa();
    LINK virtual void    setName(const char* n);
  };

class rcspdf_Ref:public rcspdf_Object {
  protected:
    int objNum;
    int genNum;
  public:
    LINK rcspdf_Ref(int obj,int gen);
    LINK virtual ~rcspdf_Ref();
    LINK virtual void print(int fildes);
    LINK int getObjNum();
    LINK int getGenNum();
    LINK void readValue(int fildes);
  };

class rcspdf_Null:public rcspdf_Object {
  protected:
  public:
    LINK rcspdf_Null();
    LINK virtual ~rcspdf_Null();
    LINK virtual void _print(int fildes);
  };

class rcspdf_Boolean:public rcspdf_Object {
  protected:
    char value;
  public:
    LINK rcspdf_Boolean(char v);
    LINK virtual ~rcspdf_Boolean();
    LINK virtual void _print(int fildes);
  };

class rcspdf_Integer:public rcspdf_Object {
  protected:
    int value;
  public:
    LINK rcspdf_Integer(int v);
    LINK virtual ~rcspdf_Integer();
    LINK virtual void _print(int fildes);
    LINK void readValue(int fildes);
    LINK int getValue();
  };

class rcspdf_UInteger:public rcspdf_Object {
  protected:
    unsigned int value;
  public:
    LINK rcspdf_UInteger(unsigned int v);
    LINK virtual ~rcspdf_UInteger();
    LINK virtual void _print(int fildes);
    LINK void readValue(int fildes);
    LINK unsigned int getValue();
  };

class rcspdf_Real:public rcspdf_Object {
  protected:
    double value;
  public:
    LINK rcspdf_Real(double v);
    LINK virtual ~rcspdf_Real();
    LINK virtual void _print(int fildes);
    LINK void readValue(int fildes);
  };

class rcspdf_String:public rcspdf_Object {
  protected:
    char* value;
  public:
    LINK rcspdf_String(char* v);
    LINK virtual ~rcspdf_String();
    LINK virtual void _print(int fildes);
  };

class rcspdf_HexString:public rcspdf_Object {
  protected:
    char* value;
  public:
    LINK rcspdf_HexString(char* v);
    LINK virtual ~rcspdf_HexString();
    LINK virtual void _print(int fildes);
  };

class rcspdf_Name:public rcspdf_Object {
  protected:
    char* value;
  public:
    LINK rcspdf_Name(const char* v);
    LINK virtual ~rcspdf_Name();
    LINK virtual void _print(int fildes);
    LINK char*   getValue();
    LINK void readValue(int fildes);
  };

class rcspdf_Array:public rcspdf_Object {
  protected:
    rcspdf_Object** values;
    int             numObjects;
    virtual void _print(int fildes);
  public:
    LINK rcspdf_Array();
    LINK virtual ~rcspdf_Array();
    LINK virtual void add(rcspdf_Object* obj);
    LINK rcspdf_Object* getEntry(int pos);
    LINK void setEntry(int pos,rcspdf_Object* obj);
    LINK void readValue(int fildes);
    LINK int  length();
  };

class rcspdf_Dictionary:public rcspdf_Object {
  protected:
    rcspdf_Name**   keys;
    rcspdf_Object** values;
    int             numObjects;
  public:
    LINK rcspdf_Dictionary();
    LINK virtual ~rcspdf_Dictionary();
    LINK virtual void add(rcspdf_Name* name,rcspdf_Object* obj);
    LINK virtual void _print(int fildes);
    LINK rcspdf_Object* getEntry(const char* key);
    LINK void update(const char* key,rcspdf_Object* obj);
    LINK int  itemCount();
    LINK rcspdf_Name* getKeyAt(int pos);
    LINK rcspdf_Object* getValueAt(int pos);
    LINK void readValue(int fildes);
  };

class rcspdf_Stream:public rcspdf_Object {
  protected:
    rcspdf_Dictionary* dict;
    char* value;
    int   size;
  public:
    LINK rcspdf_Stream(char* v,int sz);
    LINK virtual ~rcspdf_Stream();
    LINK virtual void _print(int fildes);
    LINK void addDict(rcspdf_Name* name,rcspdf_Object* obj);
  };

class rcspdf_Image:public rcspdf_Stream {
  protected:
    void _CCITTImage(IMAGE* img);
  public:
    LINK rcspdf_Image(IMAGE* img);
    LINK virtual void _print(int fildes);
    LINK ~rcspdf_Image();
    LINK void add(byte v);
  };

class rcspdf_PdfStream:public rcspdf_Stream {
  protected:
    void _add(const char* str);
  public:
    LINK rcspdf_PdfStream();
    LINK ~rcspdf_PdfStream();
    LINK void beginTextObject();
    LINK void closeFillAndStroke();
    LINK void closePath();
    LINK void closeStroke();
    LINK void concatMatrix(int a,int b,int c,int d,int e,int f);
    LINK void curve(int x1,int y1,int x2,int y2,int x3,int y3);
    LINK void doXObject(char* name);
    LINK void endPathNoDraw();
    LINK void endTextObject();
    LINK void fill();
    LINK void fillAndStroke();
    LINK void lineTo(int x,int y);
    LINK void lineWidth(int x);
    LINK void moveTo(int x,int y);
    LINK void nextLine();
    LINK void nextLine(double x,double y);
    LINK void nextLineSetLeading(double x,double y);
    LINK void nextLineShowText(char* str);
    LINK void rectangle(int x,int y,int w,int h);
    LINK void restoreState();
    LINK void saveState();
    LINK void setCharacterSpacing(double s);
    LINK void setDash(int* data,int numData,int phase);
    LINK void setFontAndSize(char* name,double s);
    LINK void setGrayFillColor(double g);
    LINK void setGrayStrokeColor(double g);
    LINK void setHorizontalSpacing(double s);
    LINK void setRenderingMode(int x);
    LINK void setRGBFillColor(double r,double g,double b);
    LINK void setRGBStrokeColor(double r,double g,double b);
    LINK void setTextLeading(double s);
    LINK void setTextMatrix(double a,double b,double c,double d,double e,double f);
    LINK void setTextRise(double s);
    LINK void setWordSpacing(double s);
    LINK void showText(char* str);
    LINK void stroke();
  };

class rcspdf_Pdf;

class rcspdf_Annotation:public rcspdf_Dictionary {
  protected:
  public:
    LINK rcspdf_Annotation();
    LINK ~rcspdf_Annotation();
    LINK void annotAsCircle();
    LINK void annotAsLine(int x1,int y1,int x2,int y2);
    LINK void annotAsSquare();
    LINK void annotAsText();
    LINK void setBorderWidth(int w);
    LINK void setColor(double r,double g,double b);
    LINK void setPosition(int x,int y,int w,int h);
    LINK void setText(char* t);
    LINK void setTitle(char* t);
  };

class rcspdf_Catalog:public rcspdf_Dictionary {
  protected:
  public:
    LINK rcspdf_Catalog(rcspdf_Pdf* pdf);
    LINK virtual ~rcspdf_Catalog();
    LINK virtual void _print(int fildes);
  };

class rcspdf_Page:public rcspdf_Dictionary {
  protected:
    rcspdf_Pdf* pdf;
  public:
    LINK rcspdf_Page(rcspdf_Pdf* p);
    LINK virtual ~rcspdf_Page();
    LINK virtual void _print(int fildes);
    LINK rcspdf_PdfStream* newPdfStream();
    LINK void setPageSize(double w,double h);
    LINK void setRotation(int r);
    LINK rcspdf_imageData addImage(char* fileName,char* imgName);
    LINK void addFont(char* typ,char* font,char* enc,char* name);
    LINK void addResource(const char* typ,char* name,rcspdf_Object* obj);
    LINK void addAnnotation(rcspdf_Annotation* annot);
  };

class rcspdf_PageTree:public rcspdf_Dictionary {
  protected:
    rcspdf_Pdf* pdf;
    rcspdf_Page** pages;
    int           numPages;
  public:
    LINK rcspdf_PageTree(rcspdf_Pdf* p);
    LINK virtual ~rcspdf_PageTree();
    LINK virtual void _print(int fildes);
    LINK rcspdf_Page* newPage();
  };

class rcspdf_Pdf {
  protected:
    int nextOID;
    rcspdf_Catalog*     catalog;
    rcspdf_PageTree*    pagetree;
    rcspdf_Dictionary* trailer;
    size_t             xrefPos;
    rcspdf_Object**    objects;
    int                numObjects;
  public:
    LINK rcspdf_Pdf();
    LINK ~rcspdf_Pdf();
    LINK void print(int fildes);
    LINK void printToFileName(char* fname);
    LINK int  getNextOID();
    LINK void add(rcspdf_Object*);
    LINK rcspdf_Page* newPage();
  };

LINK char pdfType(int fildes);

#endif



/* *************************************************** */
#endif
