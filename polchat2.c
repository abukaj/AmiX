/*AmiX - polchat2.c - v. 0.2 - (c) by ABUKAJ (J.M.Kowalski)*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <ncurses.h>

#include "polchat.h"
#include "polchat1.h"
#include "polchat2.h"
#include "interfeace.h"
#include "temp.h"

int read(int, char *, int);
int write(int, char *, int);


unsigned char *welcome(unsigned char *nick, unsigned char *pass,
                       unsigned char *room, int sfd){
  unsigned char *nw = NULL, *pw = NULL, *rw = NULL;
  unsigned char *p1 = NULL, *p2 = NULL, *p3 = NULL;
  unsigned char *result = NULL;
  int size;
  int tmp;
  int ptr;
  
  if (NULL != (nw = wrapstring(nick))){
    if (NULL != (pw = wrapstring(pass))){
      if (NULL != (rw = wrapstring(room))){
        if (NULL != (p1 = wrapstring("http://www.polchat.pl/chat/"))){
          if (NULL != (p2 = wrapstring("http://s9.polchat.pl/"))){
            if (NULL != (p3 = wrapstring("nlst=1&nnum=1&jlmsg=true&ignprv=false"))){
              size = 4; /*sam size*/
              size += 6; /*header*/
              size += wrapsize(nw); /*nick*/
              size += wrapsize(pw); /*pass*/
              size += 3; /*nie mam pojecia co - 3*0x00... Pusty string?*/
              size += wrapsize(rw); /*room*/
              size += wrapsize(p1);
              size += wrapsize(p2);
              size += wrapsize(p3);/*jakies opcje*/
              if (NULL != (result = calloc(size, sizeof(char)))){
                /*przepisuje rozmiar*/
                result[3] = size % 256;
                tmp = size / 256;
                result[2] = tmp % 256;
                tmp /= 256;
                result[1] = tmp % 256;
                tmp /= 256;
                result[0] = tmp;
                ptr = 4;
                
                result[ptr++] = 0x00;/*naglowek*/
                result[ptr++] = 0x01;
                result[ptr++] = 0x00;
                result[ptr++] = 0x07;
                result[ptr++] = 0x05;
                result[ptr++] = 0x78;

                memcpy(result + ptr, nw, wrapsize(nw));
                ptr += wrapsize(nw);
                memcpy(result + ptr, pw, wrapsize(pw));
                ptr += wrapsize(pw);

                result[ptr++] = 0x00;
                result[ptr++] = 0x00;
                result[ptr++] = 0x00;
                
                memcpy(result + ptr, rw, wrapsize(rw));
                ptr += wrapsize(rw);
                memcpy(result + ptr, p1, wrapsize(p1));
                ptr += wrapsize(p1);
                memcpy(result + ptr, p2, wrapsize(p2));
                ptr += wrapsize(p2);
                memcpy(result + ptr, p3, wrapsize(p3));
                ptr += wrapsize(p3);
                if (ptr != size){
                  printw("Error: ptr (%d) != size (%d)\n", ptr, size);
                  }
                write(sfd, result, size);/*i od razu wyslemy co trzeba*/
                free(result);
                result = NULL;
                }
              free(p3);
              }
            free(p2);
            }
          free(p1);
          }
        free(rw);
        }
      free(pw);
      }
    free(nw);
    }
  return result;
  }


