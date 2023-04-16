#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <ncurses.h>

#include "interfeace.h"
#include "polchat.h"
#include "temp.h"

line *window;
WINDOW *chatwindow;
WINDOW *nickwindow;

WINDOW *titlewindow;
WINDOW *consolewindow;

int nicklist_x;
int nicklist_h;
int console_y;
int console_w;
int title_w;

int scr_rows;
int scr_cols;
int window_h;
int window_w;


void remsn(stringnode **wezel){
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


void window_init(){
  int i;

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
  
  window = calloc(window_h - 2, sizeof(line));
  
  for (i = 0; i < window_h - 2; i++){
    window[i].body = NULL;
    window[i].length = 0;
    }
  }


void window_done(){
  int i;

  delwin(chatwindow);
  delwin(nickwindow);
  delwin(consolewindow);
  delwin(titlewindow);
  
  for (i = 0; i < WINDOW_HEIGHT - 2; i++){
    remsn(&(window[i].body));
    window[i].length = 0;
    }
  free(window);
  window = NULL;
  }


void window_nl(){
  int i;

  remsn(&(window[0].body));
  for (i = 1; i < WINDOW_HEIGHT - 2; i++){
    window[i - 1].body = window[i].body;
    window[i - 1].length = window[i].length;
    }
  window[WINDOW_HEIGHT - 3].body = NULL;
  window[WINDOW_HEIGHT - 3].length = 0;
  }


void window_put(char *word){
  char *tmp;
  int len;
  int ptr;
  static int inlen = 0;
  
  if (word != NULL && *word != '\0'){
/*    if (*word == '\n'){
      window_nl();
      }
    else if (*word == '<'){
      addstringnode(&(window[WINDOW_HEIGHT - 3].body), word);
      }
    else{
      len = strlen(word);
      if (window[WINDOW_HEIGHT - 3].length + len <= WINDOW_WIDTH - 2){
        addstringnode(&(window[WINDOW_HEIGHT - 3].body), word);
        window[WINDOW_HEIGHT - 3].length += len;
        }
      else{
        window_nl();
        ptr = 0;
        if (NULL != (tmp = calloc(window_w - 2 + 1, sizeof(char)))){
          while (len > (WINDOW_WIDTH - 2)){
            window[WINDOW_HEIGHT - 3].length = WINDOW_WIDTH - 2;
            strncpy(tmp, (word + ptr), WINDOW_WIDTH - 2);
            addstringnode(&(window[WINDOW_HEIGHT - 3].body), tmp);
            ptr += (WINDOW_WIDTH - 2);
            len -= (WINDOW_WIDTH - 2);
            window_nl();
            }
          free(tmp);
          }
        addstringnode(&(window[WINDOW_HEIGHT - 3].body), (word + ptr));
        window[WINDOW_HEIGHT - 3].length = len;
        }
      }*/
    if (*word == '\n')
      {
      waddch(chatwindow, '\n');
      inlen = 0;
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
/*  char *tmp;
  int len;
  int ptr;

  if (word != NULL && *word != '\0'){
    if (*word == '\n'){
      window_nl();
      }
    else{
      len = strlen(word);
      if (window[WINDOW_HEIGHT - 3].length + len <= WINDOW_WIDTH - 2){
        addstringnode(&(window[WINDOW_HEIGHT - 3].body), word);
        window[WINDOW_HEIGHT - 3].length += len;
        }     
      else{      
        window_nl();
        ptr = 0;
        if (NULL != (tmp = calloc(window_w - 2 + 1, sizeof(char)))){
          while (len > (WINDOW_WIDTH - 2)){
            window[WINDOW_HEIGHT - 3].length = WINDOW_WIDTH - 2;
            strncpy(tmp, (word + ptr), WINDOW_WIDTH - 2);
            addstringnode(&(window[WINDOW_HEIGHT - 3].body), tmp);
            ptr += (WINDOW_WIDTH - 2);
            len -= (WINDOW_WIDTH - 2);
            window_nl();
            }
          free(tmp);
          }
        addstringnode(&(window[WINDOW_HEIGHT - 3].body), (word + ptr));
        window[WINDOW_HEIGHT - 3].length = len;
        }
      }
    }*/
  }


void window_print(){
/*  int i;
  int j;
  stringnode *tmp;
  
  for (i = 0; i < WINDOW_HEIGHT - 2; i++){
    tmp = window[i].body;
    j = 1;
    wmove(chatwindow, i + 1, j);
    while (tmp != NULL){
      if (verbose || (tmp->string[0] != '<'))
        {
        mvwaddstr(chatwindow, i + 1, j, tmp->string);
        j += tmp->len;
        }
      else if (0 == ncsstrcmp(tmp->string, "<b>"))
        {
        wattron(chatwindow, A_BOLD);
        }
      else if (0 == ncsstrcmp(tmp->string, "</b>"))
        {
        wattroff(chatwindow, A_BOLD);
        }
      else if (0 == ncsstrcmp(tmp->string, "<u>"))
        {
        wattron(chatwindow, A_UNDERLINE);
        }
      else if (0 == ncsstrcmp(tmp->string, "</u>"))
        {
        wattroff(chatwindow, A_UNDERLINE);
        }
      else if (0 == ncsstrcmp(tmp->string, "<blink>"))
        {
        wattron(chatwindow, A_BLINK);
        }
      else if (0 == ncsstrcmp(tmp->string, "</blink>"))
        {
        wattroff(chatwindow, A_BLINK);
        }
      else
        {
        mvwaddstr(chatwindow, i + 1, j, tmp->string);
        j += tmp->len;
        }
      tmp = tmp->next;
      }
    while (j < WINDOW_WIDTH - 1){
      mvwaddch(chatwindow, i + 1, j++, ' ');
      }
    }*/
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

  for (i = 1; i < title_w; i++)
    {
    mvwaddch(titlewindow, 1, i, ' ');
    mvwaddch(titlewindow, 2, i, ' ');
    }
  if (NULL != roomname)
    {
    mvwaddstr(titlewindow, 1, 1, roomname);
    }
  if (NULL != roomdesc)
    {
    mvwaddstr(titlewindow, 2, 1, roomdesc);
    }  
  wrefresh(titlewindow);
  }
