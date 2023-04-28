/*AmiX - polchat1.h - (c) by ABUKAJ (J.M.Kowalski)*/
#ifndef AMIX_POLCHAT1_H
#define AMIX_POLCHAT1_H

#include <time.h>

#include <string>

#include "interface.h"

class tank
{
  protected:
    long length;
    char *data;

  public:
    tank(int, char *);
    ~tank();
    int send(int);
    void dump();

    long len()
    {
      return length;
    }

    const char * get()
    {
      return data;
    }

};

tank *readtank(int);

#endif
