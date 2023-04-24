/*AmiX - client.c - v. 0.2 - (c) by ABUKAJ (J.M.Kowalski)*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <netdb.h>

#include "polchat.h"
#include "polchat2.h"
#include "interfeace.h"
#include "temp.h"
#include "version.h"

#define input() 0
#define output() 1


int main(int argc, char *argv[])
  {
  int i;
  char *host = NULL;
  char *room = NULL;
  char *nick = NULL;
  char *pass = NULL;
  char *logfn = NULL;
  char ol = -1;
  int port = 14003;
  char *inputstring;

  int sfd = -1;
  struct sockaddr_in *serv_addr = NULL;
  struct addrinfo hints, *res, *tres;
  struct pollfd pol;
  char *prt = NULL;
  part *ppart = NULL;  
  
  for (i = 1; i < argc; ++i)
    {
    if (0 == strcmp(argv[i], "?"))
      {
      printf("SERVER/K PORT/K/N ROOM/K NICK/K PASSWORD/K DEBUG/S VERBOSE/S BELL/S"
             " PERIOD/K/N NICKLISTWIDTH/K/N NOATTR/S ASKPASSW/S CHECKUPDATES/S"
             " LOG/K OLDLOG/K ANTIIDLE/K/N NOHTML/S\n");
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
      puts("[-password password]");
      puts("[-period interval]");
      puts("[-port port]");
      puts("[-room room]");
      puts("[-server server]");
      puts("[-log logfilename]");
      puts("[-oldlog logfilename]");
      puts("[-antiidle interval]");
      puts("[-nohtml]");
      }
    else if (0 == strcmp(argv[i], "-askpassw") || 0 == ncsstrcmp(argv[i], "ASKPASSW"))
      {
      /*usuniete ze wzgledow bezpieczenstwa*/
      /*
      if (pass != NULL)
        {
        free(pass);
        }
      puts("Password:");
      pass = readline(input());*/
      askpassw = -1;
      }
    else if (0 == strcmp(argv[i], "-noattr") || 0 == ncsstrcmp(argv[i], "NOATTR"))
      {
      useattr = 0;
      }
    else if (0 == strcmp(argv[i], "-checkupdates") || 0 == ncsstrcmp(argv[i], "CHECKUPDATES"))
      {
      cud = -1;
      }
    else if (0 == strcmp(argv[i], "-debug") || 0 == ncsstrcmp(argv[i], "DEBUG"))
      {
      debug = -1;
      }
    else if (0 == strcmp(argv[i], "-verbose") || 0 == ncsstrcmp(argv[i], "VERBOSE"))
      {
      verbose = -1;
      }
    else if (0 == strcmp(argv[i], "-bell") || 0 == ncsstrcmp(argv[i], "BELL"))
      {
      bell = -1;
      }
    else if (0 == strcmp(argv[i], "-nohtml") || 0 == ncsstrcmp(argv[i], "NOHTML"))
      {
      html = 0;
      }
    else if (0 == strcmp(argv[i], "-server") || 0 == ncsstrcmp(argv[i], "SERVER"))
      {
      i++;
      if (i < argc)
        {
        if (host != NULL)
          {
          free(host);
          }
        host = clonestring(argv[i]);
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
        nicklist_w = atoi(argv[i]);
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
        if (room != NULL)
          {
          free(room);
          }
        room = clonestring(argv[i]);
        }                              
      }
    else if (0 == strcmp(argv[i], "-log") || 0 == ncsstrcmp(argv[i], "LOG"))
      {
      i++;
      if (i < argc)
        {
        ol = -1;
        if (logfn != NULL)
          {
          free(logfn);
          }
        logfn = clonestring(argv[i]);
        /*openlog(argv[i]);*/
        }
      }
    else if (0 == strcmp(argv[i], "-oldlog") || 0 == ncsstrcmp(argv[i], "OLDLOG"))
      {
      i++;
      if (i < argc)
        {
        ol = 0;
        if (logfn != NULL)
          {
          free(logfn);
          }
        logfn = clonestring(argv[i]);
        /*openoldlog(argv[i]);*/
        }
      }
    else if (0 == strcmp(argv[i], "-nick") || 0 == ncsstrcmp(argv[i], "NICK"))
      {
      i++;
      if (i < argc)
        {
        if (nick != NULL)
          {
          free(nick);
          }
        nick = clonestring(argv[i]);
        }                              
      }
    else if (0 == strcmp(argv[i], "-password") || 0 == ncsstrcmp(argv[i], "PASSWORD"))
      {
      i++;
      if (i < argc)
        {
        if (pass != NULL)
          {
          free(pass);
          }
        pass = clonestring(argv[i]);
        }                              
      }
    else
      printf("%s - unknown option\n", argv[i]);
    }

  if (host == NULL)
    {
    host = clonestring("polchat.pl");
    }
  if (nick == NULL)
    {
    nick = clonestring("AmiX_user");
    }
  if (room == NULL)
    {
    room = clonestring("Sekciarz");
    }
  if (logfn != NULL)
    {
    if (ol)
      {
      openlog(logfn);
      }
    else
      {
      openoldlog(logfn);
      }
    }
 
  if (run)
    {
    initscr();
    noecho();
    cbreak();
    start_color();
    
    window_init();
    nodelay(consolewindow, TRUE);
    keypad(consolewindow, TRUE);

    init_pair(7, COLOR_YELLOW, COLOR_BLUE);
    wattron(chatwindow, COLOR_PAIR(7) | A_BOLD);
    window_put(" " $VER " ");
    wattroff(chatwindow, COLOR_PAIR(7) | A_BOLD);
    window_nl();
    window_put(" Linuxowy klient Polchatu");
    window_nl();
    window_put(" By ABUKAJ (J.M.Kowalski - amiga@buziaczek.pl)");
    window_nl();
    window_put(" status wersji 0.2b: freeware (badz giftware ;-D)");
    window_nl();
    window_put(" ze wzgledu na wczesna wersje rozwojowa,");
    window_nl();
    window_put(" autor nie ponosi odpowiedzialnosci za ewentualne");
    window_nl();
    window_put(" szkody wywolane uzyciem programu w tej wersji");
    window_nl();
    window_put(" Oficjalna strona projektu:");
    window_nl();
    window_put(" http://infomax.net.pl/~kowalskijan/amix/");
    window_nl();

    wnoutrefresh(chatwindow);
    doupdate();    

    if (cud)
      {
      checkupdate();
      }
    else
      {
      window_put(" Sprawdzanie uaktualnien pominiete.");
      window_nl();
      }
    wnoutrefresh(chatwindow);
    doupdate();

    if (askpassw)
      {
      if (pass != NULL)
        {
        free(pass);
        }
      pass = input_password();
      }
    if (pass == NULL)
      {
      pass = clonestring("");
      }
    
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);

    window_put(" resolving...");
    window_nl();
    wrefresh(chatwindow);
   
    /*resolvuje adres*/
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if (0 == getaddrinfo(host, NULL, &hints, &res))
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
              window_put(" connecting...");
              window_nl();
              wrefresh(chatwindow);
              if ((connect(sfd, tres->ai_addr, tres->ai_addrlen)) < 0)
                {
                window_put(" Connection failed...");
                window_nl();
                wrefresh(chatwindow);
                close(sfd);
                sfd = -1;
                continue;
                }
              }
            else
              {
              window_put(" Unable to create socket...");
              window_nl();
              wrefresh(chatwindow);
              }
            }
      
          if (sfd >= 0)/*jesli sie polaczylismy*/
            {
            connected = -1;
            window_put(" connected");
            window_nl();
            wrefresh(chatwindow);

            freepart(&tosend);
            if (roomname != NULL)
              {
              ppart = welcome3(nick, pass, roomname, "");
              /*welcome2(nick, pass, roomname, sfd);*/
              }
            else
              {
              ppart = welcome3(nick, pass, room, "");
              /*welcome2(nick, pass, roomname, sfd);*/
              }
            putmsg(ppart);
            }
          else
            {
            window_put("Unable to connect host: ");
            window_put(host);
            window_put(" ...");
            window_nl();
            window_updated = -1;
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
            if (NULL != (prt = readpart(sfd)))
              {
              ppart = parsepart(prt);
              free(prt);
              prt = NULL;
              processpart(ppart, sfd);
              freepart(&ppart);      
              window_print();
              }
            }

          /*czy mamy cos do wyslania?*/
          sendnext(sfd);
          if (window_updated)
            {
            doupdate();
            /*window_updated = 0;*//*okno odswiezone*/
            }
          }
          
        /*czy jest cos na wejsciu?*/
        if (NULL != (inputstring = console_input()))
          {
          if (0 == ncsstrncmp(inputstring, "/quit ", 6) || 0 == ncsstrncmp(inputstring, "/quit", 6))
            {
            ppart = makemsg(inputstring);
            putmsg(ppart);
            }
          else if(0 == ncsstrncmp(inputstring, "/exit ", 6) || 0 == ncsstrncmp(inputstring, "/exit", 6))
            {
            run = 0;
            strncpy(inputstring, "/quit", 5);
            ppart = makemsg(inputstring);
            putmsg(ppart);
            }
          else if (0 == ncsstrncmp(inputstring, "/lama ", 6) || 0 == ncsstrncmp(inputstring, "/lama", 6))
            {
            ppart = makemsg("thankfully alert gauchos were able to save the llama "
                            "before it was swept into the blades of the turbine");
            putmsg(ppart);
            }
          else if (0 == ncsstrncmp(inputstring, "/borg ", 6) || 0 == ncsstrncmp(inputstring, "/borg", 6))
            {
            ppart = makemsg("I'm cybernetic organism - living tissue over metal endoskeleton.");
            putmsg(ppart);
            }
          else if (0 == ncsstrncmp(inputstring, "/debugon ", 9) || 0 == ncsstrncmp(inputstring, "/debugon", 9))
            {
            debug = -1;
            window_put("DEBUG MODE ON");
            window_nl();
            wnoutrefresh(chatwindow);
            }
          else if (0 == ncsstrncmp(inputstring, "/debugoff ", 10) || 0 == ncsstrncmp(inputstring, "/debugoff", 10))
            {
            debug = 0;
            window_put("DEBUG MODE OFF");
            window_nl();
            wnoutrefresh(chatwindow);
            }
          else if (0 == ncsstrncmp(inputstring, "/verboseon ", 11) || 0 == ncsstrncmp(inputstring, "/verboseon", 11))
            {
            verbose = -1;
            window_put("VERBOSE MODE ON");
            window_nl();
            wnoutrefresh(chatwindow);
            }
          else if (0 == ncsstrncmp(inputstring, "/verboseoff ", 12) || 0 == ncsstrncmp(inputstring, "/verboseoff", 12))
            {
            verbose = 0;
            window_put("VERBOSE MODE OFF");
            wnoutrefresh(chatwindow);
            window_nl();
            }
          else if (0 == ncsstrncmp(inputstring, "/ver ", 5) || 0 == ncsstrncmp(inputstring, "/ver", 5))
            {
            window_addstr("<B>" $VER "</B>");
            wnoutrefresh(chatwindow);
            }
          else if (0 == ncsstrncmp(inputstring, "/help ", 6) || 0 == ncsstrncmp(inputstring, "/help", 6))
            {
            window_addstr("<B>/borg</B> - wysyla tekst: &quot;"
                          "I'm cybernetic organism - living tissue over metal endoskeleton.&quot;");
            window_addstr("<B>/exit</B> - komenda analogiczna do polchatowego /quit, ale "
                          "dodatkowo konczy prace programu");
            window_addstr("<B>/help</B> - wyswietla tekst pomocy");
            window_addstr("<B>/lama</B> - wysyla tekst: &quot;" 
                          "thankfully alert gauchos were able to save the llama "
                          "before it was swept into the blades of the turbine&quot;");
            window_addstr("<B>/ver</B> - podaje wersje programu"); 
            wnoutrefresh(chatwindow);
            window_updated = -1;
            }
          else
            {
            ppart = makemsg(inputstring);
            putmsg(ppart);
            }
          }
          
        /*wnoutrefresh(consolewindow);*/
        }
        
      freepart(&tosend);
      if (NULL != roomname)
        {
        free(roomname);
        roomname = NULL;
        }
      if (NULL != roomdesc)
        {
        free(roomdesc);
        roomdesc = NULL;
        }
      freenicklist(&nicks);
      if (connected)
        {
        connected = 0;
        close(sfd);
        }
        
      }
    else
      {
      window_put("Resolver problem...");
      window_nl();
      } 
    
    window_done();
    endwin();
    }
 
  if (nick != NULL)
    {
    free(nick);
    }
  if (pass != NULL)
    {
    free(pass);
    }
  if (host != NULL)
    {
    free(host);
    }
  if (room != NULL)
    {
    free(room);
    }
  if (logfn != NULL)
    {
    free(logfn);
    }
  closelog(); 
  puts("AmiX: Koniec pracy na dzis, polecam sie na przyszlosc");
  return 0;
  }
