/*ascii - temp.c - v. 0.22 - Jakub Mateusz Kowalski*/
#include <stdio.h>
#include <ctype.h>
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

