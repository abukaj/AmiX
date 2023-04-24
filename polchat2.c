
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

#include <ncurses.h>

#include "polchat.h"
#include "polchat2.h"
#include "interfeace.h"
#include "temp.h"
#include "version.h"

part *tosend = NULL;
time_t last = 0;


part *readpart(int sfd)
  {
  int size;
  int ptr = 0;
  int i;
  part *result = NULL;
  tank *tnk;

  if (NULL != (tnk = readtank(sfd)))
    {
    /*result = parsetank(tnk);*/
    if (NULL != (result = malloc(sizeof(part))))
      {
      result->next = NULL;
      size = tnk->length;

      result->headerlen = ((unsigned char) tnk->data[ptr++]) << 8;
      result->headerlen |= (unsigned char) tnk->data[ptr++];
      result->nstrings = ((unsigned char) tnk->data[ptr++]) << 8;
      result->nstrings |= (unsigned char) tnk->data[ptr++];
      if (NULL != (result->header = calloc(result->headerlen, sizeof(short))))
        {
        for (i = 0; i < result->headerlen; i++)
          {
          result->header[i] = ((unsigned char) tnk->data[ptr++]) << 8;
          result->header[i] |= (unsigned char) tnk->data[ptr++];
          }
        if (NULL != (result->strings = calloc(result->nstrings, sizeof(char *))))
          {
          for (i = 0; i < result->nstrings; i++)
            {
            result->strings[i] = NULL;
            }
          for (i = 0; i < result->nstrings; i++)
            {
            if (NULL == (result->strings[i] = unwrapstring(ptr + tnk->data)))
              {
              freepart(&result);
              return NULL;
              }
            ptr += wrapsize((char *) (tnk->data + ptr));
            }
          }
        else
          {
          free(result->header);
          free(result);
          return NULL;
          }
        }
      else
        {
        free(result);
        return NULL;
        }
      }
    if (ptr != size)
      {
      if (debug)
        {
        window_put("ERROR: tank parse error!");
        window_nl();
        /*partdump(prt);*/
        }
      }
    if (coredump)
      {
      tankdump(tnk);
      }
    freetank(&tnk);
    }
  else
    {
    window_put("ERROR: unable to read tank");
    window_nl();
    }
  return result;
  }


part *parsetank(tank *prt)
  {
  int size;
  int ptr = 0;
  int i;
  part *result = NULL;

  if (NULL != prt)
    {
    if (NULL != (result = malloc(sizeof(part))))
      {
      result->next = NULL;
      size = prt->length;

      result->headerlen = ((unsigned char) prt->data[ptr++]) << 8;
      result->headerlen |= (unsigned char) prt->data[ptr++];
      result->nstrings = ((unsigned char) prt->data[ptr++]) << 8;
      result->nstrings |= (unsigned char) prt->data[ptr++];
      if (NULL != (result->header = calloc(result->headerlen, sizeof(short))))
        {
        for (i = 0; i < result->headerlen; i++)
          {
          result->header[i] = ((unsigned char) prt->data[ptr++]) << 8;
          result->header[i] |= (unsigned char) prt->data[ptr++];
          }
        if (NULL != (result->strings = calloc(result->nstrings, sizeof(char *))))
          {
          for (i = 0; i < result->nstrings; i++)
            {
            result->strings[i] = NULL;
            }

          for (i = 0; i < result->nstrings; i++)
            {
            if (NULL == (result->strings[i] = unwrapstring(ptr + prt->data)))
              {
              freepart(&result);
              return NULL;
              }
            ptr += wrapsize((char *) (prt->data + ptr));
            }
          }
        else
          {
          free(result->header);
          free(result);
          return NULL;
          }
        }
      else
        {
        free(result);
        return NULL;
        }
      }
    if (ptr != size)
      {
      if (debug)
        {
        window_put("Tank parse error!");
        /*partdump(prt);*/
        }
      }
    }
  else
    {
    if (debug)
      {
      window_put("Error: NULL ptr given to parsetank()");
      window_nl();
      }
    }
  return result;
  }


