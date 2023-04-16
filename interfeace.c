#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "interfeace.h"
#include "polchat.h"
#include "polchat2.h" 
/*przeniesc printpol*/
#include "temp.h"

line window[MSGSTOREMAX];
WINDOW *chatwindow = NULL;
WINDOW *nickwindow = NULL;
WINDOW *titlewindow = NULL;
WINDOW *consolewindow = NULL;
static int inlen = 0;

int nicklist_x;
int nicklist_h;
int console_y;
int console_w;
int title_w;

int scr_rows;
int scr_cols;
int window_h;
int window_w;


/*void remsn(stringnode **wezel){
  if (*wezel != NULL){
    remsn(&((*wezel)->next));
    if ((*wezel)->string != NULL){
      free((*wezel)->string);
      }
    free(*wezel);
    *wezel = NULL;
    }
  }


stringnode *newsn(char *str){
  stringnode *nsn = NULL;
  int len;

  if (NULL != (nsn = calloc(1, sizeof(stringnode)))){
    if (NULL != (nsn->string = calloc(1 + (len = strlen(str)), sizeof(char)))){
      strcpy(nsn->string, str);
      nsn->len = len;
      nsn->next = NULL;
      }
    else {
      free(nsn);
      nsn = NULL;
      }
    }
  return nsn;
  }


void addstringnode(stringnode **wezel, char *str){
  if (*wezel != NULL){
    addstringnode(&((*wezel)->next), str);
    }
  else {
    *wezel = newsn(str);
    }
  }
*/

void window_init(){
  int i;

  for (i = 0; i < MSGSTOREMAX; ++i)
    {
    window->string = NULL;
    }
  
  getmaxyx(stdscr, scr_rows, scr_cols);
  window_h = scr_rows - WINDOW_Y - CONSOLE_H;
  window_w = scr_cols - NICKLIST_WIDTH - WINDOW_X;

  chatwindow = newwin(window_h, window_w, WINDOW_Y, WINDOW_X);
  wborder(chatwindow, ' ', ' ', '#', '#', '*', '*', '*', '*');
  mvwaddstr(chatwindow, 0, window_w / 2 - 3, " CHAT ");
  scrollok(chatwindow, TRUE);
  wsetscrreg(chatwindow, 1, window_h - 2);
  wrefresh(chatwindow);
  
  
  
  nicklist_x = scr_cols - NICKLIST_WIDTH;
  nicklist_h = scr_rows - NICKLIST_Y;
  nickwindow = newwin(nicklist_h, NICKLIST_WIDTH, NICKLIST_Y, nicklist_x);
  wborder(nickwindow, '|', '|', '-', '-', '+', '+', '+', '+');
  mvwaddstr(nickwindow, 0, NICKLIST_WIDTH / 2 - 4, " NICKS: ");
  wrefresh(nickwindow);

  
  title_w = scr_cols - NICKLIST_WIDTH - TITLE_X;
  titlewindow = newwin(TITLE_H, title_w, TITLE_Y, TITLE_X);
  wborder(titlewindow, '|', '|', '-', '-', '/', '\\', '\\', '/');
  wrefresh(titlewindow);

  console_y = scr_rows - CONSOLE_H;
  console_w = scr_cols - CONSOLE_X - NICKLIST_WIDTH;
  consolewindow = newwin(CONSOLE_H, console_w, console_y, CONSOLE_X);
  wborder(consolewindow, '|', '|', '-', '-', '+', '+', '+', '+');
  wrefresh(consolewindow);
  }


