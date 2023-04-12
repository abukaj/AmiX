/*AmiX - polchat1.h - v. 0.1 - (c) by ABUKAJ (J.M.Kowalski)*/
struct structpart{
  short headerlen;
  short nstrings;
  short *header;
  char **strings;
  struct structpart *next;
  };

typedef struct structpart part;

unsigned char *wrapstring(unsigned char *);
int wrapsize(unsigned char *);
unsigned char *unwrapstring(unsigned char *);
int partlen(unsigned char *);
unsigned char *readpart(int);
part *parsepart(unsigned char *);
void freepart(part **);
int sendpol(part *, int);
void partdump(unsigned char *);

void putmsg(part *);
void sendnext(int);

extern part *tosend;
