#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "interfeace.h"
#include "polchat.h"
#include "temp.h"

line window[MSGSTOREMAX];
WINDOW *chatwindow = NULL;
WINDOW *nickwindow = NULL;
WINDOW *titlewindow = NULL;
WINDOW *consolewindow = NULL;
static int inlen = 0;
static int ptr = 0;
static int len = 0;
static int colour = 0;
static char buffer[BUFFSIZE];

int nicklist_x;
int nicklist_h;
int nicklist_w = 30;
int console_y;
int console_w;
int title_w;

int scr_rows;
int scr_cols;
int window_h;
int window_w;

int useattr = -1;
int window_updated = -1;

int transformrgb(int red, int green, int blue)
  {
  int col = 0;
  
  if (red > 0x007F)
    {
    col |= 0x01;
    }
  
  if (green > 0x007F)
    {
    col |= 0x02;
    }
  
  if (blue > 0x007F)
    {
    col |= 0x04;
    }
  return col;
  }


void window_colouron(int c)
  {
/*  window_put("SETTING COLOUR");*/
  if (colour)
    {
    window_attroff(COLOR_PAIR(colour));
    }
  colour = c;
  window_attron(COLOR_PAIR(c));
  }


void window_colouroff()
  {
  if (colour)
    {
    window_attroff(COLOR_PAIR(colour));
    colour = 0;
    }
  }


void window_attron(int attr)
  {
  if (useattr)
    {
    wattron(chatwindow, attr);
    }
  }


void window_attroff(int attr)
  {
  if (useattr)
    {
    wattroff(chatwindow, attr);
    }
  }


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
  wmove(chatwindow, 1, 0);
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
  wmove(consolewindow, 1, 1);
  wrefresh(consolewindow);
  }