unsigned char *welcome2(unsigned char *nick, unsigned char *pass,
                       unsigned char *room, int sfd){
  unsigned char *nw = NULL, *pw = NULL, *rw = NULL;
  unsigned char *p1 = NULL, *p2 = NULL, *p3 = NULL, *klient = NULL;
  unsigned char *result = NULL;
  int size;
  int tmp;
  int ptr;
  
  if (NULL != (nw = wrapstring(nick))){
    if (NULL != (pw = wrapstring(pass))){
      if (NULL != (rw = wrapstring(room))){
        if (NULL != (p1 = wrapstring("http://www.polchat.pl/chat/room.phtml/?room=AmiX"))){
          if (NULL != (p2 = wrapstring("polchat.pl"))){
            if (NULL != (p3 = wrapstring("nlst=1&nnum=1&jlmsg=true&ignprv=false"))){
              if (NULL != (klient = wrapstring("AmiX v. 0.2a"))){
                size = 4; /*sam size*/
                size += 6; /*header*/
                size += wrapsize(nw); /*nick*/
                size += wrapsize(pw); /*pass*/
                size += 3; /*nie mam pojecia co - 3*0x00... Pusty string?*/
                size += wrapsize(rw); /*room*/
                size += wrapsize(p1);
                size += wrapsize(p2);
                size += wrapsize(p3);/*jakies opcje*/
                size += wrapsize(klient);
                if (NULL != (result = calloc(size, sizeof(char)))){
                  /*przepisuje rozmiar*/
                  result[3] = size % 256;
                  tmp = size / 256;
                  result[2] = tmp % 256;
                  tmp /= 256;
                  result[1] = tmp % 256;
                  tmp /= 256;
                  result[0] = tmp;
                  ptr = 4;
                
                  result[ptr++] = 0x00;/*naglowek*/
                  result[ptr++] = 0x01;
                  result[ptr++] = 0x00;
                  result[ptr++] = 0x08;
                  result[ptr++] = 0x05;
                  result[ptr++] = 0x78;

                  memcpy(result + ptr, nw, wrapsize(nw));
                  ptr += wrapsize(nw);
                  memcpy(result + ptr, pw, wrapsize(pw));
                  ptr += wrapsize(pw);

                  result[ptr++] = 0x00;
                  result[ptr++] = 0x00;
                  result[ptr++] = 0x00;
                
                  memcpy(result + ptr, rw, wrapsize(rw));
                  ptr += wrapsize(rw);
                  memcpy(result + ptr, p1, wrapsize(p1));
                  ptr += wrapsize(p1);
                  memcpy(result + ptr, p2, wrapsize(p2));
                  ptr += wrapsize(p2);
                  memcpy(result + ptr, p3, wrapsize(p3));
                  ptr += wrapsize(p3);
                  memcpy(result + ptr, klient, wrapsize(klient));
                  ptr += wrapsize(klient);
                  if (ptr != size){
                    printw("Error: ptr (%d) != size (%d)\n", ptr, size);
                    }
                  write(sfd, result, size);/*i od razu wyslemy co trzeba*/
                  free(result);
                  result = NULL;
                  }
                free(klient);
                }
              free(p3);
              }
            free(p2);
            }
          free(p1);
          }
        free(rw);
        }
      free(pw);
      }
    free(nw);
    }
  return result;
  }


