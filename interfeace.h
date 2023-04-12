#include <ncurses.h>

#define NICKLIST_X nicklist_x
#define NICKLIST_Y 0
#define NICKLIST_WIDTH 40 
#define NICKLIST_HEIGHT nicklist_h

#define WINDOW_X 0
#define WINDOW_Y 4 
#define WINDOW_WIDTH window_w 
#define WINDOW_HEIGHT window_h

#define CONSOLE_X 0
#define CONSOLE_H 3

#define TITLE_X 0
#define TITLE_Y 0
#define TITLE_H 4



struct sn{
  char *string;
  int len;
  struct sn *next;
  };

typedef struct sn stringnode;

typedef struct{
  short length;
  stringnode *body;
  } line;

extern line *window;
extern WINDOW *chatwindow;
extern WINDOW *nickwindow;

extern WINDOW *titlewindow;
extern WINDOW *consolewindow;

extern int nicklist_x;
extern int nicklist_h;
extern int console_y;
extern int console_w;
extern int title_w;

extern int scr_rows;
extern int scr_cols;
extern int window_h;
extern int window_w;

void window_init();
void window_nl();
void window_done();
void window_put(char *);
void window_putforce(char *);
void window_print();
char *readtoken(char *);
void remsn(stringnode **);
stringnode *newsn(char *);
void addstringnode(stringnode **, char *);
void printtitle();