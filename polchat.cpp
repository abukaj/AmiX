/*AmiX - polchat.c - v. 0.2 - (c) by ABUKAJ (J.M.Kowalski)*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <ncursesw/ncurses.h>

#include "temp.h"
#include "polchat.h"
#include "interface.h"
#include "version.h"

int cud = 0;
int debug = 0;
int verbose = 0;
int coredump = 0;
int bell = 0;
int run = -1;
int connected = 0;
int askpassw = 0;
int antiidle = 0;
int html = -1;
double period = 1.0;
nicknode *nicks = NULL;
char *roomname = NULL;
char *roomdesc = NULL;
char *pass = NULL;
FILE *logfh = NULL;
short nickn;

int colourt[8] = {COLOR_BLACK, COLOR_BLUE, COLOR_CYAN, COLOR_MAGENTA, COLOR_YELLOW, COLOR_BLACK, COLOR_CYAN, COLOR_BLUE};
int colourop = COLOR_RED;

void addnick(char *nick, char *room, char *client, short status, short unknown)
{
  nicknode **nicklist;
  nicknode *tmp;
  int size;
  int res;

  if (unknown != 0x0000)
  {
    if (debug)
    {
      window_put("NICK: ");
      window_put(nick);
      window_put(" unknown: ");
      window_puthex(unknown, 4);
      window_nl();
    }
  }

  nicklist = &nicks;
  while ((*nicklist) != NULL && (res = ncsstrcmp(nick, (*nicklist)->nick)) > 0)
  {
    nicklist = &((*nicklist)->next);
  }
  if (*nicklist != NULL)
  {
    if (res)
    {
      if (NULL != (tmp = (nicknode *) calloc(1, sizeof(nicknode))))
      {
        tmp->nick = clonestring(nick);
        tmp->client = clonestring(client);
        tmp->status = status;
        tmp->unknown = unknown;
        tmp->next = *nicklist;
        *nicklist = tmp;
      }
    }
    else
    {
      (*nicklist)->status = status;
      (*nicklist)->unknown = unknown;
    }
  }
  else 
  {
    if (NULL != (*nicklist = (nicknode *) calloc(1, sizeof(nicknode))))
    {
      (*nicklist)->nick = clonestring(nick);
      (*nicklist)->client = clonestring(client);
      (*nicklist)->status = status;
      (*nicklist)->unknown = unknown;
    }
    else
    {
      if (debug)
      {
        window_put("Error: unable to allocate memory for nicknode");
        window_nl();
      }
    }
  }
}

void updatenick(char *nick, char *room, short status, short unknown)
{
  nicknode **nicklist;
  nicknode *tmp;
  int size;
  int res;

  if (unknown != 0x0000)
  {
    if (debug)
    {
      window_put("NICK: ");
      window_put(nick);
      window_put(" unknown: ");
      window_puthex(unknown, 4);
      window_nl();
    }
  }

  nicklist = &nicks;
  while ((*nicklist) != NULL && (res = ncsstrcmp(nick, (*nicklist)->nick)) > 0)
  {
    nicklist = &((*nicklist)->next);
  }
  if (*nicklist != NULL)
  {
    if (res == 0)
    {
      (*nicklist)->status = status;
      (*nicklist)->unknown = unknown;
    }
  }
}


void remnick(char *nick, char *room)
{
  nicknode **nicklist;
  nicknode *tmp;

  nicklist = &nicks;
  while ((*nicklist) != NULL && 0 != ncsstrcmp(nick, (*nicklist)->nick))
  {
    nicklist = &((*nicklist)->next);
  }
  if (*nicklist != NULL)
  {
    tmp = *nicklist;
    *nicklist = (*nicklist)->next;
    if (NULL != tmp->nick)
    {
      free(tmp->nick);
    }
    if (NULL != tmp->client)
    {
      free(tmp->client);
    }
    free(tmp);
  }
  else
  {
    if (debug)
    {
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

    for (j = strlen(nicklist->nick) + 4; j < NICKLIST_WIDTH - 1; j++)
    {
      mvwaddch(nickwindow, i, j, ' ');
    }
    i++;
    nicklist = nicklist->next;
    
    if (colour != COLOR_WHITE && colour != COLOR_BLACK && useattr)
    {
      wattroff(nickwindow, COLOR_PAIR(colour) | A_BOLD);
    }  
  }

  while (i < nicklist_h - 1)
  {
    wmove(nickwindow, i, 1);
    for (j = 2; j < NICKLIST_WIDTH; j++)
    {
      waddch(nickwindow, ' ');
    }
    i++;
  }
  wnoutrefresh(nickwindow);
  window_updated = -1;
}


void freenicklist(nicknode **nicklist)
{
  if (*nicklist != NULL)
  {
    freenicklist(&((*nicklist)->next));
    if (NULL != (*nicklist)->nick)
    {
      free((*nicklist)->nick);
    }
    if (NULL != (*nicklist)->client)
    {
      free((*nicklist)->client);
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


int openlog(char *logfn)
{
  closelog();
  logfh = fopen(logfn, "w");
  if (logfh != NULL)
  {
    if (html)
    {
      fputs("<table>\n <caption>\n  Log generated by " VER "\n </caption>\n", logfh);
      fputs(" <tr>\n  <th>\n   time\n  </th>\n  <th>\n   priv\n  </th>\n  <th>\n   message\n  </th>\n </tr>\n", logfh);
    }
    else
    {
      fputs("Log generated by " VER "\n", logfh);
    }
    return -1;
  }
  else
  {
    return 0;
  }
}


int openoldlog(char *logfn)
{
  closelog();
  logfh = fopen(logfn, "a");
  if (logfh != NULL)
  {
    return -1;
  }
  else
  {
    return 0;
  }
}



void closelog()
{
  if (logfh != NULL)
  {
    if (html)
    {
      fputs("</table>\n", logfh);
    }
    fclose(logfh);
    logfh = NULL;
  }
}


int printlog(char *descr, char *msg)
{
  time_t t;
  static char buffer[15];
  if (logfh != NULL)
  {
    t = time(NULL);
    strftime(buffer, 14, "%H:%M:%S ", localtime(&t));
    if (html)
    {
      fputs(" <tr>\n  <td>\n   ", logfh);
      fputs(buffer, logfh);
      fputs("\n  </td>\n  <td>\n   ", logfh);
      fputs(descr, logfh);
      fputs("\n  </td>\n  <td>\n   ", logfh);
      fputs(msg, logfh);
      fputs("\n  </td>\n </tr>\n", logfh);
    }
    else
    {
      fputs(buffer, logfh);
      fputs(" ", logfh);
      fputs(descr, logfh);
      fputs(" ", logfh);
      fputs(msg, logfh);
      fputs("\n", logfh);
    }
  }
  return -1; /*nie chce mi sie sprawdzac czy nie bylo EOF*/
}
