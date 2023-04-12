/*AmiX - polchat2.c - v. 0.2 - (c) by ABUKAJ (J.M.Kowalski)*/
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
                  printf("Error: ptr (%d) != size (%d)\n", ptr, size);
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
                    printf("Error: ptr (%d) != size (%d)\n", ptr, size);
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
    if (headerlen > 0){
      type = (ppart->header[0] & 0xFFFF);
      switch (type){
        case 0x0000: /*heh, chyba nie wejdzie??*/
          if (headerlen == 0x0000 && nstrings == 0x0000){
            puts("\033[5m\033[44m\033[30m CONNECTION LOST :-( /*???*/\033[0m 0x000000000000");
            run = 0;
            }
          else {
            if (debug){
              puts("Unknown part header");
              verbosedump(ppart);
              }
            }
          break;
        case 0x0001:
          if (headerlen == 0x0001 && nstrings == 0x0000){
            write(sfd, echo, 8);
            if (verbose){
              puts("ECHO REQUESTED");
              }
            }
          else{
            if (debug){
              puts("Unknown part header");
              verbosedump(ppart);
              }
            }     
          break;
        case 0x0262:                
          if (headerlen == 0x0001 && nstrings == 0x0001){
            if (bell){
              puts("BELL\a");
              }
            if (verbose){
              puts(" MSG:");
              puts(ppart->strings[0]);
              }
            else{
              printpol(ppart->strings[0]);
              }
            }
          else{
            if (debug){
              puts("Unknown part header");
              verbosedump(ppart);
              }
            }
          break;
        case 0x0263:
          if (headerlen == 0x0001 && nstrings == 0x0002){
            printf("\033[5m\033[31mWIADOMOSC PRYWATNA:\033[0m ");
            if (verbose){
              puts(ppart->strings[0]);
              }
            else{
              printpol(ppart->strings[0]);
              }
            if (verbose){
              puts("\033[31mOd:\033[0m");
              puts(ppart->strings[1]);
              }
            }
          else if (headerlen == 0x0001 && nstrings == 0x0003){
            printf("\033[5m\033[31mWIADOMOSC PRYWATNA:\033[0m ");
            printpol(ppart->strings[0]);
            putchar('\n');
            if (verbose){
              printf("\033[31mOd:\033[0m ");
              puts(ppart->strings[1]);
              printf("\033[31mDo:\033[0m ");
              puts(ppart->strings[2]);
              }           
            }
          else{
            if (debug){
              puts("Unknown part header");
              verbosedump(ppart);
              }
            }
          break;
        case 0x0267:
          if (headerlen == 0x0002 && nstrings == 0x0001){
            /*if (debug){
              printf(" OP -> jesli 0002 ???");
              printf("  %04X\n", ppart->header[1]);
              printf(" NICK:\n");
              }
            puts(ppart->strings[0]);*/
            addnick(ppart->strings[0], ppart->header[1], 0x0000);
            }
          else{
            if (debug){
              puts("Unknown part header");
              verbosedump(ppart);
              }
            }
          break;
        case 0x0268:
          if (headerlen == 0x0001 && nstrings == 0x0001){
            if (verbose){
              puts(" MSG inny (na wyjscie?):");
              }
            printf("\033[37m%s wychodzi\033[0m\n", ppart->strings[0]);
            remnick(ppart->strings[0]);
            }
          else{
            if (debug){
              puts("Unknown part header");
              verbosedump(ppart);
              }
            }
          break;
        case 0x0269:
          if (headerlen == 0x0002 && nstrings == 0x0001){
            /*if (debug){
              printf(" jakies istotne bity ???");
              printf("  %04X\n", ppart->header[1]);
              printf(" NICK:\n");
              }
            puts(ppart->strings[0]);*/
            addnick(ppart->strings[0], ppart->header[1], 0x0000);
            }
          else{
            if (debug){
              puts("Unknown part header");
              verbosedump(ppart);
              }
            }
          break;
        case 0x026a:
          if (headerlen == 0x0002 && nstrings == 0x0001){
            if (verbose || debug){
              printf(" 0004 ???");
              printf("  %04X\n", ppart->header[1]);
              printf(" NICK:\n");
              }
            puts(ppart->strings[0]);
            }
          else{
            if (debug){
              puts("Unknown part header");
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
              puts("Unknown part header");
              verbosedump(ppart);
              }
            }
          break;
        case 0x0271:
          if (headerlen == 0x0002 && nstrings == 0x0002){
            if (debug){
              printf(" 0000 ???\n");
              printf("  %04X\n", ppart->header[1]);
              printf(" ROOM:\n");
              }
            puts(ppart->strings[0]);
            if (verbose){
              puts(" DESCRIPTION:");
              puts(ppart->strings[1]);
              }                                                                                                            
            }
          else{
            if (debug){
              puts("Unknown part header");
              verbosedump(ppart);
              }
            }
          break;
        case 0x0272:
          if (headerlen == 0x0001 && nstrings == 0x0002){
            if (verbose){
              puts(" PIERDOLY[1]:");
              puts(ppart->strings[0]);
              puts(" PIERDOLY[2]:");
              puts(ppart->strings[0]);
              }
            }
          else{
            if (debug){
              puts("Unknown part header");
              verbosedump(ppart);
              }
            }
          break;
        case 0x0276:
          if (headerlen == 0x0001 && nstrings == 0x0001){
            if (verbose){
              puts(" MSG inny (na wejscie?):");
              puts(ppart->strings[0]);
              }
            else{
              printpol(ppart->strings[0]);
              }
            }
          else{
            if (debug){
              puts("Unknown part header");
              verbosedump(ppart);
              }
            }
          break;
        case 0x0277:
          if (headerlen == 0x0001 && nstrings == 0x0001){
            freenicklist(&nicks);
            if (verbose){
              puts(" MSG inny(na wyjscie?):");
              puts(ppart->strings[0]);
              }
            else{
              printpol(ppart->strings[0]);
              }
            }
          else{
            if (debug){
              puts("Unknown part header");
              verbosedump(ppart);
              }
            }     
          break;
        case (short) 0xffff:
          if (headerlen == 0x0001 && nstrings == 0x0001){
            if (verbose){
              puts(" MSG '-':");
              puts(ppart->strings[0]);
              }
            else{
              printpol(ppart->strings[0]);
              }
            run = false;
            }
          else{
            if (debug){
              puts("Unknown part header");
              verbosedump(ppart);
              }
            }
          break;
        default:
          if (debug){
            puts("Unknown part header (f)");
            verbosedump(ppart);
            }
          break;
        }
      }
    else{
      if (debug){
        puts("Unknown part header (f)");
        verbosedump(ppart);
        }
      if (headerlen == 0x0000 && nstrings == 0x0000){
        puts("\033[5m\033[44m\033[30m CONNECTION LOST :-( /*???*/\033[0m 0x000000000000");
        run = 0;
        }
      }
    }
  else{
    if (debug){
      puts("Error: NULL ptr given to processpart()");
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
      puts("Error: NULL ptr given to makemsg()");
      }
    }
  return result;
  }


