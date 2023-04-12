/*AmiX - polchat.c - v. 0.1 - (c) by ABUKAJ (J.M.Kowalski)*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "polchat.h"
#include "polchat1.h"
#include "polchat2.h"
#include "temp.h"

int read(int, char *, int);
int write(int, char *, int);

unsigned char *wrapstring(unsigned char *string){
  unsigned char *result = NULL;
  int len;

  len = strlen(string);
  if (NULL != (result = calloc(len + 3, sizeof(char)))){
    result[0] = len / 256;
    result[1] = len % 256;
    strcpy(result + 2, string);
    };
  return result;
  }


int wrapsize(unsigned char * string){
  return string[0] * 256 + string[1] + 3;
  }


unsigned char *unwrapstring(unsigned char *string){
  unsigned char *result = NULL;
  int len;

  len = string[0] * 256 + string[1];
  if (NULL != (result = calloc(len + 1, sizeof(char)))){
    strcpy(result, string + 2);
    }
  return result;
  }


int partlen(unsigned char *part){
  return ((part[0] * 256 + part[1]) * 256 + part[2]) * 256 + part[3];
  }

unsigned char *readpart(int sfd){
  unsigned char buffer[4];
  unsigned char *result = NULL;

  read(sfd, buffer, 4);
  if (NULL != (result = calloc(partlen(buffer), sizeof(char)))){
    result[0] = buffer[0];
    result[1] = buffer[1];
    result[2] = buffer[2];
    result[3] = buffer[3];
    read(sfd, result + 4, partlen(buffer) - 4);
    }
  return result;
  }


part *parsepart(unsigned char *prt){
  int size;
  int ptr;
  int i;
  part *result = NULL;

  if (NULL != prt){
    if (NULL != (result = calloc(1, sizeof(part)))){
      size = partlen(prt);
      ptr = 4;

      result->headerlen = prt[ptr++] << 8;
      result->headerlen |= prt[ptr++];
      result->nstrings = prt[ptr++] << 8;
      result->nstrings |= prt[ptr++];
      if (NULL != (result->header = calloc(result->headerlen, sizeof(short)))){
        for (i = 0; i < result->headerlen; i++){
          result->header[i] = prt[ptr++] << 8;
          result->header[i] |= prt[ptr++];
          }
        if (NULL != (result->strings = calloc(result->nstrings, sizeof(char *)))){
          for (i = 0; i < result->nstrings; i++){
            if (NULL == (result->strings[i] = unwrapstring(prt + ptr))){
              freepart(&result);
              return NULL;
              }
            ptr += wrapsize(prt + ptr);
            }
          }
        else{
          free(result->header);
          free(result);
          return NULL;
          }
        }
      else{
        free(result);
        return NULL;
        }
      }
    if (ptr != size){
      if (debug){
        puts("Part parse error!");
        partdump(prt);
        }
      }
    }
  else{
    if (debug){
      puts("Error: NULL ptr given to parsepart()");
      }
    }
  return result;
  }


void freepart(part **p){
  int i;
  
  if (*p != NULL){
    if ((*p)->header != NULL){
      free((*p)->header);
      }
    if ((*p)->strings != NULL){
      for (i = 0; i < (*p)->nstrings; i++){
        if ((*p)->strings[i] != NULL){
          free((*p)->strings[i]);
          }
        }
      free((*p)->strings);
      }
    free(*p);
    *p = NULL;
    }
  }


int sendpol(part * ppart, int sfd){
  unsigned char *result;
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
      result[3] = tmp % 256;
      tmp /= 256;
      result[2] = tmp % 256;
      tmp /= 256;
      result[1] = tmp % 256;
      tmp /= 256;
      result[0] = tmp;
      ptr += 4;

      result[ptr++] = ppart->headerlen / 256;
      result[ptr++] = ppart->headerlen % 256;
      result[ptr++] = ppart->nstrings / 256;
      result[ptr++] = ppart->nstrings % 256;
   
      for (i = 0; i < ppart->headerlen; i++){
        result[ptr++] = ppart->header[i] / 256;
        result[ptr++] = ppart->header[i] % 256;
        }
      for (i = 0; i < ppart->nstrings; i++){
        tmp = strlen(ppart->strings[i]);
        result[ptr++] = tmp / 256;
        result[ptr++] = tmp % 256;
        strcpy(result + ptr, ppart->strings[i]);
        ptr += tmp;
        result[ptr++] = '\0';
        }

      write(sfd, result, size);
      if (ptr != size){
        if (debug){
          printf("Error: ptr (%d) != size (%d)\n", ptr, size);
          }
        }
      }
    else{
      if (debug){
        printf("Error: unable to allocate %d bytes of memory\n", size);
        }
      }
    }
  else{
    if (debug){
      puts("Error: NULL ptr given to sendpol");
      }
    }
  return 0;
  }
        

void partdump(unsigned char *part){
  int size;
  int i, j;

  if (NULL != part){
    size = partlen(part);
    for (i = 0; i < size / 16; i++){
      printf("0x%04X: ", i);
      for (j = 0; j < 16; j++){
        printf(" %02X", part[i * 16 + j]);
        }
      printf("  ");
      for (j = 0; j < 16; j++){
        if (isgraph(part[i * 16 + j])){
          putchar(part[i * 16 + j]);
          }
        else {
          putchar('.');
          }
        }
      putchar('\n');
      }
  
    printf("0x%04X: ", i);
    for (j = 0; j < size % 16; j++){
      printf(" %02X", part[i * 16 + j]);
      }
    for (; j < 16; j++){
      printf("   ");
      }
    printf("  ");
    for (j = 0; j < size % 16; j++){
      if (isgraph(part[i * 16 + j])){
        putchar(part[i * 16 + j]);
        }
      else {
        putchar('.');
        }
      }
    putchar('\n');
    }
  else{
    if (debug){
      puts("Error: NULL ptr given to partdump()");
      }
    }
  }
