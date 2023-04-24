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

//char *wrapstring(const char *str)
//{
//  char *result = NULL;
//  unsigned int len;
//
//  len = strlen(str);
//  if (NULL != (result = new char[len + 3]))
//  {
//    result[0] = (char) (len / 256);
//    result[1] = (char) (len % 256);
//    strncpy(result + 2, str, len);
//  }
//  return result;
//}
//
//
//unsigned int wrapsize(const char * string)
//{
//  return ((unsigned char) string[0]) * 256 + ((unsigned char) string[1]) + 3;
//}
//
//
//char *unwrapstring(const char *str){
//  char *result = NULL;
//  char *tmp;
//  unsigned int len;
//
//  len = ((unsigned char) str[0]) * 256 + (unsigned char) str[1];
//  if (NULL != (result = new char[len + 1]))
//  {
//    strncpy(result, str + 2, len);
//    result[len] = '\0';
//  }
//  return result;
//}
//
///*wrappedString::wrappedString(std::string str)
//{
//  int len = str.length();
//
//  window_put("BBB");
//
//  if (NULL != (content = new char [len + 3]))
//  {
//    content[0] = (char) (len / 256);
//    content[1] = (char) (len % 256);
//    strcpy(content + 2, str.c_str());
//  }
//}*/
//
//wrappedString::wrappedString(const char * str)
//{
//  int len = wraplength(str);
//
//  if (NULL != (content = new char [len]))
//  {
//    memcpy(content, str, len);
//    content[len - 1] = '\0';
//  }
//}
//
//
//char * wrappedString::c_str()
//{
//  char *result = NULL;
//  int len = size() - 3;
//
//  if (NULL != (result = new char [len + 1]))
//  {
//    strcpy(result, content + 2);
//  }
//  return result;
//}
//
//
//
//int partlen(char *part)
//{
//  return (((unsigned char) part[0] * 256 + (unsigned char) part[1]) * 256
//              + (unsigned char) part[2]) * 256 + (unsigned char) part[3];
//}


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

  window_put("Size: 0x");
  window_puthex((unsigned long) length, 8);
  window_nl();
  size = length;
  for (i = 0; i < size / 32; i++)
  {
    window_put("0x");
    window_puthex(i * 32, 4);
    for (j = 0; j < 32; j++)
    {
      window_putchar(' ');
      window_puthex((unsigned char) data[i * 32 + j], 2);
    }
    window_put("  ");
    for (j = 0; j < 32; j++)
    {
      if (isgraph((unsigned char) data[i * 32 + j]))
      {
        window_putchar((unsigned char) data[i * 32 + j]);
      }
      else
      {
        window_putchar('.');
      }
    }
    window_nl();
  }

  window_put("0x");
  window_puthex(i * 32, 4);
  for (j = 0; j < size % 32; j++)
  {
    window_putchar(' ');
    window_puthex((unsigned char) data[i * 32 + j], 2);
  }
  for (; j < 32; j++)
  {
    window_put("   ");
  }
  window_put("  ");
  for (j = 0; j < size % 32; j++)
  {
    if (isgraph((unsigned char) data[i * 32 + j]))
    {
      window_putchar((unsigned char) data[i * 32 + j]);
    }
    else
    {
      window_putchar('.');
    }
  }
  window_nl();
}