part *welcome(char *nick, char *pass, char *room, char *roompass, char *referer)
  {
  char *nw = NULL, *pw = NULL, *rw = NULL, *rpw = NULL;
  char *ref = NULL, *p2 = NULL, *p3 = NULL, *klient = NULL;
  part *result = NULL;

  if (NULL != (nw = clonestring(nick)))
    {
    if (NULL != (pw = clonestring(pass)))
      {
      if (NULL != (rw = clonestring(room)))
        {
        if (NULL != (ref = clonestring(referer)))
          {
          if (NULL != (p2 = clonestring("polchat.pl")))
            {
            if (NULL != (p3 = clonestring("nlst=1&nnum=1&jlmsg=true&ignprv=false")))
              {
              if (NULL != (klient = clonestring(VER)))
                {
                if (NULL != (rpw = clonestring(roompass)))
                  {
                  strcpy(rpw, roompass);
                  if (NULL != (result = calloc(1, sizeof(part))))
                    {
                    result->headerlen = 1;
                    result->nstrings = 8;                      
                    if (NULL != (result->header = calloc(1, sizeof(short))))
                      {
                      result->header[0] = 0x0578;
                      if (NULL != (result->strings = calloc(8, sizeof(char *))))
                        {
                        result->strings[0] = nw;
                        result->strings[1] = pw;
                        result->strings[2] = rpw;
                        result->strings[3] = rw;
                        result->strings[4] = ref;
                        result->strings[5] = p2;
                        result->strings[6] = p3;
                        result->strings[7] = klient;
                        }
                      else
                        {
                        free(result->header);
                        free(result);
                        result = NULL;    
                        }
                      }
                    else
                      {
                      free(result);
                      result = NULL;
                      }
                    }                  
                  if (result == NULL) free(rpw);
                  }
                if (result == NULL) free(klient);
                }
              if (result == NULL) free(p3);
              }
            if (result == NULL) free(p2);
            }
          if (result == NULL) free(ref);
          }
        if (result == NULL) free(rw);
        }
      if (result == NULL) free(pw);
      }
    if (result == NULL) free(nw);
    }
  return result;
  }



