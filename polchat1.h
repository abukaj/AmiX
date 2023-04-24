/*AmiX - polchat1.h - v. 0.1 - (c) by ABUKAJ (J.M.Kowalski)*/
struct structpart{
  short headerlen;
  short nstrings;
  short *header;
  char **strings;
  struct structpart *next;
  };

typedef struct structpart part;

char *wrapstring(char *);
int wrapsize(char *);
char *unwrapstring(char *);
int partlen(char *);
char *readpart(int);
part *parsepart(char *);
void freepart(part **);
int sendpol(part *, int);
void partdump(char *);

void putmsg(part *);
void sendnext(int);

extern part *tosend;
