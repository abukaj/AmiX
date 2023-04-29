/*AmiX - client.c - v. 0.2 - (c) by ABUKAJ (J.M.Kowalski)*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <locale.h>

#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <netdb.h>

#include "polchat.h"
#include "polchat2.h"
#include "interface.h"
#include "temp.h"
#include "version.h"

#define input() 0
#define output() 1

int main(int argc, char *argv[])
{
  int i;
  std::string host = "s1.polchat.pl";
  std::string room = "AmiX";
  std::string reffrom = "http://www.polchat.pl/chat/room.phtml/?room=AmiX";

  int port = 14003;

  int sfd = -1;
  struct sockaddr_in *serv_addr = NULL;
  struct addrinfo hints, *res, *tres;
  struct pollfd pol;
  part *ppart = NULL;
  bool useattr = true;
  bool latin2 = false;
  int nicklistwidth = 30;

  srand(time(NULL));

  
  for (i = 1; i < argc; ++i)
  {
    if (0 == strcmp(argv[i], "?"))
    {
      printf("SERVER/K PORT/K/N ROOM/K NICK/K DEBUG/S VERBOSE/S COREDUMP/S BELL/S"
             " PERIOD/K/N NICKLISTWIDTH/K/N NOATTR/S ASKPASSW/S CHECKUPDATES/S"
             " LOG/K OLDLOG/K ANTIIDLE/K/N NOHTML/S REFFROM/K LATIN2/S\n");
      run = 0;
    }
    else if (0 == strcmp(argv[i], "-help") || 0 == strcmp(argv[i], "--help") || 0 == strcmp(argv[i], "-h"))
    {
      run = 0;
      puts("Usage:");
      puts(argv[0]);
      puts("[-askpassw]");
      puts("[-checkupdates]");
      puts("[-nick nick]");
      puts("[-nicklistwidth width of nicklist]");
      puts("[-noattr]");
      puts("[-period interval]");
      puts("[-port port]");
      puts("[-room room]");
      puts("[-server server]");
      puts("[-log logfilename]");
      puts("[-oldlog logfilename]");
      puts("[-antiidle interval]");
      puts("[-nohtml]");
      puts("[-latin2]");
    }
    else if (0 == strcmp(argv[i], "-askpassw") || 0 == ncsstrcmp(argv[i], "ASKPASSW"))
    {
      askpassw = -1;
    }
    else if (0 == strcmp(argv[i], "-noattr") || 0 == ncsstrcmp(argv[i], "NOATTR"))
    {
      useattr = false;
    }
    else if (0 == strcmp(argv[i], "-latin2") || 0 == ncsstrcmp(argv[i], "LATIN2"))
    {
      latin2 = true;
    }
    else if (0 == strcmp(argv[i], "-checkupdates") || 0 == ncsstrcmp(argv[i], "CHECKUPDATES"))
    {
      cud = -1;
    }
    else if (0 == strcmp(argv[i], "-debug") || 0 == ncsstrcmp(argv[i], "DEBUG"))
    {
      debug = -1;
    }
    else if (0 == strcmp(argv[i], "-coredump") || 0 == ncsstrcmp(argv[i], "COREDUMP"))
    {
      coredump = -1;
    }
    else if (0 == strcmp(argv[i], "-verbose") || 0 == ncsstrcmp(argv[i], "VERBOSE"))
    {
      verbose = -1;
    }
    else if (0 == strcmp(argv[i], "-nohtmlformatting") || 0 == ncsstrcmp(argv[i], "NOHTMLFORMATTING") || 0 == strcmp(argv[i], "-nohtml") || 0 == ncsstrcmp(argv[i], "NOHTML"))
    {
      nohtml = -1;
    }
    else if (0 == strcmp(argv[i], "-bell") || 0 == ncsstrcmp(argv[i], "BELL"))
    {
      bell = -1;
    }
    else if (0 == strcmp(argv[i], "-reffrom") || 0 == ncsstrcmp(argv[i], "REFFROM"))
    {
      i++;
      if (i < argc)
      {
        reffrom = argv[i];
      }                              
    }
    else if (0 == strcmp(argv[i], "-server") || 0 == ncsstrcmp(argv[i], "SERVER"))
    {
      i++;
      if (i < argc)
      {
        host = argv[i];
      }                              
    }
    else if (0 == strcmp(argv[i], "-port") || 0 == ncsstrcmp(argv[i], "PORT"))
    {
      i++;
      if (i < argc)
      {
        port = atoi(argv[i]);
      }                              
    }
    else if (0 == strcmp(argv[i], "-nicklistwidth") || 0 == ncsstrcmp(argv[i], "NICKLISTWIDTH"))
    {
      i++;
      if (i < argc)
      {
        nicklistwidth = atoi(argv[i]);
      }                              
    }
    else if (0 == strcmp(argv[i], "-period") || 0 == ncsstrcmp(argv[i], "PERIOD"))
    {
      i++;
      if (i < argc)
      {
        period = strtod(argv[i], NULL);
      }
    }
    else if (0 == strcmp(argv[i], "-antiidle") || 0 == ncsstrcmp(argv[i], "ANTIIDLE"))
    {
      i++;
      if (i < argc)
      {
        antiidle = atoi(argv[i]);
      }
    }
    else if (0 == strcmp(argv[i], "-room") || 0 == ncsstrcmp(argv[i], "ROOM"))
    {
      i++;
      if (i < argc)
      {
        room = argv[i];
      }                              
    }
    else if (0 == strcmp(argv[i], "-log") || 0 == ncsstrcmp(argv[i], "LOG"))
    {
      i++;
      if (i < argc)
      {
        openlog(argv[i]);
      }
    }
    else if (0 == strcmp(argv[i], "-oldlog") || 0 == ncsstrcmp(argv[i], "OLDLOG"))
    {
      i++;
      if (i < argc)
      {
        openoldlog(argv[i]);
      }
    }
    else if (0 == strcmp(argv[i], "-nick") || 0 == ncsstrcmp(argv[i], "NICK"))
    {
      i++;
      if (i < argc)
      {
        nick = argv[i];
      }                              
    }
    else
    {
      printf("%s - unknown option\n", argv[i]);
    }
  }

  if (!latin2)
  {
    setlocale(LC_ALL, "");
  }

  if (run)
  {
    interface = new amixInterface;

    interface->nicklist_w = nicklistwidth;
    interface->useattr = useattr;
    interface->latin2 = latin2;
    interface->resize();

    interface->window_attron(COLOR_PAIR(7) | A_BOLD);
    interface->put(" " VER " ");
    interface->window_attroff(COLOR_PAIR(7) | A_BOLD);
    interface->nl();
    interface->put(" Linuxowy klient Polchatu");
    interface->nl();
    interface->put(" By ABUKAJ (J.M.Kowalski - amiga@buziaczek.pl)");
    interface->nl();
    interface->put(" status: freeware (badz giftware ;-D)");
    interface->nl();
    interface->put(" autor nie ponosi odpowiedzialnosci za ewentualne");
    interface->nl();
    interface->put(" szkody wywolane uzyciem programu w tej wersji");
    interface->nl();
    interface->put(" Oficjalna strona projektu: http://amix.org.pl");
    interface->nl();
    interface->nl();

    switch (rand() % 3)
    {
      case 0:
        interface->put(" Cieszmy się, że mamy harcerki - to dzięki nim istnieje AmiX.");
        break;
      case 1:
        interface->put(" Wyłącz komputer i wyjdź z domu.");
        interface->nl();
        interface->put(" Może pod twoim blokiem nawalają się magowie.");
        break;
      case 2:
        interface->put("1f j00 (4n 1234d 7|-|15, j00 n33d 70 937 |41d");
        break;
    }
    interface->nl();
    interface->nl();

    interface->print();
    interface->update();

    if (cud)
    {
      checkupdate();
    }
    else
    {
      interface->put(" Sprawdzanie uaktualnien pominiete.");
      interface->nl();
    }
    interface->print();
    interface->update();

    if (askpassw)
    {
      pass = interface->input_password();
    }
    
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);

    interface->put(" resolving...");
    interface->nl();

    interface->print();
    interface->update();
   
    /*resolvuje adres*/
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if (0 == getaddrinfo(host.c_str(), NULL, &hints, &res))
    {
      while (run || connected)
      {
        if (!connected)
        {
          sfd = -1;
          for (tres = res; tres != NULL && connected == 0; tres = tres->ai_next)
          {
            serv_addr = (struct sockaddr_in *) tres->ai_addr;
            serv_addr->sin_port = htons(port);
            if ((sfd = socket(tres->ai_family, tres->ai_socktype, tres->ai_protocol)) != -1)
            {
              interface->put(" connecting...");
              interface->nl();
              interface->print();
              interface->update();

              if ((connect(sfd, tres->ai_addr, tres->ai_addrlen)) < 0)
              {
                interface->put(" Connection failed...");
                interface->nl();
                interface->print();
                interface->update();
                close(sfd);
                sfd = -1;
                continue;
              }
            }
            else
            {
              interface->put(" Unable to create socket...");
              interface->nl();
              interface->print();
              interface->update();
            }
          }
      
          if (sfd >= 0)/*jesli sie polaczylismy*/
          {
            connected = -1;
            interface->put(" connected");
            interface->nl();
            interface->print();
            interface->update();

            if (tosend != NULL)
            {
              delete tosend;
              tosend = NULL;
            }

            //TODO: rethink reconnect
            //if (roomname != NULL)
            //{
            //  ppart = new part(nick, pass, roomname, "", reffrom);
            //}
            //else
            {
              ppart = new part(nick, pass, room, "", reffrom);
            }
            putmsg(ppart);
          }
          else
          {
            interface->put("Unable to connect host: ");
            interface->put(host.c_str());
            interface->put(" ...");
            interface->nl();
          }                               
        }
        else /* if (connected)*/
        {
          /*czy jest cos na gniezdzie?*/
          pol.fd = sfd;
          pol.events = POLLIN;
          pol.revents = 0;
          poll(&pol, 1, 50);
          if (((pol.revents) & POLLIN) == POLLIN)
          {
            if (NULL != (ppart = readpart(sfd)))
            {
              processpart(ppart, sfd);
              delete ppart;
              ppart = NULL;
              interface->print();
            }
          }

          /*czy mamy cos do wyslania?*/
          sendnext(sfd);

          interface->update();
        }
          
        /*czy jest cos na wejsciu?*/
        if (interface->console_input())
        {
          std::string inputstring = interface->get_input();
          if (0 == ncsstrncmp(inputstring, "/quit ", 6) || 0 == ncsstrncmp(inputstring, "/quit", 6))
          {
            ppart = new part(inputstring);
            putmsg(ppart);
          }
          else if (0 == ncsstrncmp(inputstring, "/exit ", 6) || 0 == ncsstrncmp(inputstring, "/exit", 6))
          {
            run = 0;
            inputstring.replace(0, 5, "/quit");
            ppart = new part(inputstring);
            putmsg(ppart);
          }
          else if (0 == ncsstrncmp(inputstring, "/next ", 6) || 0 == ncsstrncmp(inputstring, "/next", 6))
          {
            chatrooms.next();
            interface->print();
          }
          else if (0 == ncsstrncmp(inputstring, "/prev ", 6) || 0 == ncsstrncmp(inputstring, "/prev", 6))
          {
            chatrooms.prev();
            interface->print();
          }
          else if (0 == ncsstrncmp(inputstring, "/debugon ", 9) || 0 == ncsstrncmp(inputstring, "/debugon", 9))
          {
            debug = -1;
            interface->put("DEBUG MODE ON");
            interface->nl();
            interface->print();
          }
          else if (0 == ncsstrncmp(inputstring, "/debugoff ", 10) || 0 == ncsstrncmp(inputstring, "/debugoff", 10))
          {
            debug = 0;
            interface->put("DEBUG MODE OFF");
            interface->nl();
            interface->print();
          }
          else if (0 == ncsstrncmp(inputstring, "/verboseon ", 11) || 0 == ncsstrncmp(inputstring, "/verboseon", 11))
          {
            verbose = -1;
            interface->put("VERBOSE MODE ON");
            interface->nl();
            interface->print();
          }
          else if (0 == ncsstrncmp(inputstring, "/verboseoff ", 12) || 0 == ncsstrncmp(inputstring, "/verboseoff", 12))
          {
            verbose = 0;
            interface->put("VERBOSE MODE OFF");
            interface->nl();
            interface->print();
          }
          else if (0 == ncsstrncmp(inputstring, "/ver ", 5) || 0 == ncsstrncmp(inputstring, "/ver", 5))
          {
            //TODO: rethink it 
            chatrooms.currentroom().addline("<B>" VER "</B>");
            interface->print();
          }
          else if (0 == ncsstrncmp(inputstring, "/help ", 6) || 0 == ncsstrncmp(inputstring, "/help", 6))
          {
            //TODO: retink it
            chatrooms.currentroom().addline("<B>/borg</B> - wysyla tekst: &quot;"
                          "I'm cybernetic organism - living tissue over metal endoskeleton.&quot;");
            chatrooms.currentroom().addline("<B>/exit</B> - komenda analogiczna do polchatowego /quit, ale "
                          "dodatkowo konczy prace programu");
            chatrooms.currentroom().addline("<B>/help</B> - wyswietla tekst pomocy");
            chatrooms.currentroom().addline("<B>/lama</B> - wysyla tekst: &quot;" 
                          "thankfully alert gauchos were able to save the llama "
                          "before it was swept into the blades of the turbine&quot;");
            chatrooms.currentroom().addline("<B>/ver</B> - podaje wersje programu"); 
            interface->print();
          }
          else if ((0 == ncsstrncmp(inputstring, "/part ", 6) || 0 == ncsstrncmp(inputstring, "/part", 6)) && !(*(chatrooms.current)).room)
          {
            chatrooms.part((*(chatrooms.current)).name, true);
            interface->update_all();
          }         
          else
          {
            if (0 == ncsstrncmp(inputstring, "/lama ", 6) || 0 == ncsstrncmp(inputstring, "/lama", 6))
            {
              inputstring = "thankfully alert gauchos were able to save the llama"
                            "before it was swept into the blades of the turbine";
            }
            else if (0 == ncsstrncmp(inputstring, "/borg ", 6) || 0 == ncsstrncmp(inputstring, "/borg", 6))
            {
              inputstring = "I'm cybernetic organism - living tissue over metal endoskeleton.";
            }
            else if (0 == ncsstrncmp(inputstring, "/jedi ", 6) || 0 == ncsstrncmp(inputstring, "/jedi", 6))
            {
              inputstring = "May the Force be with you, my young padawan...";
            }         

            
            if ((*(chatrooms.current)).room)
            {
              ppart = new part(inputstring, chatrooms.currentroom().name);
            }
            else
            {
              inputstring = "/msg " + chatrooms.currentroom().name + " " + inputstring;
              ppart = new part(inputstring);
            }
            putmsg(ppart);
          }
        }
      }
        
      if (tosend != NULL)
      {
        delete tosend;
      }

      if (connected)
      {
        connected = 0;
        close(sfd);
      }
        
    }
    else
    {
      interface->put("Resolver problem...");
      interface->nl();
      interface->print();
      interface->update();
    } 
    
    delete interface;
  }
 
  closelog(); 
  puts("AmiX: Koniec pracy na dzis, polecam sie na przyszlosc");
  return 0;
}
