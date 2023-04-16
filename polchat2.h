/*AmiX - polchat2.h - v. 0.2 - (c) by ABUKAJ (J.M.Kowalski)*/
#include "polchat1.h"

unsigned char *welcome(unsigned char *, unsigned char *,
                           unsigned char *, int);
unsigned char *welcome2(unsigned char *, unsigned char *,
                           unsigned char *, int);
part *makemsg(unsigned char *);
void verbosedump(part *);
void processpart(part *, int);
