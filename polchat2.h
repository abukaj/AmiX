/*AmiX - polchat2.h - v. 0.2 - (c) by ABUKAJ (J.M.Kowalski)*/
#include "polchat1.h"

struct structpart{
  short headerlen;
  short nstrings;
  short *header;
  char **strings;
  struct structpart *next;
  };

typedef struct structpart part;

part *readpart(int);
part *parsetank(tank *);
part *welcome(char *, char *, char *, char *, char *);
part *welcome3(char *, char *, char *, char *);
part *makemsg(char *);
void verbosedump(part *);
void processpart(part *, int);


void freepart(part **);
int sendpol(part *, int);

void putmsg(part *);
void sendnext(int);

extern part *tosend;
extern time_t last;
