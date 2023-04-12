/*AmiX - polchat2.h - v. 0.2 - (c) by ABUKAJ (J.M.Kowalski)*/
unsigned char *welcome(unsigned char *, unsigned char *,
                           unsigned char *, int);
unsigned char *welcome2(unsigned char *, unsigned char *,
                           unsigned char *, int);
void printpol(char *);
part *makemsg(unsigned char *);
void verbosedump(part *);
void processpart(part *, int);