void processpart(part *ppart, int sfd){
  unsigned char echo[8] = {0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00};
  short type;
  short headerlen;
  short nstrings;
  int i;
  
  if (NULL != ppart){
    headerlen = ppart->headerlen;
    nstrings = ppart->nstrings;

    if (verbose){
      verbosedump(ppart);
      }
    else{
      if (headerlen > 0){
        type = (ppart->header[0] & 0xFFFF);
        switch (type){
          case 0x0000: /*heh, chyba nie wejdzie??*/
            if (headerlen == 0x0000 && nstrings == 0x0000){
              window_put("\033[5m\033[44m\033[30m");
              window_put("CONNECTION LOST :-( /*???*/");
              window_put("\033[0m");
              window_put("0x000000000000");
              window_put("\n");
              run = 0;
              }
            else {
              if (debug){
                window_put("Unknown part header");
                window_put("\n");
                verbosedump(ppart);
                }
              }
            break;
          case 0x0001:
            if (headerlen == 0x0001 && nstrings == 0x0000){
              write(sfd, echo, 8);
              if (verbose){
                window_put("ECHO REQUESTED");
                window_put("\n");
                }
              }
            else{
              if (debug){
                window_put("Unknown part header");
                window_put("\n");
                verbosedump(ppart);
                }
              }     
            break;
          case 0x0262:                
            if (headerlen == 0x0001 && nstrings == 0x0001){
              if (bell){
                window_put("\a");
                window_put("\n");
                }
              if (verbose){
                window_put(" MSG:");
                window_put("\n");
                window_put(ppart->strings[0]);
                window_put("\n");
                }
              else{
                printpol(ppart->strings[0]);
                }
              }
            else{
              if (debug){
                window_put("Unknown part header");
                window_put("\n");
                verbosedump(ppart);
                }
              }
            break;
          case 0x0263:
            if (headerlen == 0x0001 && nstrings == 0x0002){
              window_put("<blink>");
              window_put("WIADOMOSC PRYWATNA:");
              window_put("</blink>");
              printpol(ppart->strings[0]);
              if (verbose){
                window_put("\033[31m");
                window_put("Od:");
                window_put("\033[0m");
                window_put(ppart->strings[1]);
                window_put("\n");
                }
              }
            else if (headerlen == 0x0001 && nstrings == 0x0003){
              window_put("<blink>");
              window_put("WIADOMOSC PRYWATNA: ");
              window_put("</blink>");
              printpol(ppart->strings[0]);
              if (verbose){
                window_put("\033[31m");
                window_put("Od: ");
                window_put("\033[0m");
                window_put(ppart->strings[1]);
                window_put("\n");
                window_put("\033[31m");
                window_put("Do: ");
                window_put("\033[0m");
                window_put(ppart->strings[2]);
                window_put("\n");
                }           
              }
            else{
              if (debug){
                window_put("Unknown part header");
                window_put("\n");
                verbosedump(ppart);
                }
              }
            break;
          case 0x0267:
            if (headerlen == 0x0002 && nstrings == 0x0001){
              addnick(ppart->strings[0], ppart->header[1], 0x0000);
              printnicks(nicks);
              }
            else{
              if (debug){
                window_put("Unknown part header");
                window_put("\n");
                verbosedump(ppart);
                }
              }
            break;
          case 0x0268:
            if (headerlen == 0x0001 && nstrings == 0x0001){
              if (verbose){
                window_put(" MSG inny (na wyjscie?):");
                window_put("\n");
                }
              remnick(ppart->strings[0]);
              printnicks(nicks);
              }
            else{
              if (debug){
                window_put("Unknown part header");
                window_put("\n");
                verbosedump(ppart);
                }
              }
            break;
          case 0x0269:
            if (headerlen == 0x0002 && nstrings == 0x0001){
              addnick(ppart->strings[0], ppart->header[1], 0x0000);
              printnicks(nicks);
              }
            else{
              if (debug){
                window_put("Unknown part header");
                window_put("\n");
                verbosedump(ppart);
                }
              }
            break;
          case 0x026a:
            if (headerlen == 0x0002 && nstrings == 0x0001){
              if (verbose || debug){
                printw(" 0004 ???");
                printw("  %04X\n", ppart->header[1]);
                printw(" NICK:\n");
                }
              printw(ppart->strings[0]);
              }
            else{
              if (debug){
                window_put("Unknown part header");
                window_put("\n");
                verbosedump(ppart);
                }
              }
            break;
          case 0x026b:
            if (headerlen >= 5 && ppart->header[1] == 0x0001 &&
                ppart->header[2] == 0x0001 && ppart->header[3] == 0x0000 &&
                ppart->header[4] == 0x0000){
              for (i = 0; i < nstrings; i++){
                addnick(ppart->strings[i], ppart->header[2 * i + 5], ppart->header[2 * i + 6]);
                }
              printnicks(nicks);
              }
            else {
              if (debug){
                window_put("Unknown part header");
                window_put("\n");
                verbosedump(ppart);
                }
              }
            break;
          case 0x0271:
            if (headerlen == 0x0002 && nstrings == 0x0002)
              {
              if (NULL != roomname)
                {
                free(roomname);
                roomname = NULL;
                }
              if (NULL != (roomname = calloc(1 + strlen(ppart->strings[0]), sizeof(char))))
                {
                strcpy(roomname, ppart->strings[0]);
                }
              if (NULL != roomdesc)
                {
                free(roomdesc);
                roomdesc = NULL;
                }
              if (NULL != (roomdesc = calloc(1 + strlen(ppart->strings[1]), sizeof(char))))
                {
                strcpy(roomdesc, ppart->strings[1]);
                }
              printtitle();
              }
            else{
              if (debug){
                window_put("Unknown part header");
                window_put("\n");
                verbosedump(ppart);
                }
              }
            break;
          case 0x0272:
            if (headerlen == 0x0001 && nstrings == 0x0002){
              if (verbose){
                window_put(" PIERDOLY[1]:");
                window_put(ppart->strings[0]);
                window_put("\n");
                window_put(" PIERDOLY[2]:");
                window_put(ppart->strings[0]);
                window_put("\n");
                }
              }
            else{
              if (debug){
                window_put("Unknown part header");
                window_put("\n");
                verbosedump(ppart);
                }
              }
            break;
          case 0x0276:
            if (headerlen == 0x0001 && nstrings == 0x0001){
              if (verbose){
                window_put(" MSG inny (na wejscie?):");
                window_put("\n");
                window_put(ppart->strings[0]);
                window_put("\n");
                }
              else{
                printpol(ppart->strings[0]);
                }
              }
            else{
              if (debug){
                window_put("Unknown part header");
                window_put("\n");
                verbosedump(ppart);
                }
              }
            break;
          case 0x0277:
            if (headerlen == 0x0001 && nstrings == 0x0001){
              freenicklist(&nicks);
              printnicks(nicks);
              if (verbose){
                window_put(" MSG inny(na wyjscie?):");
                window_put("\n");
                window_put(ppart->strings[0]);
                window_put("\n");
                }
              else{
                printpol(ppart->strings[0]);
                }
              }
            else{
              if (debug){
                window_put("Unknown part header");
                window_put("\n");
                verbosedump(ppart);
                }
              }     
            break;
          case (short) 0xffff:
            if (headerlen == 0x0001 && nstrings == 0x0001){
              if (verbose){
                window_put(" MSG '-':");
                window_put("\n");
                window_put(ppart->strings[0]);
                window_put("\n");
                }
              else{
                printpol(ppart->strings[0]);
                }
              run = false;
              }
            else{
              if (debug){
                window_put("Unknown part header");
                window_put("\n");
                verbosedump(ppart);
                }
              }
            break;
          default:
            if (debug){
              window_put("Unknown part header (f)");
              window_put("\n");
              verbosedump(ppart);
              }
            break;
          }
        }
      else{
        if (debug){
          window_put("Unknown part header (f)");
          window_put("\n");
          verbosedump(ppart);
          }
        if (headerlen == 0x0000 && nstrings == 0x0000){
          window_put("\033[5m\033[44m\033[30m");
          window_put("CONNECTION LOST :-( /*???*/");
          window_put("\033[0m");
          window_put("0x000000000000");
          window_put("\n");
          run = 0;
          }
        }
      }
    }
  else{
    if (debug){
      window_put("Error: NULL ptr given to processpart()");
      window_put("\n");
      }
    }
  }