void window_resize()
  {
  getmaxyx(stdscr, scr_rows, scr_cols);
  
  window_h = scr_rows - WINDOW_Y - CONSOLE_H;
  window_w = scr_cols - NICKLIST_WIDTH - WINDOW_X;
  mvwin(chatwindow, WINDOW_Y, WINDOW_X);
  wresize(chatwindow, window_h, window_w);
  wborder(chatwindow, ' ', ' ', '#', '#', '*', '*', '*', '*');
  mvwaddstr(chatwindow, 0, window_w / 2 - 3, " CHAT ");
  wsetscrreg(chatwindow, 1, window_h - 2);
  window_redraw();
  wnoutrefresh(chatwindow);
  
  nicklist_x = scr_cols - NICKLIST_WIDTH;
  nicklist_h = scr_rows - NICKLIST_Y;
  mvwin(nickwindow, NICKLIST_Y, nicklist_x);
  wresize(nickwindow, nicklist_h, NICKLIST_WIDTH);
  wborder(nickwindow, '|', '|', '-', '-', '+', '+', '+', '+');
  mvwaddstr(nickwindow, 0, NICKLIST_WIDTH / 2 - 4, " NICKS: ");
  printnicks();
  wnoutrefresh(nickwindow);

  title_w = scr_cols - NICKLIST_WIDTH - TITLE_X;
  mvwin(titlewindow, TITLE_Y, TITLE_X);
  wresize(titlewindow, TITLE_H, title_w);
  wborder(titlewindow, '|', '|', '-', '-', '/', '\\', '\\', '/');
  printtitle();
  wnoutrefresh(titlewindow);

  console_y = scr_rows - CONSOLE_H;
  console_w = scr_cols - CONSOLE_X - NICKLIST_WIDTH;
  mvwin(consolewindow, console_y, CONSOLE_X);
  wresize(consolewindow, CONSOLE_H, console_w);
  wborder(consolewindow, '|', '|', '-', '-', '+', '+', '+', '+');  
  wnoutrefresh(consolewindow);
  }


void window_redraw()
  {
  int i, j;
  static char buffer[15];

  wmove(chatwindow, 1, 0);
  for (i = 0; i < MSGSTOREMAX; ++i)
    {
    if (NULL != window[i].string)
      {
      strftime(buffer, 14, "%H:%M:%S ", localtime(&(window[i].time)));
      window_put(buffer);
      printpol(window[i].string);
      }
    else
      {
      for (j = 0; j < window_w; ++j)
        {
        waddch(chatwindow, ' ');
        }
      waddch(chatwindow, '\n');
      }
    }
  }


void window_addstr(char *string){
  int i;
  static char buffer[15];
  
  if (NULL != string)
    {
    if (NULL != window[0].string)
      {
      free(window[0].string);
      }
    for (i = 0; i < MSGSTOREMAX - 1; ++i)
      {
      window[i] = window[i + 1];
      }
    if (NULL != (window[MSGSTOREMAX - 1].string = calloc(strlen(string) + 1, sizeof(char))))
      {
      strcpy(window[MSGSTOREMAX - 1].string, string);
      window[MSGSTOREMAX - 1].time = time(NULL);
      strftime(buffer, 14, "%H:%M:%S ", localtime(&(window[MSGSTOREMAX - 1].time)));
      window_put(buffer);
      printpol(window[MSGSTOREMAX - 1].string);
      }
    }
  }


void window_done()
  {
  int i;

  delwin(chatwindow);
  delwin(nickwindow);
  delwin(consolewindow);
  delwin(titlewindow);
  
  for (i = 0; i < MSGSTOREMAX; i++)
    {
    if (NULL != window[i].string)
      {
      free(window[i].string);
      window[i].string = NULL;
      }
    }
  }


void window_nl(){
  while (inlen < window_w - 2)
    {
    waddch(chatwindow, ' ');
    inlen++;
    }
  waddch(chatwindow, '\n');
  inlen = 0;
  }


void window_put(char *word){
  char *tmp;
  int len;
  int ptr;
  
  if (word != NULL && *word != '\0'){
    if (*word == '\n')
      {
      window_nl();
      }
    else if (verbose || (*word != '<'))
      {
      len = strlen(word);
      if (inlen + len <= window_w - 2)
        {
        waddstr(chatwindow, word);
        inlen += len;
        }
      else 
        {
        window_nl();
        inlen = window_w - 2;
        ptr = 0;
        if (NULL != (tmp = calloc(inlen + 1, sizeof(char))))
          {
          while (len > inlen)
            {
            strncpy(tmp, (word + ptr), inlen);
            waddstr(chatwindow, tmp);
            waddch(chatwindow, '\n');
            ptr += inlen;
            len -= inlen;
            }
          free(tmp);
          }

        waddstr(chatwindow, (word + ptr));
        inlen = len;
        }
      }
    else if (0 == ncsstrcmp(word, "<b>"))
      {
      wattron(chatwindow, A_BOLD);
      }
    else if (0 == ncsstrcmp(word, "</b>"))
      {
      wattroff(chatwindow, A_BOLD);
      }
    else if (0 == ncsstrcmp(word, "<u>"))
      {
      wattron(chatwindow, A_UNDERLINE);
      }
    else if (0 == ncsstrcmp(word, "</u>"))
      {
      wattroff(chatwindow, A_UNDERLINE);
      }
    else if (0 == ncsstrcmp(word, "<blink>"))
      {
      wattron(chatwindow, A_BLINK);
      }
    else if (0 == ncsstrcmp(word, "</blink>"))
      {
      wattroff(chatwindow, A_BLINK);
      }
    else
      {
      len = strlen(word);
      if (inlen + len <= window_w - 2)
        {
        waddstr(chatwindow, word);
        inlen += len;
        }
      else 
        {
        waddch(chatwindow, '\n');
        inlen = window_w - 2;
        ptr = 0;
        if (NULL != (tmp = calloc(inlen + 1, sizeof(char))))
          {
          while (len > inlen)
            {
            strncpy(tmp, (word + ptr), inlen);
            waddstr(chatwindow, tmp);
            waddch(chatwindow, '\n');
            ptr += inlen;
            len -= inlen;
            }
          free(tmp);
          }
        waddstr(chatwindow, (word + ptr));
        inlen = len;
        }
      }
    }
  }


