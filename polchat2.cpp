/*AmiX - polchat2.c - v. 0.1 - (c) by ABUKAJ (J.M.Kowalski)*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

#include <ncursesw/ncurses.h>

#include "polchat.h"
#include "polchat2.h"
#include "interface.h"
#include "temp.h"
#include "version.h"

part *tosend = NULL;
time_t last = 0;


part *readpart(int sfd)
{
  tank *tnk = readtank(sfd);

  if (NULL != tnk)
  {
    part * result = new part(*tnk);
    delete tnk;
    return result;
  }
  else
  {
    interface->put("ERROR: unable to read tank");
    interface->nl();
  }
  return NULL;
}


part::part(tank &prt)
{
  int ptr = 0;

  int size = prt.len();

  this->next = NULL;
  const char * data = prt.get();

  this->headerlen = ((unsigned char) data[ptr++]) << 8;
  this->headerlen |= (unsigned char) data[ptr++];
  this->nstrings = ((unsigned char) data[ptr++]) << 8;
  this->nstrings |= (unsigned char) data[ptr++];

  this->header = new short int [this->headerlen];

  for (int i = 0; i < this->headerlen; i++)
  {
    this->header[i] = ((unsigned char) data[ptr++]) << 8;
    this->header[i] |= (unsigned char) data[ptr++];
  }

  this->strings = new std::string [this->nstrings];

  for (int i = 0; i < this->nstrings; i++)
  {
    int len = ((unsigned char) data[ptr++]) << 8;
    len |= (unsigned char) data[ptr++];
    this->strings[i] = std::string(data + ptr, len);
    ptr += len + 1;
  }

  if (ptr != size)
  {
    if (debug)
    {
      interface->put("Tank parse error!");
      /*partdump(prt);*/
    }
  }
}


//welcome message
part::part(std::string nick,
           std::string pass,
           std::string room,
           std::string roompass,
           std::string referer="http://www.polchat.pl/chat/room.phtml/?room=Sekciarz")
{
  this->next = NULL;

  this->headerlen = 1;
  this->header = new short int [1];
  this->header[0] = 0x0578;

  this->nstrings = 8;
  this->strings =new std::string [8];

  this->strings[0] = nick;
  this->strings[1] = pass;
  this->strings[2] = roompass;
  this->strings[3] = room;
  this->strings[4] = referer;
  this->strings[5] = "polchat.pl";
  this->strings[6] = "nlst=1&nnum=1&jlmsg=true&ignprv=false";
  this->strings[7] = VER;
}

//message
//part::part(const char * string)
//{
//  this->next = NULL;
//
//  this->header = new short int [1];
//
//  this->headerlen = 1;
//  this->header[0] = 0x019a;
//
//  this->strings = new std::string [1];
//  this->nstrings = 1;
//  this->strings[0] = std::string(string);
//}

part::part(std::string & str)
{
  this->next = NULL;

  this->header = new short int [1];

  this->headerlen = 1;
  this->header[0] = 0x019a;

  this->strings = new std::string [1];
  this->nstrings = 1;
  this->strings[0] = str;
}

//message
//part::part(const char * string, std::string & room)
//{
//  this->next = NULL;
//
//  this->header = new short int [1];
//
//  this->headerlen = 1;
//  this->header[0] = 0x019a;
//
//  this->strings = new std::string [2];
//  this->nstrings = 2;
//  this->strings[0] = std::string(string);
//  this->strings[1] = room;
//}

//message
part::part(std::string & str, std::string & room)
{
  this->next = NULL;

  this->header = new short int [1];

  this->headerlen = 1;
  this->header[0] = 0x019a;

  this->strings = new std::string [2];
  this->nstrings = 2;
  this->strings[0] = str;
  this->strings[1] = room;
}

void part::dump()
{
  interface->put("HEADER: ");
  //interface->nl();
  for (int i = 0; i < this->headerlen; i++)
  {
    interface->puthex(this->header[i], 4);
    interface->putchar(' ');
  }

  interface->nl();
  interface->put("STRINGS ");
  interface->puthex(this->nstrings, 4);
  interface->nl();
  for (int i = 0; i < this->nstrings; i++)
  {
    interface->put(this->strings[i].c_str());
    interface->nl();
  }
  //interface->nl();
}

