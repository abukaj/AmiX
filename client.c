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

#define input() 0
#define output() 1


int main(int argc, char *argv[]){
  int i;
  char *host = NULL;
  char *room = NULL;
  char *nick = NULL;
  char *pass = NULL;
  int port = 14003;
  char *inputstring;

  int sfd = -1;
  struct sockaddr_in *serv_addr = NULL;
  struct addrinfo hints, *res, *tres;
  struct pollfd pol;
  unsigned char *prt = NULL;
  part *ppart = NULL;  
  
  for (i = 1; i < argc; ++i){
    if (0 == strcmp(argv[i], "?")){
      printf("SERVER/K PORT/K/N ROOM/K NICK/K PASSWORD/K DEBUG/S VERBOSE/S BELL/S PERIOD/K/N\n");
      run = 0;
      }
    else if (0 == strcmp(argv[i], "-debug") || 0 == ncsstrcmp(argv[i], "DEBUG")){
      debug = -1;
      }
    else if (0 == strcmp(argv[i], "-verbose") || 0 == ncsstrcmp(argv[i], "VERBOSE")){
      verbose = -1;
      }
    else if (0 == strcmp(argv[i], "-bell") || 0 == ncsstrcmp(argv[i], "BELL")){
      bell = -1;
      }
    else if (0 == strcmp(argv[i], "-server") || 0 == ncsstrcmp(argv[i], "SERVER")){
      i++;
      if (i < argc){
        host = argv[i];
        }                              
      }
    else if (0 == strcmp(argv[i], "-port") || 0 == ncsstrcmp(argv[i], "PORT")){
      i++;
      if (i < argc){
        port = atoi(argv[i]);
        }                              
      }
    else if (0 == strcmp(argv[i], "-period") || 0 == ncsstrcmp(argv[i], "PERIOD")){
      i++;
      if (i < argc){
        period = strtod(argv[i], NULL);
        }
      }
    else if (0 == strcmp(argv[i], "-room") || 0 == ncsstrcmp(argv[i], "ROOM")){
      i++;
      if (i < argc){
        room = argv[i];
        }                              
      }
     else if (0 == strcmp(argv[i], "-nick") || 0 == ncsstrcmp(argv[i], "NICK")){
      i++;
      if (i < argc){
        nick = argv[i];
        }                              
      }
     else if (0 == strcmp(argv[i], "-password") || 0 == ncsstrcmp(argv[i], "PASSWORD")){
      i++;
      if (i < argc){
        pass = argv[i];
        }                              
      }
    else
      printf("%s - unknown option", argv[i]);
    }

  if (host == NULL)
    host = "polchat.pl";
  if (nick == NULL)
    nick = "AmiX_user";
  if (room == NULL)
    room = "Sekciarz";
  if (pass == NULL)
    pass = "";
  
  
  if (run){
    initscr();
    noecho();
    cbreak();
    start_color();
    
    window_init();
    nodelay(consolewindow, TRUE);
    keypad(consolewindow, TRUE);

    init_pair(1, COLOR_YELLOW, COLOR_BLUE);
    wattron(chatwindow, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(chatwindow, 1, 0, " AmiX v. 0.2b rev 4\n");
    wattroff(chatwindow, COLOR_PAIR(1) | A_BOLD);
    wprintw(chatwindow, " Linuxowy klient Polchatu\n");
    wprintw(chatwindow, " By ABUKAJ (J.M.Kowalski - amiga@buziaczek.pl)\n");
    wprintw(chatwindow, " status wersji 0.2b: freeware (badz giftware ;-D)\n");
    wprintw(chatwindow, " ze wzgledu na wczesna wersje rozwojowa,\n");
    wprintw(chatwindow, " autor nie ponosi odpowiedzialnosci za ewentualne\n");
    wprintw(chatwindow, " szkody wywolane uzyciem programu w tej wersji\n");

    wnoutrefresh(chatwindow);
    doupdate();

    wprintw(chatwindow, " resolving...\n");
    wrefresh(chatwindow);
    
    /*resolvuje adres*/
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if (0 == getaddrinfo(host, NULL, &hints, &res)){
      for (tres = res; tres; tres = tres->ai_next){
        serv_addr = (struct sockaddr_in *) res->ai_addr;
        serv_addr->sin_port = htons(port);
        if ((sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) != -1){
          mvwprintw(chatwindow, 8, 1, "connecting...\n");
          wrefresh(chatwindow);
          if ((connect(sfd, res->ai_addr, res->ai_addrlen)) < 0){
            mvwprintw(chatwindow, 9, 1, "Connection failed...\n");
            wrefresh(chatwindow);
            close(sfd);
            sfd = -1;
            continue;
            }
          }
        else{
          mvwprintw(chatwindow, 9, 1, "Unable to create socket...\n");
          wrefresh(chatwindow);
          }
        break;
        }

      /*jesli sie udalo polaczyc*/
      if (sfd >= 0){
        mvwprintw(chatwindow, 8, 1, "connected              \n");
        wrefresh(chatwindow);
        welcome2(nick, pass, room, sfd);/*do wymiany*/
        i = 0;
        do {
          /*czy jest cos na wejsciu?*/
          if (NULL != (inputstring = console_input()))
            {
            if (0 == ncsstrncmp(inputstring, "/quit ", 6) || 0 == ncsstrncmp(inputstring, "/quit", 6))
              {
              ppart = makemsg(inputstring);
              putmsg(ppart);
              }
            else if (0 == ncsstrncmp(inputstring, "/lama ", 6) || 0 == ncsstrncmp(inputstring, "/lama", 6))
              {
              ppart = makemsg("thankfully alert gauchos were able to save the llama "
                              "before it was swept into the blades of the turbine");
              putmsg(ppart);
              }
            else if (0 == ncsstrncmp(inputstring, "/debugon ", 9) || 0 == ncsstrncmp(inputstring, "/debugon", 9))
              {
              debug = -1;
              }
            else if (0 == ncsstrncmp(inputstring, "/debugoff ", 10) || 0 == ncsstrncmp(inputstring, "/debugoff", 10))
              {
              debug = 0;
              }
            else if (0 == ncsstrncmp(inputstring, "/verboseon ", 11) || 0 == ncsstrncmp(inputstring, "/verboseon", 11))
              {
              verbose = -1;
              }
            else if (0 == ncsstrncmp(inputstring, "/verboseoff ", 12) || 0 == ncsstrncmp(inputstring, "/verboseoff", 12))
              {
              verbose = 0;
              }
            else
              {
              ppart = makemsg(inputstring);
              putmsg(ppart);
              }
            }

          wnoutrefresh(consolewindow);
 
          /*czy jest cos na gniezdzie?*/
          pol.fd = sfd;
          pol.events = POLLIN;
          pol.revents = 0;
          poll(&pol, 1, 50);
          if (((pol.revents) & POLLIN) == POLLIN){
            if (NULL != (prt = readpart(sfd))){
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
          doupdate();
          } while (run);
        
        freepart(&tosend);
        if (NULL != roomname){
          free(roomname);
          roomname = NULL;
          }
        if (NULL != roomdesc){
          free(roomdesc);
          roomdesc = NULL;
          }
        freenicklist(&nicks);
        close(sfd);
        }
      else{
        mvwprintw(chatwindow, 9, 1, "Unable to connect host: %s ...\n", host);
        }
   
      }                            
    else{
      mvwprintw(chatwindow, 9, 1, "Resolver problem...\n");
      }
      
    window_done();
    endwin();
    }
 
  puts("AmiX: Koniec pracy na dzis, polecam sie na przyszlosc");
  return 0;
  }