void window_putforce(char *word){
  waddstr(chatwindow, word);
  }


void window_print(){
  wnoutrefresh(chatwindow);
  }


char *readtoken(char *string){
  static int ptr = 0;
  static int done = 0;
  static int link = 0;
  int mode = 0;
  int len = 0;
  char c;
  char *result = NULL;
  char *tmp;


  if (done){
    ptr = 0;
    done = 0;
    }
  else{
    if (NULL != (result = calloc(1, sizeof(char)))){
      *result = '\0';
      while (mode != 1)
        {
        c = string[ptr++];
        switch (mode){
          case 0:
            switch (c)
              {
              case '\0':
                done = -1;
                mode = 1;
                break;
              case '<':
                mode = 2;
                if (NULL != (tmp = realloc(result, len + 2)))
                  {
                  result = tmp;
                  result[len++] = c;
                  result[len] = '\0';
                  }
                else 
                  {
                  printw("ERROR: unable to allocate enogh memory for letter: %c\n", c);
                  }
                break;
              case ' ':
              case '\n':
              case '\t':
              case '\r':
              case '\f':
              case '\b':
              case '\a':
                break;
              case '&':
                if (link)
                  {
                  mode = 4;
                  }
                else  
                  {
                  mode = 3;
                  }
                if (NULL != (tmp = realloc(result, len + 2))){
                  result = tmp;
                  result[len++] = c;
                  result[len] = '\0';
                  }
                else 
                  {
                  printw("ERROR: unable to allocate enogh memory for letter: %c\n", c);
                  }
                break;
              default:
                mode = 4;
                if (NULL != (tmp = realloc(result, len + 2)))
                  {
                  result = tmp;
                  result[len++] = c;
                  result[len] = '\0';
                  }
                else 
                  {
                  printw("ERROR: unable to allocate enogh memory for letter: %c\n", c);
                  }
                break;
              }
            break;
          case 2:
            if (NULL != (tmp = realloc(result, len + 2)))
              {
              result = tmp;
              result[len++] = c;
              result[len] = '\0';
              }
            else {
              printw("ERROR: unable to allocate enogh memory for letter: %c\n", c);
              }
            if (c == '>')
              {
              if (0 == ncsstrncmp(result, "<a", 2))
                {
                link = -1;  
                }
              if (0 == ncsstrcmp(result, "</a>"))
                {
                link = 0;
                }
              mode = 1;
              }
            if (c == '\0')
              {
              mode = 1;
              done = -1;
              }
            break;
          case 3:
            if (NULL != (tmp = realloc(result, len + 2)))
              {
              result = tmp;
              result[len++] = c;
              result[len] = '\0';
              }
            else 
              {
              printw("ERROR: unable to allocate enogh memory for letter: %c\n", c);
              }
            if (c == ';')
              {
              mode = 1;
              }
            if (c == '\0')
              {
              mode = 1;
              done = -1;
              }
            break;
          case 4:
            if (c == '<' || c == '&')
              {
              mode = 1;
              ptr--;
              }
            else 
              {
              if (NULL != (tmp = realloc(result, len + 2)))
                {
                result = tmp;
                result[len++] = c;
                result[len] = '\0';
                }
              else 
                {
                printw("ERROR: unable to allocate enogh memory for letter: %c\n", c);
                }
              if (isspace(c))
                {
                mode = 1;
                }
              if (c == '\0')
                {
                mode = 1;
                done = -1;
                }
              }
            break;
          }
        }
      }
    }
 
  return result;
  }