part::~part()
{
  if (this->next !=NULL)
  {
    delete this->next;
  }

  if (this->strings != NULL)
  {
    delete [] this->strings;
  }

  if (this->header != NULL)
  {
    delete [] this->header;
  }
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
            interface->put("Unknown part header");
            interface->nl();
            if (!verbose)
            {
              ppart->dump();
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
              interface->put("Unknown part header");
              interface->nl();
              if (!verbose)
              {
                ppart->dump();
              }
            }
          }     
          break;
        case 0x0262:/*MSG*/                
          if (headerlen == 0x0001 && nstrings == 0x0002)
          {
            if (bell)
            {
              beep();
              /*interface->put("\a");
              interface->nl();*/
            }
            if (!verbose)
            {
              chatrooms.roommsg(ppart->strings[1], ppart->strings[0]);
              printlog("-msg-", ppart->strings[0], ppart->strings[1]);
            }
          }
          else
          {
            if (debug)
            {
              interface->put("Unknown part header");
              interface->nl();
              if (!verbose)
              {
                ppart->dump();
              }
            }
          }
          break;
        case 0x0263:/*Priv. msg*/
          if (headerlen == 0x0001 && nstrings == 0x0002)
          {
            //interface->put(ppart->strings[1].c_str()); interface->nl();

            chatrooms.privmsg(ppart->strings[1], ppart->strings[0]);
            //priv(PRIV_FROM, ppart->strings[1].c_str(), ppart->strings[0].c_str());
          }
          else if (headerlen == 0x0001 && nstrings == 0x0003)
          {
            chatrooms.privmsg(ppart->strings[2], ppart->strings[0]);
            //priv(PRIV_TO, ppart->strings[2].c_str(), ppart->strings[0].c_str());
          }
          else
          {
            if (debug)
            {
              interface->put("Unknown part header");
              interface->nl();
              if (!verbose)
              {
                ppart->dump();
              }
            }
          }
          break;
        case 0x0266: /*client config*/
          if (headerlen == 0x0001 || nstrings == 0x0001)
          {
            if (debug)
            {
              interface->put("CLIENT CONFIG: ");
              interface->put(ppart->strings[0].c_str());
              interface->nl();
            }
          }
          else
          {
            if (debug)
            {
              interface->put("Unknown part header");
              interface->nl();
              if (!verbose)
              {
                ppart->dump();
              }
            }
          }
          break;
        case 0x0267:/*nick entered*/
          if (headerlen == 0x0002 && nstrings == 0x0003)
          {
            std::string &room = ppart->strings[1];

            chatrooms.room[room].addnick(ppart->strings[0],
                                         ppart->strings[2],
                                         ppart->header[1]);
            if ((*(chatrooms.current)).name == room)
            {
              interface->printnicklist();
            }
            if ((ppart->header[1] & 0x00ff8c) != 0x0000 && debug)
            {
              interface->put("Unknown status of: ");
              interface->put(ppart->strings[0].c_str());
              interface->put(" : ");
              interface->puthex(ppart->header[1], 4);
              interface->nl();
            }
          }
          else
          {
            if (debug)
            {
              interface->put("Unknown part header");
              interface->nl();
              if (!verbose)
              {
                ppart->dump();
              }
            }
          }
          break;
        case 0x0268:/*nick left*/
          if (headerlen == 0x0001 && nstrings == 0x0002)
          {
            std::string &room = ppart->strings[1];

            chatrooms.room[room].removenick(ppart->strings[0]);
            if ((*(chatrooms.current)).name == room)
            {
              interface->printnicklist();
            }
          }
          else
          {
            if (debug)
            {
              interface->put("Unknown part header");
              interface->nl();
              if (!verbose)
              {
                ppart->dump();
              }
            }
          }
          break;
        case 0x0269:/*NICK update*/
          if (headerlen == 0x0002 && nstrings == 0x0002)
          {
            std::string &room = ppart->strings[1];

            chatrooms.room[room].updatenick(ppart->strings[0], ppart->header[1]);
            if ((*(chatrooms.current)).name == room)
            {
              interface->printnicklist();
            }
            if ((ppart->header[1] & 0x00ff8c) != 0x0000 && debug)
            {
              interface->put("Unknown status of: ");
              interface->put(ppart->strings[0].c_str());
              interface->put(" : ");
              interface->puthex(ppart->header[1], 4);
              interface->nl();
            }
          }
          else
          {
            if (debug)
            {
              interface->put("Unknown part header");
              interface->nl();
              if (!verbose)
              {
                ppart->dump();
              }  
            }
          }
          break;
        case 0x026a:/*I have absolutly no idea - chyba ze wlazlem jako ja???*/
          if (headerlen == 0x0002 && nstrings == 0x0001)
          {
            if ((verbose || debug) && ppart->header[1] != 0x0004)
            {
              interface->put(" 0004 != ");
              interface->puthex(ppart->header[1], 4);
              interface->nl();
              interface->put(" NICK:");
              interface->nl();
              if (!verbose)
              {
                interface->put(ppart->strings[0].c_str());
                interface->nl();
              }
            }
          }
          else
          {
            if (debug)
            {
              interface->put("Unknown part header");
              interface->nl();
              if (!verbose)
              {
                ppart->dump();
              }
            }
          }
          break;
        case 0x026b:/*nicklist*/
          if (headerlen >= 5 && ppart->header[1] == 0x0001 &&
            ppart->header[2] == 0x0001 && ppart->header[3] == 0x0001 &&
            ppart->header[4] == 0x0000)
          {
            chatroom & room = chatrooms.room[ppart->strings[0]];
            for (i = 1; i < nstrings; i += 2)
            {
              room.addnick(ppart->strings[i],
                           ppart->strings[i + 1],
                           ppart->header[i + 4]);
              if (((ppart->header[i + 4] & 0x00ff8c) != 0x0000 || ppart->header[i + 5] != 0x0000) && debug)
              {
                interface->put("Unknown status of: ");
                interface->put(ppart->strings[i].c_str());
                interface->put(" : ");
                interface->puthex(ppart->header[i + 4], 4);
                interface->put(" : ");
                interface->puthex(ppart->header[i + 5], 4);
                interface->nl();
              }
            }
            if (room.name == (*(chatrooms.current)).name)
            {
              interface->printnicklist();
            }
          }
          else
          {
            if (debug)
            {
              interface->put("Unknown part header");
              interface->nl();
              if (!verbose)
              {
                ppart->dump();
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
              interface->put("W pokoju: 0x");
              interface->puthex(nickn, 4);
              interface->nl();
            }
          }
          else
          {
            if (debug)
            {
              interface->put("Unknown part header");
              interface->nl();
              if (!verbose)
              {
                ppart->dump();
              }
            }
          }
          break;
        case 0x0271: /*ROOMINFO*/
          if (headerlen == 0x0002 && nstrings == 0x0002)
          {
            chatrooms.room[ppart->strings[0]].setdesc(ppart->strings[1]);
            interface->printtitle();
          }
          else
          {
            if (debug)
            {
              interface->put("Unknown part header");
              interface->nl();
              if (!verbose)
              {
                ppart->dump();
              }
            }
          }
          break;
        case 0x0272:/*PIERDOLY... konfiguracja pokoju ;-D*/
          if (headerlen == 0x0001 && nstrings == 0x0002)
          {
            if (NULL != (ptr = strstr((char *) ppart->strings[0].c_str(), "color_user=")))
            {
              ptr += 11;
              sscanf(ptr, "#%x", &tmp);
              colourt[0] = transformrgb((tmp >> 16) & 0x00FF, (tmp >> 8) & 0x00FF, tmp & 0x00FF);
            }
            if (NULL != (ptr = strstr((char *) ppart->strings[0].c_str(), "color_op=")))
            {
              ptr += 9;
              sscanf(ptr, "#%x", &tmp);
              colourop = transformrgb((tmp >> 16) & 0x00FF, (tmp >> 8) & 0x00FF, tmp & 0x00FF);
            }
            if (NULL != (ptr = strstr((char *) ppart->strings[0].c_str(), "color_guest=")))
            {
              ptr += 12;
              tmp = sscanf(ptr, "#%x #%x #%x #%x #%x #%x #%x", &tempt[0],
                           &tempt[1], &tempt[2], &tempt[3], &tempt[4], &tempt[5],
                           &tempt[6]);
              for (i = 0; i < tmp; i++)
              {
                colourt[i + 1] = transformrgb((tempt[i] >> 16) & 0x00FF, (tempt[i] >> 8) & 0x00FF, tempt[i] & 0x00FF);
              }
            }
          }    
          else
          {
            if (debug)
            {
              interface->put("Unknown part header");
              interface->nl();
              if (!verbose)
              {
                ppart->dump();
              }
            }
          }
          break;
        case 0x0276:/*MSG powitalny*/
          if (headerlen == 0x0001 && nstrings == 0x0002)
          {
            chatrooms.join(ppart->strings[1]);
            if (!verbose)
            {
              chatrooms.roommsg(ppart->strings[1], ppart->strings[0]);
              printlog("-hi-", ppart->strings[0], ppart->strings[1]);
              interface->colouroff();
            }
          }
          else
          {
            if (debug)
            {
              interface->put("Unknown part header");
              interface->nl();
              if (!verbose)
              {
                ppart->dump();
              }
            }
          }
          break;
        case 0x0277:/*MSG pozegnalny*/
          if (headerlen == 0x0001 && nstrings == 0x0002)
          {
            chatrooms.part(ppart->strings[1], false);
            interface->update_all();

            if (!verbose)
            {
              chatrooms.currentroom().msg(ppart->strings[0], true);
              printlog("-bye-", ppart->strings[0], ppart->strings[1]);
              interface->colouroff();
            }
          }
          else
          {
            if (debug)
            {
              interface->put("Unknown part header");
              interface->nl();
              if (!verbose)
              {
                ppart->dump();
              }
            } 
          }
          break;
        case 0x0591:
          if (debug && !verbose)
          {
            interface->put("REKLAMY");
            interface->nl();
            ppart->dump();
          }
          break;
        case (short) 0xffff:/*MSG -*/
          if (headerlen == 0x0001 && nstrings == 0x0001)
          {
            if (!verbose)
            {
              //TODO: rethink it
              chatrooms.currentroom().addline(ppart->strings[0].c_str());
              line & tmp = chatrooms.currentroom().lines.back();
              interface->put(tmp.timestring.c_str());
              interface->printpol(tmp.text.c_str());

              std::string dummy = "";

              printlog("---", ppart->strings[0], dummy);
            }
            connected = 0;
            close(sfd);
            if (0 == strncmp("nieprawidłowe hasło i/lub identyfikator użytkownika", ppart->strings[0].c_str(), 9))
            {
              pass = interface->input_password();
            }
          }
          else
          {
            if (debug)
            {
              interface->put("Unknown part header");
              interface->nl();
              if (!verbose)
              {
                ppart->dump();
              }
            }
          }
          break;
        default:
          if (debug)
          {
            interface->put("Unknown part header");
            interface->nl();
            if (!verbose)
            {
              ppart->dump();
            }
          }
          break;
      }
    }
    else
    {
      if (debug)
      {
        interface->put("Unknown part header");
        interface->nl();
        if (!verbose)
        {
          ppart->dump();
        }
      }
      if (headerlen == 0x0000 && nstrings == 0x0000)
      {
        interface->put("<blink>");
        interface->put("CONNECTION LOST :-( /*???*/");
        interface->put("</blink>");
        interface->put("0x000000000000");
        interface->nl();
        connected = 0;
        close(sfd);
      }
    }     
    if (verbose)
    {
      ppart->dump();
    }
  }
  else
  {
    if (debug)
    {
      interface->put("Error: NULL ptr given to processpart()");
      interface->nl();
    }
  }
}


