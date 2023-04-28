/*AmiX - polchat1.cpp - (c) by ABUKAJ (J.M.Kowalski)*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

#include <string>

#include "polchat.h"
#include "polchat2.h"
#include "interface.h"
#include "temp.h"


tank::~tank()
{
  if (NULL != data)
  {
    delete data;
  }
}

tank::tank(int len, char * dat)
{
  length = len;
  data = new char[len];
  if (data != NULL)
  {
    memcpy(data, dat, len);
  }
}

tank *readtank(int sfd)
{
  unsigned char buffer[4];

  if (connected)
  {
    int len = 4;
    int tmp;
    char * ptr = (char *) buffer;

    while (len > 0 && 0 < (tmp = read(sfd, ptr, len)))
    {
      len -= tmp;
      ptr += tmp;
    }

    if (len == 0)
    {
      len = ((buffer[0] * 256 + buffer[1]) * 256 + buffer[2]) * 256 + buffer[3] - 4;
      int size = len;
      char * data = new char[len];
      if (NULL != data)
      {
        ptr = data;
        while (len)
        {
          tmp = read(sfd, ptr, len);

          if (0 < tmp)
          {
            len -= tmp;
            ptr += tmp;
          }
          else
          {
            connected = 0;
            close(sfd);
            delete [] data;
          }
        }
        tank * res = new tank(size, data);
        delete [] data;
        return res;
      }
    }
    else
    {
      connected = 0;
      close(sfd);
    }
  }
  return NULL;
}


int tank::send(int sfd)
{
  char buffer[4];

  int tmp = length + 4;
  buffer[3] = (char) (tmp % 256);
  tmp /= 256;
  buffer[2] = (char) (tmp % 256);
  tmp /= 256;
  buffer[1] = (char) (tmp % 256);
  tmp /= 256;
  buffer[0] = (char) tmp;

  write(sfd, buffer, 4);
  write(sfd, data, length);
  
  return 0;
}

void tank::dump()
{
  int size;
  int i, j;

  interface->put("Size: 0x");
  interface->puthex((unsigned long) length, 8);
  interface->nl();
  size = length;
  for (i = 0; i < size / 32; i++)
  {
    interface->put("0x");
    interface->puthex(i * 32, 4);
    for (j = 0; j < 32; j++)
    {
      interface->putchar(' ');
      interface->puthex((unsigned char) data[i * 32 + j], 2);
    }
    interface->put("  ");
    for (j = 0; j < 32; j++)
    {
      if (isgraph((unsigned char) data[i * 32 + j]))
      {
        interface->putchar((unsigned char) data[i * 32 + j]);
      }
      else
      {
        interface->putchar('.');
      }
    }
    interface->nl();
  }

  interface->put("0x");
  interface->puthex(i * 32, 4);
  for (j = 0; j < size % 32; j++)
  {
    interface->putchar(' ');
    interface->puthex((unsigned char) data[i * 32 + j], 2);
  }
  for (; j < 32; j++)
  {
    interface->put("   ");
  }
  interface->put("  ");
  for (j = 0; j < size % 32; j++)
  {
    if (isgraph((unsigned char) data[i * 32 + j]))
    {
      interface->putchar((unsigned char) data[i * 32 + j]);
    }
    else
    {
      interface->putchar('.');
    }
  }
  interface->nl();
}