void printtitle()
  {
  int i;

  for (i = 1; i < title_w - 1; i++)
    {
    mvwaddch(titlewindow, 1, i, ' ');
    mvwaddch(titlewindow, 2, i, ' ');
    }
  if (NULL != roomname)
    {
    mvwaddnstr(titlewindow, 1, 1, roomname, title_w - 2);
    }
  if (NULL != roomdesc)
    {
    mvwaddnstr(titlewindow, 2, 1, roomdesc, title_w - 2);
    }  
  wrefresh(titlewindow);
  }


void printpol(char *string)
  {
  int mode = 0;
  int tokens = -1;
  char *token = NULL;
/*  static char buffer[15];
  time_t t;*/
  
  if (string != NULL)
    {
/*    t = time(NULL);
    strftime(buffer, 14, "%H:%M:%S ", localtime(&t));
    window_put(buffer);*/
    
    token = readtoken(string);
    while (tokens)
      {
      switch (mode)
        {
        case 0:
          if (token == NULL)
            {
            tokens = 0;
            }
          else if (*token == '\0')
            {
            mode = 1;
            }
          else if (0 == ncsstrncmp(token, "<font", 5))
            {
            mode = 2;
            }
          else if (0 == ncsstrcmp(token, "</font>"))
            {
            mode = 2;
            }
          else if (0 == ncsstrcmp(token, "<i>"))
            {
            mode = 2;
            }
          else if (0 == ncsstrcmp(token, "</i>"))
            {
            mode = 2;
            }
          else if (0 == ncsstrncmp(token, "<a", 2))
            {
            mode = 2;
            }
          else if (0 == ncsstrcmp(token, "</a>"))
            {
            mode = 2;
            }
          else if (0 == ncsstrncmp(token, "<img", 4))
            {
            mode = 2;
            }
          else if (0 == ncsstrcmp(token, "</img>"))
            {
            mode = 2;
            }
          else if (0 == ncsstrcmp(token, "&quot;"))
            {
            window_put("\"");
            mode = 1;
            }
          else if (0 == ncsstrcmp(token, "&amp;"))
            {
            window_put("&");
            mode = 1;
            }
          else if (0 == ncsstrcmp(token, "&gt;"))
            {
            window_put(">");
            mode = 1;
            }
          else if (0 == ncsstrcmp(token, "&lt;"))
            {
            window_put("<");
            mode = 1;
            }
          else 
            {
            window_put(token);
            mode = 1;
            }
          break;
        case 1:
          free(token);
          token = readtoken(string);
          mode = 0;
          break;
        case 2:
          if (verbose)
            {
            window_put(token);
            }
          mode = 1;
          break;
        }
      }
    window_put("\n");
    }
  else
    {
    if (debug)
      {
      window_put("Error: NULL ptr given to printpol()");
      window_put("\n");
      }
    }
  }

char *console_input(){
  int c;
  static int i = 0;
  static char buffer[BUFFSIZE];
  while (ERR != (c = wgetch(consolewindow))){
    switch (c){
      case '\n':
      case '\r':
        buffer[i] = '\0';
        wmove(consolewindow, 1, 1);
        for (i = 1; i < console_w - 1; i++)
          {
          waddch(consolewindow, ' ');
          }
        i = 0;
        return (char *) buffer;
        break;
      case KEY_BACKSPACE:
      case 0x007F: /*backspace mapuje na DEL?*/
      case KEY_LEFT:
        if (i > 0)
          {
          i--;
          }
        break;
      case KEY_RESIZE:  
        window_resize();
        break;
      case KEY_RIGHT:
        if (i < BUFFSIZE - 1)
          {
          i++;
          }
        break;
      default:
        if (i < BUFFSIZE - 1)
          {
          if (debug)
            {
            mvwprintw(consolewindow, 0, 1, "0x%X %c", c, c);
            }
          mvwaddch(consolewindow, 1, 1 + i, c);
          buffer[i++] = c;
          }
        break;
      }
    }
  return NULL;
  }
