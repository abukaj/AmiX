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

part *tosend = NULL;
time_t last = 0;

char *wrapstring(char *string){
  char *result = NULL;
  char *tmp;
  int len;

  if (NULL != (tmp = iso2utf8string(string)))
    {
    len = strlen(tmp);
    if (NULL != (result = calloc(len + 3, sizeof(char)))){
      result[0] = (char) (len / 256);
      result[1] = (char) (len % 256);
      strncpy(result + 2, tmp, len);
      }
    free(tmp);
    }
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
    result = utf82isostring(tmp);
    free(tmp);
    }
  return result;
  }


int partlen(char *part){
  return (((unsigned char) part[0] * 256 + (unsigned char) part[1]) * 256
              + (unsigned char) part[2]) * 256 + (unsigned char) part[3];
  }

char *readpart(int sfd){
  char buffer[4];
  char *result = NULL;
  int len, tmp, ptr = 0;

  if (connected)
    {
    if (0 < read(sfd, buffer, 4))
      {
      if (NULL != (result = calloc(partlen(buffer), sizeof(char)))){
        result[0] = buffer[0];
        result[1] = buffer[1];
        result[2] = buffer[2];
        result[3] = buffer[3];
        len = partlen(buffer) - 4;
        while (len)
          {
          if (0 < (tmp = read(sfd, result + 4 + ptr, len)))
            {
            len -= tmp;
            ptr += tmp;
            }
          else
            {
            connected = 0;
            close(sfd);
            free(result);
            result = NULL;
            }
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


part *parsepart(char *prt)
  {
  int size;
  int ptr;
  int i;
  part *result = NULL;

  if (NULL != prt)
    {
    if (NULL != (result = calloc(1, sizeof(part))))
      {
      result->next = NULL;
      size = partlen(prt);
      ptr = 4;

      result->headerlen = ((unsigned char) prt[ptr++]) << 8;
      result->headerlen |= (unsigned char) prt[ptr++];
      result->nstrings = ((unsigned char) prt[ptr++]) << 8;
      result->nstrings |= (unsigned char) prt[ptr++];
      if (NULL != (result->header = calloc(result->headerlen, sizeof(short))))
        {
        for (i = 0; i < result->headerlen; i++)
          {
          result->header[i] = ((unsigned char) prt[ptr++]) << 8;
          result->header[i] |= (unsigned char) prt[ptr++];
          }
        if (NULL != (result->strings = calloc(result->nstrings, sizeof(char *))))
          {
          for (i = 0; i < result->nstrings; i++)
            {
            result->strings[i] = NULL;
            }
          
          for (i = 0; i < result->nstrings; i++)
            {
            if (NULL == (result->strings[i] = unwrapstring(prt + ptr)))
              {
              freepart(&result);
              return NULL;
              }
            ptr += wrapsize(prt + ptr);
            }
          }
        else
          {
          free(result->header);
          free(result);
          return NULL;
          }
        }
      else
        {
        free(result);
        return NULL;
        }
      }
    if (ptr != size)
      {
      if (debug)
        {
        window_put("Part parse error!");
        partdump(prt);
        }
      }
    }
  else
    {
    if (debug)
      {
      window_put("Error: NULL ptr given to parsepart()");
      window_nl();
      }
    }
  return result;
  }


void freepart(part **p)
  {
  int i;
  
  if (*p != NULL)
    {
    freepart(&((*p)->next));
    if ((*p)->header != NULL)
      {
      free((*p)->header);
      }
    if ((*p)->strings != NULL)
      {
      for (i = 0; i < (*p)->nstrings; i++)
        {
        if ((*p)->strings[i] != NULL)
          {
          free((*p)->strings[i]);
          }
        }
      free((*p)->strings);
      }
    free(*p);
    *p = NULL;
    }
  }


int sendpol(part *ppart, int sfd){
  char *result;
  int size = 8;
  int i;
  int tmp = 0;
  int ptr = 0;
  
  if (NULL != ppart){
    size += 2 * ppart->headerlen;
    for (i = 0; i < ppart->nstrings; i++){
      size += strlen(ppart->strings[i]) + 3;
      }
    if (NULL != (result = calloc(size, sizeof(char)))){
      tmp = size;
      result[3] = (char) (tmp % 256);
      tmp /= 256;
      result[2] = (char) (tmp % 256);
      tmp /= 256;
      result[1] = (char) (tmp % 256);
      tmp /= 256;
      result[0] = (char) tmp;
      ptr += 4;

      result[ptr++] = (char) (ppart->headerlen / 256);
      result[ptr++] = (char) (ppart->headerlen % 256);
      result[ptr++] = (char) (ppart->nstrings / 256);
      result[ptr++] = (char) (ppart->nstrings % 256);
   
      for (i = 0; i < ppart->headerlen; i++){
        result[ptr++] = (char) (ppart->header[i] / 256);
        result[ptr++] = (char) (ppart->header[i] % 256);
        }
      for (i = 0; i < ppart->nstrings; i++){
        tmp = strlen(ppart->strings[i]);
        result[ptr++] = (char) (tmp / 256);
        result[ptr++] = (char) (tmp % 256);
        strcpy(result + ptr, ppart->strings[i]);
        ptr += tmp;
        result[ptr++] = '\0';
        }

      write(sfd, result, size);
      if (ptr != size){
        if (debug){
          window_put("Error: ptr != size");
          window_nl();
          }
        }
      }
    else{
      if (debug){
        window_put("Error: unable to allocate 0x");
        window_puthex(size, 4);
        window_put("bytes of memory");
        window_nl();
        }
      }
    }
  else{
    if (debug){
      window_put("Error: NULL ptr given to sendpol");
      window_nl();
      }
    }
  return 0;
  }
        

void putmsg(part *msg){
  part **tmp;
  
  tmp = &tosend;
  while (*tmp != NULL)
    {
    tmp = &((*tmp)->next);
    }
  *tmp = msg;
  }


void sendnext(int sfd)
  {
  part *tmp;

  if (last == 0)
    {
    last = time(NULL);
    }
  if (NULL != tosend)
    {
    if (connected && (period < difftime(time(NULL), last))){
      sendpol(tosend, sfd);
      tmp = tosend->next;
      tosend->next = NULL;
      freepart(&tosend);
      tosend = tmp;
      last = time(NULL);
      }
    }
  else
    {
    if (antiidle)
      {
      if (connected && (antiidle < difftime(time(NULL), last)))
        {
        if (NULL != (tmp = makemsg("")))
          {
          sendpol(tmp, sfd);
          freepart(&tmp);
          last = time(NULL);
          }
        }
      }
    }
  }


void partdump(char *part){
  int size;
  int i, j;

  if (NULL != part){
    size = partlen(part);
    for (i = 0; i < size / 16; i++){
      window_put("0x");
      window_puthex(i, 4);
      for (j = 0; j < 16; j++){
        window_putchar(' ');
        window_puthex((unsigned char) part[i * 16 + j], 2);
        }
      window_put("  ");
      for (j = 0; j < 16; j++){
        if (isgraph((unsigned char) part[i * 16 + j])){
          window_putchar((unsigned char) part[i * 16 + j]);
          }
        else {
          window_putchar('.');
          }
        }
      window_nl();
      }
  
    window_put("0x");
    window_puthex(i, 4);
    for (j = 0; j < size % 16; j++){
      window_putchar(' ');
      window_puthex((unsigned char) part[i * 16 + j], 2);
      }
    for (; j < 16; j++){
      window_put("   ");
      }
    window_put("  ");
    for (j = 0; j < size % 16; j++){
      if (isgraph((unsigned char) part[i * 16 + j])){
        window_putchar((unsigned char) part[i * 16 + j]);
        }
      else {
        window_putchar('.');
        }
      }
    window_nl();
    }
  else{
    if (debug){
      window_put("Error: NULL ptr given to partdump()");
      window_nl();
      }
    }
  }
