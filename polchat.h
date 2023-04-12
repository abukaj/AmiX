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
void printnicks(nicknode *);
void freenicklist(nicknode **);

extern int debug;
extern int verbose;
extern int bell;
extern int run;
extern double period;
extern nicknode *nicks;
extern int colour[];
extern char *roomname;
extern char *roomdesc;