part *welcome3(char *nick, char *pass, char *room, char *roompass)
  {
  char *nw = NULL, *pw = NULL, *rw = NULL, *rpw = NULL;
  char *p1 = NULL, *p2 = NULL, *p3 = NULL, *klient = NULL;
  part *result = NULL;

  if (NULL != (nw = calloc(strlen(nick) + 1, sizeof(char))))
    {
    strcpy(nw, nick);
    if (NULL != (pw = calloc(strlen(pass) + 1, sizeof(char))))
      {
      strcpy(pw, pass);
      if (NULL != (rw = calloc(strlen(room) + 1, sizeof(char))))
        {
        strcpy(rw, room);
        if (NULL != (p1 = clonestring("http://www.polchat.pl/chat/room.phtml/?room=Sekciarz")))
          {
          if (NULL != (p2 = calloc(strlen("polchat.pl") + 1, sizeof(char))))
            {
            strcpy(p2, "polchat.pl");  
            if (NULL != (p3 = calloc(strlen("nlst=1&nnum=1&jlmsg=true&ignprv=false") + 1, sizeof(char))))
              {
              strcpy(p3, "nlst=1&nnum=1&jlmsg=true&ignprv=false");
              if (NULL != (klient = calloc(strlen(VER) + 1, sizeof(char))))
                {
                strcpy(klient, VER);
                if (NULL != (rpw = calloc(strlen(roompass) + 1, sizeof(char))))
                  {
                  strcpy(rpw, roompass);
                  if (NULL != (result = calloc(1, sizeof(part))))
                    {
                    result->headerlen = 1;
                    result->nstrings = 8;                      
                    if (NULL != (result->header = calloc(1, sizeof(short))))
                      {
                      result->header[0] = 0x0578;
                      if (NULL != (result->strings = calloc(8, sizeof(char *))))
                        {
                        result->strings[0] = nw;
                        result->strings[1] = pw;
                        result->strings[2] = rpw;
                        result->strings[3] = rw;
                        result->strings[4] = p1;
                        result->strings[5] = p2;
                        result->strings[6] = p3;
                        result->strings[7] = klient;
                        }
                      else
                        {
                        free(result->header);
                        free(result);
                        result = NULL;    
                        }
                      }
                    else
                      {
                      free(result);
                      result = NULL;
                      }
                    }                  
                  if (result == NULL) free(rpw);
                  }
                if (result == NULL) free(klient);
                }
              if (result == NULL) free(p3);
              }
            if (result == NULL) free(p2);
            }
          if (result == NULL) free(p1);
          }
        if (result == NULL) free(rw);
        }
      if (result == NULL) free(pw);
      }
    if (result == NULL) free(nw);
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
  char *ptr;
  unsigned int tmp;
  unsigned int tempt[7];
  
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
            window_nl();
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
              window_nl();
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
              beep();
              /*window_put("\a");
              window_nl();*/
              }
            if (!verbose)
              {
              window_addstr(ppart->strings[0]);
              printlog("-msg-", ppart->strings[0]);
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
        case 0x0263:/*Priv. msg*/
          if (headerlen == 0x0001 && nstrings == 0x0002)
            {
            priv(PRIV_FROM, ppart->strings[1], ppart->strings[0]);
            }
          else if (headerlen == 0x0001 && nstrings == 0x0003)
            {
            priv(PRIV_TO, ppart->strings[2], ppart->strings[0]);
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
        case 0x0266: /*client config*/
          if (headerlen == 0x0001 || nstrings == 0x0001)
            {
            if (debug)
              {
              window_put("CLIENT CONFIG: ");
              window_put(ppart->strings[0]);
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
        case 0x026a:/*I have absolutly no idea - chyba ze wlazlem jako ja???*/
          if (headerlen == 0x0002 && nstrings == 0x0001)
            {
            if ((verbose || debug) && ppart->header[1] != 0x0004)
              {
              window_put(" 0004 != ");
              window_puthex(ppart->header[1], 4);
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
              window_nl();
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
              window_nl();
              if (!verbose)
                {
                verbosedump(ppart);
                }
              }
            }
          break;
        case 0x026C: /*#nicks*/
          if (headerlen == 0x0002 && nstrings == 0x0000)
            {
            nickn = ppart->header[1];
            if (debug)
              {
              window_put("W pokoju: 0x");
              window_puthex(nickn, 4);
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
              window_nl();
              if (!verbose)
                {
                verbosedump(ppart);
                }
              }
            }
          break;
        case 0x0272:/*PIERDOLY... konfiguracja pokoju ;-D*/
          if (headerlen == 0x0001 && nstrings == 0x0002)
            {
            if (NULL != (ptr = strstr(ppart->strings[0], "color_user=")))
              {
              ptr += 11;
              sscanf(ptr, "#%x", &tmp);
              colourt[0] = transformrgb((tmp >> 16) & 0x00FF, (tmp >> 8) & 0x00FF, tmp & 0x00FF);
              }
            if (NULL != (ptr = strstr(ppart->strings[0], "color_op=")))
              {
              ptr += 9;
              sscanf(ptr, "#%x", &tmp);
              colourop = transformrgb((tmp >> 16) & 0x00FF, (tmp >> 8) & 0x00FF, tmp & 0x00FF);
              }
            if (NULL != (ptr = strstr(ppart->strings[0], "color_guest=")))
              {
              ptr += 12;
              tmp = sscanf(ptr, "#%x #%x #%x #%x #%x #%x #%x", &tempt[0],
                           &tempt[1], &tempt[2], &tempt[3], &tempt[4], &tempt[5],
                           &tempt[6]);
              for (i = 0; i <tmp; i++)
                {
                colourt[i + 1] = transformrgb((tempt[i] >> 16) & 0x00FF, (tempt[i] >> 8) & 0x00FF, tempt[i] & 0x00FF);
                }
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
        case 0x0276:/*MSG powitalny*/
          if (headerlen == 0x0001 && nstrings == 0x0001)
            {
            if (!verbose)
              {
              window_addstr(ppart->strings[0]);
              printlog("-hi-", ppart->strings[0]);
              window_colouroff();
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
        case 0x0277:/*MSG pozegnalny*/
          if (headerlen == 0x0001 && nstrings == 0x0001)
            {
            freenicklist(&nicks);
            printnicks(nicks);
            if (!verbose)
              {
              window_addstr(ppart->strings[0]);
              printlog("-bye-", ppart->strings[0]);
              window_colouroff();
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
        case 0x0591:
          if (debug && !verbose)
            {
            window_put("REKLAMY");
            window_nl();
            verbosedump(ppart);
            }
          break;
        case (short) 0xffff:/*MSG -*/
          if (headerlen == 0x0001 && nstrings == 0x0001)
            {
            if (!verbose)
              {
              window_addstr(ppart->strings[0]);
              printlog("---", ppart->strings[0]);
              }
            connected = 0;
            close(sfd);
            if (0 == strncmp("nieprawidłowe hasło i/lub identyfikator użytkownika", ppart->strings[0], 9))
              {
              if (pass != NULL)
                {
                free(pass);
                }
              pass = input_password();
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
        default:
          if (debug)
            {
            window_put("Unknown part header");
            window_nl();
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
        window_nl();
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
        connected = 0;
        close(sfd);
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


part *makemsg(char *string)
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
          if (NULL == (result->strings[0] = clonestring(string)/*iso2utf8string(string)*/))
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
  
  if (dump != NULL){
    window_put("HEADER:");
    window_nl();
    for (i = 0; i < dump->headerlen; i++)
      {
      window_puthex(dump->header[i], 4);
      window_putchar(' ');
      }
    window_nl();
    window_put("STRINGS ");
    window_puthex(dump->nstrings, 4);
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


void freepart(part **p)
  {
  int i;

  if (*p != NULL)
    {
    freepart(&((*p)->next));
    if ((*p)->header != NULL)
      {
      free((*p)->header);
      }
    if ((*p)->strings != NULL)
      {
      for (i = 0; i < (*p)->nstrings; i++)
        {
        if ((*p)->strings[i] != NULL)
          {
          free((*p)->strings[i]);
          }
        }
      free((*p)->strings);
      }
    free(*p);
    *p = NULL;
    }
  }


int sendpol(part *ppart, int sfd){
  tank *result;
  int size = 4;/*8*/
  int i;
  int tmp = 0;
  int ptr = 0;

  if (NULL != ppart)
    {
    size += 2 * ppart->headerlen;
    for (i = 0; i < ppart->nstrings; i++)
      {
      size += strlen(ppart->strings[i]) + 3;
      }
    if (NULL != (result = malloc(sizeof(tank))))
      {
      if (NULL != (result->data = malloc(size)))
        {
        result->length = size;
        result->data[ptr++] = (char) (ppart->headerlen / 256);
        result->data[ptr++] = (char) (ppart->headerlen % 256);
        result->data[ptr++] = (char) (ppart->nstrings / 256);
        result->data[ptr++] = (char) (ppart->nstrings % 256);
        for (i = 0; i < ppart->headerlen; i++)
          {
          result->data[ptr++] = (char) (ppart->header[i] / 256);
          result->data[ptr++] = (char) (ppart->header[i] % 256);
          }
        for (i = 0; i < ppart->nstrings; i++)
          {
          tmp = strlen(ppart->strings[i]);
          result->data[ptr++] = (char) (tmp / 256);
          result->data[ptr++] = (char) (tmp % 256);
          strcpy(result->data + ptr, ppart->strings[i]);
          ptr += tmp;
          result->data[ptr++] = '\0';
          }
        sendtank(result, sfd);
        freetank(&result);
        if (ptr != size)
          {
          if (debug)
            {
            window_put("Error: ptr != size");
            window_nl();
            }
          }
        }
      else
        {
        freetank(&result);
        if (debug)
          {
          window_put("Error: unable to allocate 0x");
          window_puthex(size, 4);
          window_put("bytes of memory");
          window_nl();
          }
        }
      }
    else{
      if (debug)
        {
        window_put("Error: unable to allocate memory for tank");
        window_nl();
        }
      }
    }
  else{
    if (debug){
      window_put("Error: NULL ptr given to sendpol");
      window_nl();
      }
    }
  return 0;
  }


void putmsg(part *msg){
  part **tmp;

  tmp = &tosend;
  while (*tmp != NULL)
    {
    tmp = &((*tmp)->next);
    }
  *tmp = msg;
  }


void sendnext(int sfd)
  {
  part *tmp;

  if (last == 0)
    {
    last = time(NULL);
    }
  if (NULL != tosend)
    {
    if (connected && (period < difftime(time(NULL), last))){
      sendpol(tosend, sfd);
      tmp = tosend->next;
      tosend->next = NULL;
      freepart(&tosend);
      tosend = tmp;
      last = time(NULL);
      }
    }
  else
    {
    if (antiidle)
      {
      if (connected && (antiidle < difftime(time(NULL), last)))
        {
        if (NULL != (tmp = makemsg("/noop")))
          {
          sendpol(tmp, sfd);
          freepart(&tmp);
          last = time(NULL);
          }
        }
      }
    }
  }
