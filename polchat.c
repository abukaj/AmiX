/*AmiX - polchat.c - v. 0.2 - (c) by ABUKAJ (J.M.Kowalski)*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <ncurses.h>

#include "temp.h"
#include "polchat.h"
#include "interfeace.h"

int debug = 0;
int verbose = 0;
int bell = 0;
int run = -1;
double period = 1.0;
nicknode *nicks = NULL;
int colour[8] = {90, 34, 96, 95, 93, 90, 36, 94};
char *roomname = NULL;
char *roomdesc = NULL;

void addnick(char *nick, short status, short unknown){
  nicknode **nicklist;
  nicknode *tmp;
  int size;
  int res;

  if (unknown != 0x0000){
    if (debug){
      printw("NICK: %s unknown: %04X\n", nick, unknown);
      }
    }

  nicklist = &nicks;
  while ((*nicklist) != NULL && (res = ncsstrcmp(nick, (*nicklist)->nick)) > 0){
    nicklist = &((*nicklist)->next);
    }
  if (*nicklist != NULL){
    if (res){
      if (NULL != (tmp = calloc(1, sizeof(nicknode)))){
        if (NULL != (tmp->nick = calloc(strlen(nick) + 1, sizeof(char)))){
          strcpy(tmp->nick, nick);
          tmp->status = status;
          tmp->unknown = unknown;
          tmp->next = *nicklist;
          *nicklist = tmp;
          }
        else{
          free(tmp);
          }
        }
      }
    else{
      (*nicklist)->status = status;
      (*nicklist)->unknown = unknown;
      }
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
          printw("Error: unable to allocate %d bytes of memory\n", size);
          }
        free(*nicklist);
        *nicklist = NULL;
        }
      }
    else {
      if (debug){
        printw("Error: unable to allocate memory for nicknode\n");
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
      printw("Error: no nick to delete\n");
      }
    }
  }


void printnicks(nicknode *nicklist){
  int i = 1;
  int j;
  while (NULL != nicklist && i <= NICKLIST_HEIGHT){
    if ((nicklist->status & 0xff8c) != 0x0000 || nicklist->unknown != 0x0000){
      if (debug){
        mvwprintw(nickwindow, i, 1, "%04X %04X", nicklist->status, nicklist->unknown);
        }
      else{
        mvwprintw(nickwindow, i, 1, "         ");
        }
      }
    else {
      mvwprintw(nickwindow, i, 1, "         ");
      }
    
    /*mvwprintw(nickwindow, i++, 10, "\033[%um", colour[(nicklist->status & 0x0070) >> 4]);*/
    if (nicklist->status & 0x0002){
      mvwprintw(nickwindow, i, 10, "OP ");
      }
    else{
      mvwprintw(nickwindow, i, 10, "   ");
      }
    if (nicklist->status & 0x0001){
      wattron(nickwindow, A_UNDERLINE);
      }
    mvwprintw(nickwindow, i, 13, nicklist->nick);
    wattroff(nickwindow, A_UNDERLINE);
    for (j = strlen(nicklist->nick) + 13; j < NICKLIST_WIDTH - 1; j++){
      mvwaddch(nickwindow, i, j, ' ');
      }
    i++;
    nicklist = nicklist->next;
    }
  while (i < NICKLIST_HEIGHT - 1){
    wmove(nickwindow, i, 1);
    for (j = 2; j < NICKLIST_WIDTH; j++){
      waddch(nickwindow, ' ');
      }
    i++;
    }
  wrefresh(nickwindow);
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


