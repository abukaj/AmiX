/*ABUKAJtools - temp.h - v. 0.3 - Jakub Mateusz Kowalski*/
/*#define false 0
#define true (~false)*/

void clearscreen();
void mypause();
int ncsstrcmp(char *, char *);
int ncsstrncmp(char *, char *, int);
char inttohex(int);
unsigned char unicode2iso(unsigned int);
unsigned int iso2unicode(unsigned char);
unsigned char *iso2utf8string(unsigned char *);
unsigned char *utf82isostring(unsigned char *);
