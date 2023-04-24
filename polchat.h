/*AmiX - polchat.h - v. 0.2 - (c) by ABUKAJ (J.M.Kowalski)*/
struct nn{
  short status;
  short unknown;
  char *nick;
  struct nn *next;
  };

typedef struct nn nicknode;

void addnick(char *, short, short);
void remnick(char *);
void printnicks();
void freenicklist(nicknode **);
const char *getnickbyprefix(char *, int, nicknode *);
int openlog(char *);
void closelog();
int printlog(char *, char *);

extern int cud;
extern int debug;
extern int verbose;
extern int bell;
extern int run;
extern int connected;
extern double period;
extern nicknode *nicks;
extern int colourt[];
extern int colourop;
extern char *roomname;
extern char *roomdesc;
