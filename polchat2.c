/*AmiX - polchat2.c - v. 0.2 - (c) by ABUKAJ (J.M.Kowalski)*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include <ncurses.h>

#include "polchat.h"
#include "polchat2.h"
#include "interfeace.h"
#include "temp.h"

int read(int, char *, int);
int write(int, char *, int);


unsigned char *welcome(unsigned char *nick, unsigned char *pass,
                       unsigned char *room, int sfd)
  {
  unsigned char *nw = NULL, *pw = NULL, *rw = NULL;
  unsigned char *p1 = NULL, *p2 = NULL, *p3 = NULL;
  unsigned char *result = NULL;
  int size;
  int tmp;
  int ptr;
  
  if (NULL != (nw = wrapstring(nick)))
    {
    if (NULL != (pw = wrapstring(pass)))
      {
      if (NULL != (rw = wrapstring(room)))
        {
        if (NULL != (p1 = wrapstring("http://www.polchat.pl/chat/")))
          {
          if (NULL != (p2 = wrapstring("http://s9.polchat.pl/")))
            {
            if (NULL != (p3 = wrapstring("nlst=1&nnum=1&jlmsg=true&ignprv=false")))
              {
              size = 4; /*sam size*/
              size += 6; /*header*/
              size += wrapsize(nw); /*nick*/
              size += wrapsize(pw); /*pass*/
              size += 3; /*nie mam pojecia co - 3*0x00... Pusty string?*/
              size += wrapsize(rw); /*room*/
              size += wrapsize(p1);
              size += wrapsize(p2);
              size += wrapsize(p3);/*jakies opcje*/
              if (NULL != (result = calloc(size, sizeof(char))))
                {
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
                  window_put("Error: ptr != size");
                  window_put("\n");
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
                       unsigned char *room, int sfd)
  {
  unsigned char *nw = NULL, *pw = NULL, *rw = NULL;
  unsigned char *p1 = NULL, *p2 = NULL, *p3 = NULL, *klient = NULL;
  unsigned char *result = NULL;
  int size;
  int tmp;
  int ptr;
  
  if (NULL != (nw = wrapstring(nick)))
    {
    if (NULL != (pw = wrapstring(pass)))
      {
      if (NULL != (rw = wrapstring(room)))
        {
        if (NULL != (p1 = wrapstring("http://www.polchat.pl/chat/room.phtml/?room=AmiX")))
          {
          if (NULL != (p2 = wrapstring("polchat.pl")))
            {
            if (NULL != (p3 = wrapstring("nlst=1&nnum=1&jlmsg=true&ignprv=false")))
              {
              if (NULL != (klient = wrapstring("AmiX v. 0.2a")))
                {
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
                if (NULL != (result = calloc(size, sizeof(char))))
                  {
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
                  if (ptr != size)
                    {
                    window_put("Error: ptr != size");
                    window_put("\n");
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


void processpart(part *ppart, int sfd)
  {
  static unsigned char echo[8] = {0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00};
  /*static char buffer[6];*/
  short type;
  short headerlen;
  short nstrings;
  int i;
  
  if (NULL != ppart)
    {
    headerlen = ppart->headerlen;
    nstrings = ppart->nstrings;
    if (headerlen > 0)
      {
      type = (ppart->header[0] & 0xFFFF);
      switch (type)
        {
        case 0x0000: /*heh, chyba nie wejdzie??*/
          if (debug)
            {
            window_put("Unknown part header");
            window_put("\n");
            if (!verbose)
              {
              verbosedump(ppart);
              }
            }
          break;
        case 0x0001:/*ECHO REQUEST*/
          if (headerlen == 0x0001 && nstrings == 0x0000)
            {
            write(sfd, echo, 8);
            }
          else
            {
            if (debug)
              {
              window_put("Unknown part header");
              window_put("\n");
              if (!verbose)
                {
                verbosedump(ppart);
                }
              }
            }     
          break;
        case 0x0262:/*MSG*/                
          if (headerlen == 0x0001 && nstrings == 0x0001)
            {
            if (bell)
              {
              window_put("\a");
              window_put("\n");
              }
            if (!verbose)
              {
              window_addstr(ppart->strings[0]);
              }
            }
          else
            {
            if (debug)
              {
              window_put("Unknown part header");
              window_put("\n");
              if (!verbose)
                {
                verbosedump(ppart);
                }
              }
            }
          break;
        case 0x0263:/*Priv. msg*/
          if (headerlen == 0x0001 && nstrings == 0x0002)
            {
            window_put("<blink>");
            window_put("WIADOMOSC PRYWATNA:");
            window_put("</blink>");
            if (!verbose)
              {
              window_addstr(ppart->strings[0]);
              }    
            }
          else if (headerlen == 0x0001 && nstrings == 0x0003)
            {
            window_put("<blink>");
            window_put("WIADOMOSC PRYWATNA: ");
            window_put("</blink>");
            if (!verbose)
              {
              window_addstr(ppart->strings[0]);
              }
            }
          else
            {
            if (debug)
              {
              window_put("Unknown part header");
              window_put("\n");
              if (!verbose)
                {
                verbosedump(ppart);
                }
              }
            }
          break;
        case 0x0266: /*client config*/
          if (headerlen == 0x0001 || nstrings == 0x0001)
            {
            if (debug)
              {
              window_put("CLIENT CONFIG: ");
              window_put(ppart->strings[0]);
              window_put("\n");
              }
            }
          else
            {
            if (debug)
              {
              window_put("Unknown part header");
              window_put("\n");
              if (!verbose)
                {
                verbosedump(ppart);
                }
              }
            }
          break;
        case 0x0267:/*nick entered*/
          if (headerlen == 0x0002 && nstrings == 0x0001)
            {
            addnick(ppart->strings[0], ppart->header[1], 0x0000);
            printnicks(nicks);
            if ((ppart->header[1] & 0x00ff8c) != 0x0000 && debug)
              {
              window_put("Unknown status of: ");
              window_put(ppart->strings[0]);
              window_put(" : ");
              window_puthex(ppart->header[1], 4);
              window_nl();
              }
            }
          else
            {
            if (debug)
              {
              window_put("Unknown part header");
              window_nl();
              if (!verbose)
                {
                verbosedump(ppart);
                }
              }
            }
          break;
        case 0x0268:/*nick left*/
          if (headerlen == 0x0001 && nstrings == 0x0001)
            {
            remnick(ppart->strings[0]);
            printnicks(nicks);
            }
          else
            {
            if (debug)
              {
              window_put("Unknown part header");
              window_nl();
              if (!verbose)
                {
                verbosedump(ppart);
                }
              }
            }
          break;
        case 0x0269:/*NICK update*/
          if (headerlen == 0x0002 && nstrings == 0x0001)
            {
            addnick(ppart->strings[0], ppart->header[1], 0x0000);
            printnicks(nicks);
            if ((ppart->header[1] & 0x00ff8c) != 0x0000 && debug)
              {
              window_put("Unknown status of: ");
              window_put(ppart->strings[0]);
              window_put(" : ");
              window_puthex(ppart->header[1], 4);
              window_nl();
              }
            }
          else{
            if (debug)
              {
              window_put("Unknown part header");
              window_nl();
              if (!verbose)
                {
                verbosedump(ppart);
                }  
              }
            }
          break;
        case 0x026a:/*I have absolutly no idea - chyba ze4 wlazlem jako ja???*/
          if (headerlen == 0x0002 && nstrings == 0x0001)
            {
            if (verbose || debug)
              {
              window_put(" 0004 ??? ");
              window_puthex(ppart->header[1], 4);/*
              buffer[4] = inttohex(ppart->header[1] & 0x0F);
              buffer[3] = inttohex((ppart->header[1] >> 4) & 0x0F);
              buffer[2] = inttohex((ppart->header[1] >> 8) & 0x0F);
              buffer[1] = inttohex((ppart->header[1] >> 12) & 0x0F);
              buffer[0] = ' ';
              buffer[5] = '\0';
              window_put(buffer);*/
              window_nl();
              window_put(" NICK:");
              window_nl();
              if (!verbose)
                {
                window_put(ppart->strings[0]);
                window_nl();
                }
              }
            }
          else
            {
            if (debug)
              {
              window_put("Unknown part header");
              window_put("\n");
              if (!verbose)
                {
                verbosedump(ppart);
                }
              }
            }
          break;
        case 0x026b:/*nicklist*/
          if (headerlen >= 5 && ppart->header[1] == 0x0001 &&
            ppart->header[2] == 0x0001 && ppart->header[3] == 0x0000 &&
            ppart->header[4] == 0x0000)
            {
            for (i = 0; i < nstrings; i++)
              {
              addnick(ppart->strings[i], ppart->header[2 * i + 5], ppart->header[2 * i + 6]);
              if (((ppart->header[2 * i + 5] & 0x00ff8c) != 0x0000 || ppart->header[2 * i + 6] != 0x0000) && debug)
                {
                window_put("Unknown status of: ");
                window_put(ppart->strings[i]);
                window_put(" : ");
                window_puthex(ppart->header[2 * i + 5], 4);
                window_put(" : ");
                window_puthex(ppart->header[2 * i + 6], 4);
                window_nl();
                }
              }
            printnicks(nicks);
            }
          else
            {
            if (debug)
              {
              window_put("Unknown part header");
              window_put("\n");
              if (!verbose)
                {
                verbosedump(ppart);
                }
              }
            }
          break;
        case 0x0271: /*ROOMINFO*/
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
          else
            {
            if (debug)
              {
              window_put("Unknown part header");
              window_put("\n");
              if (!verbose)
                {
                verbosedump(ppart);
                }
              }
            }
          break;
        case 0x0272:/*PIERDOLY*/
          if (headerlen != 0x0001 || nstrings != 0x0002)
            {
            if (debug)
              {
              window_put("Unknown part header");
              window_put("\n");
              if (!verbose)
                {
                verbosedump(ppart);
                }
              }
            }
          break;
        case 0x0276:/*MSG powitalny*/
          if (headerlen == 0x0001 && nstrings == 0x0001)
            {
            if (!verbose)
              {
              window_addstr(ppart->strings[0]);
              }
            }
          else
            {
            if (debug)
              {
              window_put("Unknown part header");
              window_put("\n");
              if (!verbose)
                {
                verbosedump(ppart);
                }
              }
            }
          break;
        case 0x0277:/*MSG pozegnalny*/
          if (headerlen == 0x0001 && nstrings == 0x0001)
            {
            freenicklist(&nicks);
            printnicks(nicks);
            if (!verbose)
              {
              window_addstr(ppart->strings[0]);
              }
            }
          else
            {
            if (debug)
              {
              window_put("Unknown part header");
              window_put("\n");
              if (!verbose)
                {
                verbosedump(ppart);
                }
              } 
            }
          break;
        case (short) 0xffff:/*MSG -*/
          if (headerlen == 0x0001 && nstrings == 0x0001)
            {
            if (!verbose)
              {
              window_addstr(ppart->strings[0]);
              }
            run = false;
            }
          else
            {
            if (debug)
              {
              window_put("Unknown part header");
              window_put("\n");
              if (!verbose)
                {
                verbosedump(ppart);
                }
              }
            }
          break;
        default:
          if (debug)
            {
            window_put("Unknown part header");
            window_put("\n");
            if (!verbose)
              {
              verbosedump(ppart);
              }
            }
          break;
        }
      }
    else
      {
      if (debug)
        {
        window_put("Unknown part header");
        window_put("\n");
        if (!verbose)
          {
          verbosedump(ppart);
          }
        }
      if (headerlen == 0x0000 && nstrings == 0x0000)
        {
        window_put("<blink>");
        window_put("CONNECTION LOST :-( /*???*/");
        window_put("</blink>");
        window_put("0x000000000000");
        window_nl();
        run = 0;
        }
      }     
    if (verbose)
      {
      verbosedump(ppart);
      }
    }
  else{
    if (debug)
      {
      window_put("Error: NULL ptr given to processpart()");
      window_nl();
      }
    }
  }


part *makemsg(unsigned char *string)
  {
  part *result = NULL;
  
  if (string != NULL)
    {
    if (NULL != (result = calloc(1, sizeof(part))))
      {
      result->next = NULL;
      if (NULL != (result->header = calloc(1, sizeof(short))))
        {
        result->headerlen = 1;
        result->header[0] = 0x019a;
        if (NULL != (result->strings = calloc(1, sizeof(char *))))
          {
          result->nstrings = 1;
          if (NULL == (result->strings[0] = iso2utf8string(string)))
            {
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
  else
    {
    if (debug)
      {
      window_put("Error: NULL ptr given to makemsg()");
      window_nl();
      }
    }
  return result;
  }


void verbosedump(part *dump)
  {
  int i;
  /*static char buffer[6];*/
  
  if (dump != NULL){
    window_put("HEADER:");
    window_nl();
    for (i = 0; i < dump->headerlen; i++)
      {/*
      buffer[3] = inttohex(dump->header[i] & 0x0F);
      buffer[2] = inttohex((dump->header[i] >> 4) & 0x0F);
      buffer[1] = inttohex((dump->header[i] >> 8) & 0x0F);
      buffer[0] = inttohex((dump->header[i] >> 12) & 0x0F);
      buffer[4] = ' ';
      buffer[5] = '\0';
      window_put(buffer);*/
      window_puthex(dump->header[i], 4);
      window_putchar(' ');
      }
    window_nl();
    window_put("STRINGS");
    window_nl();
    for (i = 0; i < dump->nstrings; i++)
      {
      window_put(dump->strings[i]);
      window_nl();
      }
    window_nl();
    }
  else
    {
    if (debug)
      {
      window_put("Error: NULL ptr given to verbosedump()");
      window_nl();
      }
    }
  }



