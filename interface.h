#ifndef AMIX_INTERFACE_H
#define AMIX_INTERFACE_H

#include <time.h>

#include <string>
#include <stack>

#include <ncursesw/ncurses.h>

#include "polchat.h"

#define NICKLIST_Y 0

#define WINDOW_X 0
#define WINDOW_Y 2 

#define CONSOLE_X 0
#define CONSOLE_H 1

#define TITLE_X 0
#define TITLE_Y 0
#define TITLE_H 2

#define BUFFSIZE 10240

class amixInterface
{
  protected:
    WINDOW * chatwindow;
    WINDOW * nickwindow;
    WINDOW * titlewindow;
    WINDOW * consolewindow;
    int nicklist_x;
    int nicklist_y;
    int nicklist_h;
    int console_x;
    int console_y;
    int console_w;
    int console_h;
    int title_x;
    int title_y;
    int title_w;
    int title_h;
    int window_x;
    int window_y;
    int window_w;
    int window_h;
    int scr_rows;
    int scr_cols;
    bool window_updated;

    int inlen;
    int ptr;
    int len;
    int consptr;
    unsigned char buffer[BUFFSIZE];
    int utf8charToGo;
    int utf8left;
    void getsize();
    std::stack<int> colourstack;

  public:
    int nicklist_w;
    bool useattr;

    amixInterface();
    ~amixInterface();
    void colouron(int);
    void colouroff();
    void colourflush();
    void window_attron(int);
    void window_attroff(int);
    void resize();
    void window_redraw();
    void update();
    void update_all();
    void nl();
    void putchar(unsigned char);
    void puthex(unsigned int, unsigned int);
    void put(const char *);
    void putforce(const char *);
    void print();
    void printtitle();
    void printpol(const char *);
    char * console_input();
    void printnicklist();
    std::string input_password();
    void console_update();
};

char *readtoken(const char *);
int transformrgb(int, int, int);

extern amixInterface * interface;

#endif