void window_resize()
  {
/*  int i;*/
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

  console_update();
  
  window_updated = -1;
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


void window_putchar(unsigned char c)
  {
  if (inlen >= window_w - 2)
    {
    window_nl();
    }
  waddch(chatwindow, c);
  inlen++;
  }

void window_puthex(unsigned int n, unsigned int len)
  {
  if (inlen + len >= window_w - 2)
    {
    window_nl();
    }
  while (len-- > 0)
    {
    waddch(chatwindow, inttohex((n >> (len * 4)) & 0x0F));
    }
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
      window_attron(A_BOLD);
      }
    else if (0 == ncsstrcmp(word, "</b>"))
      {
      window_attroff(A_BOLD);
      }
    else if (0 == ncsstrcmp(word, "<u>"))
      {
      window_attron(A_UNDERLINE);
      }
    else if (0 == ncsstrcmp(word, "</u>"))
      {
      window_attroff(A_UNDERLINE);
      }
    else if (0 == ncsstrcmp(word, "<blink>"))
      {
      window_attron(A_BLINK);
      }
    else if (0 == ncsstrcmp(word, "</blink>"))
      {
      window_attroff(A_BLINK);
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


void window_putforce(char *word)
  {
  waddstr(chatwindow, word);
  }


void window_print(){
  wnoutrefresh(chatwindow);

  window_updated = -1;
  }


char *readtoken(char *string){
  static int ptr = 0;
  static int done = 0;
  static int link = 0;
  int mode = 0;
  int len = 0;
  int start = 0;
  char c;
  char *result = NULL;

  if (done)
    {
    ptr = 0;
    done = 0;
    }
  else
    {
    start = ptr;
    c = string[ptr];
    switch (c)
      {
      case '\0':
        done = -1;
        mode = 1;
        break;
      case '<':
        mode = 2;
        break;
      case '&':
        mode = 3;
        break;
      case ' ':
      case '\n':
      case '\t':
      case '\r':
      case '\f':
      case '\b':
      case '\a':
        mode = 5;
        break;
      default:
        mode = 0;
        break;
      }

    switch (mode)
      {
      case 0:
        while (!isspace(string[ptr]) && '\0' != string[ptr] && '<' != string[ptr] && (link || ('&' != string[ptr])))
          {
          ptr++;
          }
        len = ptr - start;
        if (NULL != (result = calloc(len + 1, sizeof(char))))
          {
          memcpy(result, string + start, len);
          result[len] = '\0';
          return result;
          }
        break;
      case 1:
        if (NULL != (result = calloc(1, sizeof(char))))
          {
          result[0] = '\0';
          return result;
          }
        break;
      case 2:
        while ('\0' != string[ptr] && '>' != string[ptr])
          {
          ptr++;
          }
        if (string[ptr] == '>')
          {
          ptr++;
          }
        len = ptr - start;
        if (NULL != (result = calloc(len + 1, sizeof(char))))
          {
          memcpy(result, string + start, len);
          result[len] = '\0';
          if (0 == ncsstrncmp(result, "<a", 2))
            {
            link = -1;
            }
          if (0 == ncsstrcmp(result, "</a>"))
            {
            link = 0;
            }             
          return result;
          }
        break;
      case 3:
        while ('\0' != string[ptr] && ';' != string[ptr])
          {
          ptr++;
          }
        if (string[ptr] == ';')
          {
          ptr++;
          }
        len = ptr - start;
        if (NULL != (result = calloc(len + 1, sizeof(char))))
          {
          memcpy(result, string + start, len);
          result[len] = '\0';
          return result;
          }                 
        break;
      case 5:
        while (isspace(string[ptr++]))
          {
          }
        ptr--;
        if (NULL != (result = calloc(2, sizeof(char))))
          {
          result[0] = ' ';
          result[1] = '\0';
          return result;
          }
        break;
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
  char *ptr;
  unsigned int tmp;
  int col;
  
  if (string != NULL)
    {
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
            if (NULL != (ptr = strstr(token, "color=")))
              {
              ptr += 6; /*+= strlen("color=");*/
              if (0 == ncsstrncmp(ptr, "red", 3))
                {
                window_colouron(COLOR_RED);
                }
              else
                {
                sscanf(ptr, "#%x", &tmp);
                col = transformrgb((tmp >> 16) & 0x00FF, (tmp >> 8) & 0x00FF, tmp & 0x00FF);
                if (col != COLOR_WHITE && col != COLOR_BLACK)
                  {
                  window_colouron(col);
                  }
                }
              }
            mode = 2/*2*/;
            }
          else if (0 == ncsstrcmp(token, "</font>"))
            {
            window_colouroff();
            mode = 2/*2*/;
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
  int j;
  int updated = 0;
  int tptr;
  int tlen = 0;
  int delta;
  const char *nick = NULL;
  
  while (ERR != (c = wgetch(consolewindow))){
    switch (c){
      case '\n':
      case '\r':
        buffer[len] = '\0';
        wmove(consolewindow, 1, 1);
        for (j = 1; j < console_w - 1; j++)
          {
          waddch(consolewindow, ' ');
          }
        ptr = 0;
        len = 0;
        console_update();
        return (char *) buffer;
        break;
      case KEY_BACKSPACE:
      case 0x007F: /*backspace mapuje na DEL?*/
        if (ptr > 0)
          {
          for (j = ptr; j <= len; j++)
            {
            buffer[j - 1] = buffer[j];
            }
          ptr--;
          len--;
          buffer[len] = '\0';
          updated = -1;
          /*console_update();*/
          }
        break;
      case KEY_DL:
      case KEY_DC:
        if (ptr >= 0 && ptr < len)
          {
          for (j = ptr; j < len; j++)
            {
            buffer[j] = buffer[j + 1];
            }
          len--;
          buffer[len] = '\0';
          updated = -1;
          /*console_update();*/
          }
        break;
      case KEY_LEFT:
        if (ptr > 0)
          {
          ptr--;
          wmove(consolewindow, 1, 1 + ptr);
          if (ptr /*+1*/ > window_w - 3/*-2*/)
            {
            updated = -1;
            }
          }
        break;
      case KEY_RESIZE:  
        window_resize();
        break;
      case KEY_RIGHT:
        if (ptr < BUFFSIZE - 1 && ptr < len)
          {
          ptr++;
          wmove(consolewindow, 1, 1 + ptr);
          if (ptr > window_w - 2)
            {
            updated = -1;
            }
          }
        break;
      case '\t':
        tptr = ptr - 1;
        tlen = 1;
        while (tptr >= 0 && buffer[tptr] != ' ')
          {
          tptr--;
          tlen++;
          }
        tptr++;/*korekta dlugosci*/
        tlen--;
        if (NULL != (nick = getnickbyprefix(buffer + tptr, tlen, nicks)))
          {
          delta = strlen(nick) - tlen;
          if (len + delta < BUFFSIZE)
            {
            for (j = len + delta; j >= ptr + delta; j--)
              {
              buffer[j] = buffer[j - delta];
              }
            for (j = tlen; j < tlen + delta; j++)
              {
              buffer[ptr++] = nick[j];
              len++;
              }
            updated = -1;
            }
          nick = NULL;
          }
        break;
      default:
        if (ptr < BUFFSIZE - 1 && len < BUFFSIZE - 1)
          {
          if (debug)
            {
            mvwprintw(consolewindow, 0, 1, "0x%X %c", c, c);
            }
          for (j = len; j > ptr; j--)
            {
            buffer[j] = buffer[j - 1];
            }
          buffer[ptr++] = c;
          len++;
          buffer[len] = '\0';
          updated = -1;
          }
        break;
      }
    }
  if (updated)
    {
    console_update();
    }
  return NULL;
  }


void console_update()
  {
  int i = 0;
  int d = 0;

  if (ptr > console_w - 2)
    {
    d = ptr + 2 - console_w;
    }
  wmove(consolewindow, 1, 1);
  while (i + d < len && i < console_w - 2)
    {
    waddch(consolewindow, (unsigned char) buffer[d + i++]);
    }
  while (i++ < console_w - 2)
    {
    waddch(consolewindow, ' ');
    }
  wmove(consolewindow, 1, ptr + 1);
  wnoutrefresh(consolewindow);
  
  window_updated = -1;
  }


void priv(privinfo info, char *who, char *what)
  {
  window_attron(A_BLINK);
  switch (info)
    {
    case PRIV_FROM:
      window_put("<-- ");
      break;
    case PRIV_TO:
      window_put("--> ");
      break;
    }
  window_attroff(A_BLINK);
  window_put(who);
  window_put(": ");
  if (!verbose)
    {
    window_addstr(what);
    }
  printlog(who, what);
  }


char *input_password()
  {
  int state = 0;
  int inbuf = 0;
  int length = 0;
  static char buffer[1024];
  char c;
  char *string = NULL;
  char *tmp;

  window_put(" Password:");
  window_nl();
  wnoutrefresh(chatwindow);
  doupdate();
  nodelay(consolewindow, FALSE);
  while ((state != 2) && ERR != (c = wgetch(consolewindow)))
    {
    buffer[inbuf] = c;
    switch (c)
      {
      case '\n':
        state = 2;
        break;
      default:
        state = 1;
        if (inbuf >= 1024)
          {
          if (NULL != string)
            {
            if (NULL != (tmp = realloc(string, length + 1024)))
              {
              string = tmp;
              strncpy(string + length, buffer, 1024);
              length += 1024;
              }
            else
              {
              free(string);
              string = NULL;
              }
            }
          else
            {
            if (NULL != (string = calloc(1024, sizeof(char))))
              {
              strncpy(string, buffer, 1024);
              length = 1024;
              }
            }
          inbuf = 0;
          }                
        buffer[inbuf++] = c;
        break;
      }                           
    }
  
  nodelay(consolewindow, TRUE);
  if (state != 0)
    {
    if (NULL != string)
      {
      if (NULL != (tmp = realloc(string, length + inbuf + 1)))
        {
        string = tmp;
        strncpy(string + length, buffer, inbuf);
        length += inbuf;
        string[length] = '\0';
        }
      else
        {
        free(string);
        string = NULL;
        }
      }
    else
      {
      if (NULL != (string = calloc(inbuf + 1, sizeof(char))))
        {
        strncpy(string, buffer, inbuf);
        length = inbuf;
        string[inbuf] = '\0';
        }
      }
    }
  return string;
  }