void verbosedump(part *dump){
  int i;

  if (dump != NULL){
    puts("HEADER:");
    for (i = 0; i < dump->headerlen; i++){
      printf("%04X", dump->header[i]);
      }
    putchar('\n');
    puts("STRINGS:");
    for (i = 0; i < dump->nstrings; i++){
      puts(dump->strings[i]);
      }
    putchar('\n');
    }
  else{
    if (debug){
      puts("Error: NULL ptr given to verbosedump()");
      }
    }
  }


void printpol(char *string){
  int i = 0;
  int mode = 0;
  int bold = 0;
  int underlined = 0;
  int c;
  
  if (string != NULL){
    while ((c = string[i]) != '\0'){
      switch (mode){
        case 0:
          switch (c){
            case '<':
              mode = 1;
              break;
            case '&':
              mode = 3;
              break;
            default:
              putchar(c);
              i++;
            }  
          break;
        case 1:
          if (0 == ncsstrncmp(string + i, "<b>", 3)){
            bold = -1;
            mode = 4;
            i += 2;
            }
          else if (0 == ncsstrncmp(string + i, "</b>", 4)){
            bold = 0;
            mode = 4;
            i += 3;
            }
          else if (0 == ncsstrncmp(string + i, "<u>", 3)){
            underlined = -1;
            mode = 4;
            i += 2;
            }
          else if (0 == ncsstrncmp(string + i, "</u>", 4)){
            underlined = 0;
            mode = 4;
            i += 3;
            }
          else if (0 == ncsstrncmp(string + i, "<font", 5)) {
            mode = 2;
            }
          else if (0 == ncsstrncmp(string + i, "</font", 6)) {
            mode = 2;
            }
          else if (0 == ncsstrncmp(string + i, "<i>", 3)) {
            mode = 2;
            }
          else if (0 == ncsstrncmp(string + i, "</i>", 4)) {
            mode = 2;
            }
          else if (0 == ncsstrncmp(string + i, "<a", 2)) {
            mode = 2;
            }
          else if (0 == ncsstrncmp(string + i, "</a>", 4)) {
            mode = 2;
            }
          else if (0 == ncsstrncmp(string + i, "<img", 4)) {
            mode = 2;
            }
          else if (0 == ncsstrncmp(string + i, "</img>", 6)) {
            mode = 2;
            }
          else {
            putchar('<');
            mode = 0;
            i++;
            }
          
          break;
        case 2:
          if (c == '>'){
            mode = 0;
            }
          i++;
          break;
        case 3:
          if (0 == ncsstrncmp(string + i, "&quot;", 6)){
            putchar('"');
            mode = 0;
            i += 6;
            }
          else if (0 == ncsstrncmp(string + i, "&amp;", 5)){
            putchar('&');
            mode = 0;
            i += 5;
            }
          else if (0 == ncsstrncmp(string + i, "&gt;", 4)){
            putchar('>');
            mode = 0;
            i += 4;
            }
          else if (0 == ncsstrncmp(string + i, "&lt;", 4)){
            putchar('<');
            mode = 0;
            i += 4;
            }
          else {
            putchar('&');
            mode = 0;
            i++;
            }
          break;
        case 4:
          printf("\033[0m");
          if (bold){
            printf("\033[1m");
            }
          if (underlined){
            printf("\033[4m");
            }
          mode = 2;
          break;
        }
      }
    putchar('\n');
    }
  else{
    if (debug){
      puts("Error: NULL ptr given to printpol()");
      }
    }
  }