part *makemsg(unsigned char *string){
  part *result = NULL;
  
  if (string != NULL){
    if (NULL != (result = calloc(1, sizeof(part)))){
      if (NULL != (result->header = calloc(1, sizeof(short)))){
        result->headerlen = 1;
        result->header[0] = 0x019a;
        if (NULL != (result->strings = calloc(1, sizeof(char *)))){
          result->nstrings = 1;
          if (NULL != (result->strings[0] = calloc(strlen(string) + 1, sizeof(char)))){
            strcpy(result->strings[0], string);
            }
          else{
            freepart(&result);
            }
          }
        else{
          freepart(&result);
          }
        }
      else{
        freepart(&result);
        }
      }
    }
  else{
    if (debug){
      window_put("Error: NULL ptr given to makemsg()");
      window_put("\n");
      }
    }
  return result;
  }


void verbosedump(part *dump){
  int i;
  static char buffer[6];
  
  if (dump != NULL){
    wprintw(chatwindow, "HEADER[%u]:\n", dump->headerlen);
    for (i = 0; i < dump->headerlen; i++){
      buffer[3] = inttohex(dump->header[i] & 0x0F);
      buffer[2] = inttohex((dump->header[i] >> 4) & 0x0F);
      buffer[1] = inttohex((dump->header[i] >> 8) & 0x0F);
      buffer[0] = inttohex((dump->header[i] >> 12) & 0x0F);
      buffer[4] = ' ';
      buffer[5] = '\0';
      waddstr(chatwindow, buffer);
      }
    wprintw(chatwindow, "\nSTRINGS[%u]:\n", dump->nstrings);
    for (i = 0; i < dump->nstrings; i++){
      wprintw(chatwindow, "%s\n", dump->strings[i]);
      }
    waddstr(chatwindow, "\n");
    }
  else{
    if (debug){
      window_put("Error: NULL ptr given to verbosedump()");
      window_put("\n");
      }
    }
  }


void printpol(char *string){
  int mode = 0;
  int tokens = -1;
  char *token = NULL;

  if (string != NULL){
    token = readtoken(string);
    while (tokens){
      switch (mode){
        case 0:
          if (token == NULL){
            tokens = 0;
            }
          else if (*token == '\0'){
            mode = 1;
            }
          else if (0 == ncsstrncmp(token, "<font", 5)) {
            mode = 2;
            }
          else if (0 == ncsstrcmp(token, "</font>")) {
            mode = 2;
            }
          else if (0 == ncsstrcmp(token, "<i>")) {
            mode = 2;
            }
          else if (0 == ncsstrcmp(token, "</i>")) {
            mode = 2;
            }
          else if (0 == ncsstrncmp(token, "<a", 2)) {
            mode = 2;
            }
          else if (0 == ncsstrcmp(token, "</a>")) {
            mode = 2;
            }
          else if (0 == ncsstrncmp(token, "<img", 4)) {
            mode = 2;
            }
          else if (0 == ncsstrcmp(token, "</img>")) {
            mode = 2;
            }
          else if (0 == ncsstrcmp(token, "&quot;")){
            window_put("\"");
            mode = 1;
            }
          else if (0 == ncsstrcmp(token, "&amp;")){
            window_put("&");
            mode = 1;
            }
          else if (0 == ncsstrcmp(token, "&gt;")){
            window_put(">");
            mode = 1;
            }
          else if (0 == ncsstrcmp(token, "&lt;")){
            window_put("<");
            mode = 1;
            }
          else {
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
          if (verbose){
            window_put(token);
            }
          mode = 1;
          break;
        }
      }
    window_put("\n");
    }

  else{
    if (debug){
      window_put("Error: NULL ptr given to printpol()");
      window_put("\n");
      }
    }
  }

