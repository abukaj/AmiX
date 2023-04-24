/*AmiX - polchat.c - v. 0.1 - (c) by ABUKAJ (J.M.Kowalski)*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

#include "polchat.h"
#include "polchat2.h"
#include "interfeace.h"
#include "temp.h"

char *wrapstring(char *string){
  char *result = NULL;
  char *tmp;
  int len;

  /*if (NULL != (tmp = iso2utf8string(string)))
    {*/tmp = string;
    len = strlen(tmp);
    if (NULL != (result = calloc(len + 3, sizeof(char)))){
      result[0] = (char) (len / 256);
      result[1] = (char) (len % 256);
      strncpy(result + 2, tmp, len);
      }
    /*free(tmp);
    }*/
  return result;
  }


int wrapsize(char * string){
  return ((unsigned char) string[0]) * 256 + ((unsigned char) string[1]) + 3;
  }


char *unwrapstring(char *string){
  char *result = NULL;
  char *tmp;
  int len;

  len = ((unsigned char) string[0]) * 256 + (unsigned char) string[1];
  if (NULL != (tmp = calloc(len + 1, sizeof(char)))){
    strncpy(tmp, string + 2, len);
    tmp[len] = '\0';
    result = tmp;/* utf82isostring(tmp);
    free(tmp);*/
    }
  return result;
  }


int partlen(char *part){
  return (((unsigned char) part[0] * 256 + (unsigned char) part[1]) * 256
              + (unsigned char) part[2]) * 256 + (unsigned char) part[3];
  }


void freetank(tank **ptr)
  {
  if (NULL != ptr)
    {
    if (NULL != *ptr)
      {
      if (NULL != (*ptr)->data)
        {
        free((*ptr)->data);
        }
      free(*ptr);
      *ptr = NULL;
      }
    }
  }


tank *readtank(int sfd){
  unsigned char buffer[4];
  tank *result = NULL;
  int len, tmp;
  char *ptr;

  if (connected)
    {
    if (0 < read(sfd, buffer, 4))
      {
      if (NULL != (result = malloc(sizeof(tank))))
        {
        result->length = len = ((buffer[0] * 256 + buffer[1]) * 256 + buffer[2]) * 256 + buffer[3] - 4;
        if (NULL != (result->data = ptr = malloc(len)))
          {
          while (len)
            {
            if (0 < (tmp = read(sfd, ptr, len)))
              {
              len -= tmp;
              ptr += tmp;
              }
            else
              {
              connected = 0;
              close(sfd);
              freetank(&result);
              }
            }
          }
        else
          {
          freetank(&result);
          }
        }
      }
    else
      {
      connected = 0;
      close(sfd);
      }
    }
  return result;
  }


int sendtank(tank *tnk, int sfd)
  {
  char buffer[4];
  int tmp = 0;

  if (NULL != tnk)
    {
    tmp = tnk->length + 4;
    buffer[3] = (char) (tmp % 256);
    tmp /= 256;
    buffer[2] = (char) (tmp % 256);
    tmp /= 256;
    buffer[1] = (char) (tmp % 256);
    tmp /= 256;
    buffer[0] = (char) tmp;

    write(sfd, buffer, 4);
    write(sfd, tnk->data, tnk->length);
    }
  else
    {
    if (debug)
      {
      window_put("Error: NULL ptr given to sendpol");
      window_nl();
      }
    }
  return 0;
  }


void tankdump(tank *part)
  {
  int size;
  int i, j;

  if (NULL != part)
    {
    window_put("Size: 0x");
    window_puthex((unsigned long) part->length, 8);
    window_nl();
    size = part->length;
    for (i = 0; i < size / 16; i++)
      {
      window_put("0x");
      window_puthex(i << 4, 4);
      for (j = 0; j < 16; j++)
        {
        window_putchar(' ');
        window_puthex((unsigned char) part->data[i * 16 + j], 2);
        }
      window_put("  ");
      for (j = 0; j < 16; j++)
        {
        if (isgraph((unsigned char) part->data[i * 16 + j]))
          {
          window_putchar((unsigned char) part->data[i * 16 + j]);
          }
        else
          {
          window_putchar('.');
          }
        }
      window_nl();
      }
  
    window_put("0x");
    window_puthex(i << 4, 4);
    for (j = 0; j < size % 16; j++)
      {
      window_putchar(' ');
      window_puthex((unsigned char) part->data[i * 16 + j], 2);
      }
    for (; j < 16; j++)
      {
      window_put("   ");
      }
    window_put("  ");
    for (j = 0; j < size % 16; j++)
      {
      if (isgraph((unsigned char) part->data[i * 16 + j]))
        {
        window_putchar((unsigned char) part->data[i * 16 + j]);
        }
      else
        {
        window_putchar('.');
        }
      }
    window_nl();
    }
  else
    {
    if (debug)
      {
      window_put("Error: NULL ptr given to tankdump()");
      window_nl();
      }
    }
  }
