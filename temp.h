/*ABUKAJtools - temp.h - v. 0.4 - Jakub Mateusz Kowalski*/
/*#define false 0
#define true (~false)*/

void clearscreen();
void mypause();
int ncsstrcmp(char *, char *);
int ncsstrncmp(char *, char *, int);
char inttohex(int);
unsigned char unicode2iso(unsigned int);
unsigned int iso2unicode(unsigned char);
char *iso2utf8string(char *);
char *utf82isostring(char *);
char *clonestring(char *);
char *freadline(FILE *);