int sendpol(part *ppart, int sfd)
{
  int size = 4;/*8*/
  int i;
  int tmp = 0;
  int ptr = 0;

  if (NULL != ppart)
  {
    size += 2 * ppart->headerlen;
    for (i = 0; i < ppart->nstrings; i++)
    {
      size += ppart->strings[i].length() + 3;
    }
    char * data = new char[size];

    if (NULL != data)
    {
      data[ptr++] = (char) (ppart->headerlen / 256);
      data[ptr++] = (char) (ppart->headerlen % 256);
      data[ptr++] = (char) (ppart->nstrings / 256);
      data[ptr++] = (char) (ppart->nstrings % 256);

      for (i = 0; i < ppart->headerlen; i++)
      {
        data[ptr++] = (char) (ppart->header[i] / 256);
        data[ptr++] = (char) (ppart->header[i] % 256);
      }

      for (i = 0; i < ppart->nstrings; i++)
      {
        tmp = ppart->strings[i].length();
        data[ptr++] = (char) (tmp / 256);
        data[ptr++] = (char) (tmp % 256);
        strcpy(data + ptr, ppart->strings[i].c_str());
        ptr += tmp;
        data[ptr++] = '\0';
      }

      tank * result = new tank(size, data);

      if (result != NULL)
      {
        result->send(sfd);
        delete result;
      } 

      if (ptr != size)
      {
        if (debug)
        {
          interface->put("Error: ptr != size");
          interface->nl();
        }
      }
      delete [] data;
    }
    else
    {
      if (debug)
      {
        interface->put("Error: unable to allocate memory for tank");
        interface->nl();
      }
    }
  }
  else
  {
    if (debug)
    {
      interface->put("Error: NULL ptr given to sendpol");
      interface->nl();
    }
  }
  return 0;
}


void putmsg(part *msg)
{
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
    if (connected && (period < difftime(time(NULL), last)))
    {
      sendpol(tosend, sfd);
      tmp = tosend->next;
      tosend->next = NULL;
      delete tosend;
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
        std::string noop = "/noop";
        if (NULL != (tmp = new part(noop)))
        {
          sendpol(tmp, sfd);
          delete tmp;
          last = time(NULL);
        }
      }
    }
  }
}
