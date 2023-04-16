/*ascii - temp.c - v. 0.3 - Jakub Mateusz Kowalski*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "temp.h"

/*****************************************************************************\
\*****************************************************************************/
void clearscreen(){
    int a;

    for (a = 0; a < 25; a++){
        putchar('\n');
        }

    }


/*****************************************************************************\
\*****************************************************************************/
void mypause(){
    int a;

    a = 0;
    puts("Nacisnij ENTER/RETURN");
    do{
        a = getchar();
        fflush(stdin);
        }
        while (a == 0);

    a = 0;
    }


/*****************************************************************************\
\*****************************************************************************/
int ncsstrcmp(char *cs, char *ct){
  while (*cs != '\0' && tolower(*cs) == tolower(*ct)){
    cs++; ct++;
    }
  return tolower(*cs) - tolower(*ct);
  }


/*****************************************************************************\
\*****************************************************************************/
int ncsstrncmp(char *cs, char *ct, int n){
  if (n == 0)
    return 0;
  while (*cs != '\0' && tolower(*cs) == tolower(*ct) && --n > 0){
    cs++; ct++;
    }
  return tolower(*cs) - tolower(*ct);
  }


/*****************************************************************************\
\*****************************************************************************/
char inttohex(int n){
  switch (n){
    case 1:
      return '1';
    case 2:
      return '2';
    case 3:
      return '3';
    case 4:
      return '4';
    case 5:
      return '5';
    case 6:
      return '6';
    case 7:
      return '7';
    case 8:
      return '8';
    case 9:
      return '9';
    case 0:
      return '0';
    case 10:
      return 'A';
    case 11:
      return 'B';
    case 12:
      return 'C';
    case 13:
      return 'D';
    case 14:
      return 'E';
    case 15:
      return 'F';
    }
  return 0;
  }


/*****************************************************************************\
\*****************************************************************************/
unsigned int iso2unicode(unsigned char c)
  {
  switch (c)
    {
    case 0x00A1:/* A, */
      return 0x0104;
    case 0x00B1:/* a, */
      return 0x0105;
    case 0x00A3:/* L/ */
      return 0x0141;
    case 0x00B3:
      return 0x0142;
    case 0x00A6:/* S' */
      return 0x015A;
    case 0x00B6:
      return 0x015B;
    case 0x00AC: /* Z' */
      return 0x0179;
    case 0x00BC:
      return 0x017A;
    case 0x00AF: /* Z* */
      return 0x017B;
    case 0x00BF:
      return 0x017C;
    case 0x00C6: /* C' */
      return 0x0106;
    case 0x00E6:
      return 0x0107;
    case 0x00CA: /* E, */
      return 0x0118;
    case 0x00EA:
      return 0x0119;
    case 0x00D1: /* N' */
      return 0x0143;
    case 0x00F1:
      return 0x0144;
    default:/* O' */
      return (int) c;
    }
  return 0;
  }


/*****************************************************************************\
\*****************************************************************************/
unsigned char unicode2iso(unsigned int c)
  {
  switch (c)
    {
    case 0x0104:/* A, */
      return 0x00A1;
    case 0x0105:/* a, */
      return 0x00B1;
    case 0x0141:/* L/ */
      return 0x00A3;
    case 0x0142:
      return 0x00B3;
    case 0x015A:/* S' */
      return 0x00A6;
    case 0x015B:
      return 0x00B6;
    case 0x0179: /* Z' */
      return 0x00AC;
    case 0x017A:
      return 0x00BC;
    case 0x017B: /* Z* */
      return 0x00AF;
    case 0x017C:
      return 0x00BF;
    case 0x0106: /* C' */
      return 0x00C6;
    case 0x0107:
      return 0x00E6;
    case 0x0118: /* E, */
      return 0x00CA;
    case 0x0119:
      return 0x00EA;
    case 0x0143: /* N' */
      return 0x00D1;
    case 0x0144:
      return 0x00F1;
    default:/* O' */
      return (int) c;
    }
  return 0;
  }


/*****************************************************************************\
\*****************************************************************************/
unsigned char *iso2utf8string(unsigned char *string)
  {
  unsigned char *result = NULL;
  unsigned char *tmp;
  int src = 0;
  int dst = 0;
  int len = 10;
  unsigned int c;
  
  if (NULL != string)
    {
    if (NULL != (result = calloc(11, sizeof(char))))
      {
      while (result != NULL && string[src] != '\0')
        {
        c = iso2unicode(string[src++]);
        if (dst >= len)
          {
          if (NULL != (tmp = realloc(result, len + 11)))
            {
            len += 10;
            result = tmp;
            }
          else
            {
            free(result);
            result = NULL;
            }
          }
        if (NULL != result)
          {
          if (c < 0x0080)
            {
            result[dst++] = c & 0x00FF;
            }
          else if (c < 0x0800)
            {
            result[dst++] = 0x00C0 | (0x001F & (c >> 6));

            if (dst >= len)
              {
              if (NULL != (tmp = realloc(result, len + 11)))
                {
                len += 10;
                result = tmp;
                }
              else
                {
                free(result);
                result = NULL;
                }
              }
            if (NULL != result)
              {
              result[dst++] = 0x0080 | (0x003F & c); 
              }
            }
          }
        }
      if (NULL != result)
        {
        result[dst] = '\0';
        }
      }
    }
  return result;
  }


/*****************************************************************************\
\*****************************************************************************/
unsigned char *utf82isostring(unsigned char *string)
  {
  unsigned char *result = NULL;
  unsigned char *tmp;
  int src = 0;
  int dst = 0;
  int len = 10;
  unsigned int c;
  
  if (NULL != string)
    {
    if (NULL != (result = calloc(11, sizeof(char))))
      {
      while (result != NULL && string[src] != '\0')
        {
        c = string[src++];
        if (dst >= len)
          {
          if (NULL != (tmp = realloc(result, len + 11)))
            {
            len += 10;
            result = tmp;
            }
          else
            {
            free(result);
            result = NULL;
            }
          }
        if (NULL != result)
          {
          if ((c & 0x0080) == 0x0000)
            {
            result[dst++] = c & 0x00FF;
            }
          else if ((c & 0x00E0) == 0x00C0)
            {
            result[dst++] = unicode2iso(((c & 0x001F) << 6) | (string[src++] & 0x003F));
            }
          }
        }
      if (NULL != result)
        {
        result[dst] = '\0';
        }
      }
    }
  return result;
  }

