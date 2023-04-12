/*AmiX - polchat.c - v. 0.2 - (c) by ABUKAJ (J.M.Kowalski)*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "temp.h"
#include "polchat.h"

int debug = 0;
int verbose = 0;
int bell = 0;
int run = -1;
nicknode *nicks = NULL;
int colour[8] = {90, 34, 96, 95, 93, 90, 36, 94};

void addnick(char *nick, short status, short unknown){
  nicknode **nicklist;
  int size;

  if (unknown != 0x0000){
    if (debug){
      printf("NICK: %s unknown: %04X\n", nick, unknown);
      }
    }

  nicklist = &nicks;
  while ((*nicklist) != NULL && 0 != ncsstrcmp(nick, (*nicklist)->nick)){
    nicklist = &((*nicklist)->next);
    }
  if (*nicklist != NULL){
    (*nicklist)->status = status;
    (*nicklist)->unknown = unknown;
    }
  else {
    if (NULL != (*nicklist = calloc(1, sizeof(nicknode)))){
      if (NULL != ((*nicklist)->nick = calloc(strlen(nick) + 1, sizeof(char)))){
        strcpy((*nicklist)->nick, nick);
        (*nicklist)->status = status;
        (*nicklist)->unknown = unknown;
        }
      else {
        if (debug){
          size = strlen(nick) + 1;
          printf("Error: unable to allocate %d bytes of memory\n", size);
          }
        free(*nicklist);
        *nicklist = NULL;
        }
      }
    else {
      if (debug){
        puts("Error: unable to allocate memory for nicknode");
        }
      }
    }
  }


void remnick(char *nick){
  nicknode **nicklist;
  nicknode *tmp;

  nicklist = &nicks;
  while ((*nicklist) != NULL && 0 != ncsstrcmp(nick, (*nicklist)->nick)){
    nicklist = &((*nicklist)->next);
    }
  if (*nicklist != NULL){
    tmp = *nicklist;
    *nicklist = (*nicklist)->next;
    if (NULL != tmp->nick){
      free(tmp->nick);
      }
    free(tmp);
    }
  else {
    if (debug) {
      puts("Error: no nick to delete");
      }
    }
  }


void printnicks(nicknode *nicklist){
  /*printf("\033[1;2H");*/
  while (NULL != nicklist){
    if ((nicklist->status & 0xff8c) != 0x0000 || nicklist->unknown != 0x0000){
      if (debug){
        printf("%04X %04X", nicklist->status, nicklist->unknown);
        }
      else{
        printf("         ");
        }
      }
    else {
      printf("         ");
      }
    printf("\033[%um", colour[(nicklist->status & 0x0070) >> 4]);
    if (nicklist->status & 0x0002){
      printf("OP ");
      }
    else{
      printf("   ");
      }
    if (nicklist->status & 0x0001){
      printf("\033[4m");
      }
    printf("%s\033[0m\n", nicklist->nick);
    nicklist = nicklist->next;
    }
  }


void freenicklist(nicknode **nicklist){
  if (*nicklist != NULL){
    freenicklist(&((*nicklist)->next));
    if (NULL != (*nicklist)->nick){
      free((*nicklist)->nick);
      }
    free(*nicklist);
    *nicklist = NULL;
    }
  }


