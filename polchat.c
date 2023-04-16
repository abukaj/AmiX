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
int connected = 0;
double period = 1.0;
nicknode *nicks = NULL;
char *roomname = NULL;
char *roomdesc = NULL;

int colourt[8] = {COLOR_BLACK, COLOR_BLUE, COLOR_CYAN, COLOR_MAGENTA, COLOR_YELLOW, COLOR_BLACK, COLOR_CYAN, COLOR_BLUE};
int colourop = COLOR_RED;


void addnick(char *nick, short status, short unknown){
  nicknode **nicklist;
  nicknode *tmp;
  int size;
  int res;

  if (unknown != 0x0000){
    if (debug){
      window_put("NICK: ");
      window_put(nick);
      window_put(" unknown: ");
      window_puthex(unknown, 4);
      window_nl();
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
          window_put("Error: unable to allocate 0x");
          window_puthex(size, 4);
          window_put("bytes of memory");
          window_nl();
          }
        free(*nicklist);
        *nicklist = NULL;
        }
      }
    else {
      if (debug){
        window_put("Error: unable to allocate memory for nicknode");
        window_nl();
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
      window_put("Error: no nick to delete");
      window_nl();
      }
    }
  }


void printnicks()
  {
  int i = 1;
  int j;
  int colour;
  nicknode *nicklist;

  nicklist = nicks;
  while (NULL != nicklist && i < nicklist_h - 1)
    {
    colour = colourt[(nicklist->status & 0x0070) >> 4];
    if (colour != COLOR_WHITE && colour != COLOR_BLACK && useattr)
      {
      wattron(nickwindow, COLOR_PAIR(colour) | A_BOLD);
      }
    
    if (nicklist->status & 0x0002)
      {
      mvwprintw(nickwindow, i, 1, "OP ");
      }
    else
      {
      mvwprintw(nickwindow, i, 1, "   ");
      }
    if ((nicklist->status & 0x0001) && useattr)
      {
      wattron(nickwindow, A_UNDERLINE);
      }
    mvwaddnstr(nickwindow, i, 4, nicklist->nick, NICKLIST_WIDTH - 5);
    if (strlen(nicklist->nick) > NICKLIST_WIDTH - 4)
      {
      mvwaddstr(nickwindow, i, NICKLIST_WIDTH - 4, "...");
      }
    if ((nicklist->status & 0x0001) && useattr)
      {
      wattroff(nickwindow, A_UNDERLINE);
      }
    for (j = strlen(nicklist->nick) + 4; j < NICKLIST_WIDTH - 1; j++){
      mvwaddch(nickwindow, i, j, ' ');
      }
    i++;
    nicklist = nicklist->next;
    
    if (colour != COLOR_WHITE && colour != COLOR_BLACK && useattr)
      {
      wattroff(nickwindow, COLOR_PAIR(colour) | A_BOLD);
      }
    
    }
  while (i < nicklist_h - 1){
    wmove(nickwindow, i, 1);
    for (j = 2; j < NICKLIST_WIDTH; j++){
      waddch(nickwindow, ' ');
      }
    i++;
    }
  wnoutrefresh(nickwindow);
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


const char *getnickbyprefix(char *prefix, int len, nicknode *node)
  {
  if (NULL != node)
    {
    if (0 == ncsstrncmp(prefix, node->nick, len))
      {
      return node->nick;
      }
    else
      {
      return getnickbyprefix(prefix, len, node->next);
      }
    }
  return NULL;
  }

