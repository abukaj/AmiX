#include <time.h>

#include <string>

#include <ncursesw/ncurses.h>

#include "polchat.h"

#ifndef AMIX_INTERFACE_H
#define AMIX_INTERFACE_H

#define NICKLIST_X nicklist_x
#define NICKLIST_Y 0
#define NICKLIST_WIDTH nicklist_w 
#define NICKLIST_HEIGHT nicklist_h

#define WINDOW_X 0
#define WINDOW_Y 2 
#define WINDOW_WIDTH window_w 
#define WINDOW_HEIGHT window_h

#define CONSOLE_X 0
#define CONSOLE_H 1

#define TITLE_X 0
#define TITLE_Y 0
#define TITLE_H 2

#define BUFFSIZE 10240


typedef enum
  {
  PRIV_FROM = 0,
  PRIV_TO = 1
  } privinfo;

extern WINDOW *chatwindow;
extern WINDOW *nickwindow;
extern WINDOW *titlewindow;
extern WINDOW *consolewindow;

extern int nicklist_x;
extern int nicklist_h;
extern int nicklist_w;
extern int console_y;
extern int console_w;
extern int title_w;

extern int scr_rows;
extern int scr_cols;
extern int window_h;
extern int window_w;

extern int useattr;

extern int window_updated;

void window_init();
void window_nl();
void window_done();
void window_put(const char *);
void window_putchar(unsigned char);
void window_puthex(unsigned int, unsigned int);
void window_putforce(char *);
void window_print();
void window_colouron(int);
void window_colouroff();
void window_attron(int);
void window_attroff(int);

void window_resize();
void window_redraw();
void update_all();

char *readtoken(const char *);
/*void remsn(stringnode **);
stringnode *newsn(char *);
void addstringnode(stringnode **, char *);*/
void printtitle();
void printpol(const char *);
char *console_input();
void console_update();

void printnicklist();

int transformrgb(int, int, int);

//void priv(privinfo, const char *, const char *);

std::string input_password();

#endif
