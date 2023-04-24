/*AmiX - polchat1.h - v. 0.1 - (c) by ABUKAJ (J.M.Kowalski)*/
#include <time.h>

struct structtank{
  long length;
  char *data;
  };

typedef struct structtank tank;

char *wrapstring(char *);
int wrapsize(char *);
char *unwrapstring(char *);

tank *readtank(int);
void freetank(tank **);
int sendtank(tank *, int);

void tankdump(tank *);


