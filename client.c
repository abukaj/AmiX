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
#include "polchat1.h"
#include "polchat2.h"
#include "temp.h"

#define input() 0
#define output() 1

#define BUFFSIZE 10240

int main(int argc, char *argv[]){
  int i, j;
  char *host = NULL;
  char *room = NULL;
  char *nick = NULL;
  char *pass = NULL;
  int port = 14003;
  static char buffer[BUFFSIZE];
  char *ptr;
  
  int sfd = -1;
  struct sockaddr_in *serv_addr = NULL;
  struct addrinfo hints, *res, *tres;
  struct pollfd pol;
  unsigned char *prt = NULL;
  part *ppart = NULL;
  
  for (i = 1; i < argc; ++i){
    if (0 == strcmp(argv[i], "?")){
      printf("SERVER/K PORT/K/N ROOM/K NICK/K PASSWORD/K DEBUG/S VERBOSE/S BELL/S\n");
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
    puts("\033[44m\033[93m AmiX v. 0.2a \033[0m");
    puts(" Linuxowy klient Polchatu ");
    puts(" By ABUKAJ (J.M.Kowalski - amiga@buziaczek.pl) ");
    puts(" status wersji 0.2a: freeware (badz giftware ;-D)");
    puts(" ze wzgledu na wczesna wersje rozwojowa,");
    puts(" autor nie ponosi odpowiedzialnosci za ewentualne");
    puts(" szkody wywolane uzyciem programu w tej wersji");
    putchar('\n');

    printf("resolving...\n");
    /*resolvuje adres*/
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if (0 == getaddrinfo(host, NULL, &hints, &res)){
      for (tres = res; tres; tres = tres->ai_next){
        serv_addr = (struct sockaddr_in *) res->ai_addr;
        serv_addr->sin_port = htons(port);
        if ((sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) != -1){
          printf("connecting...\n");
          if ((connect(sfd, res->ai_addr, res->ai_addrlen)) < 0){
            printf("Connection failed...\n");
            close(sfd);
            sfd = -1;
            continue;
            }
          }
        else{
          printf("Unable to create socket...\n");
          }
        break;
        }

      /*jesli sie udalo polaczyc*/
      if (sfd >= 0){
        printf("connected\n");
        welcome2(nick, pass, room, sfd);/*do wymiany*/
        do {
          /*czy jest cos na wejsciu?*/
          pol.fd = input();
          pol.events = POLLIN;
          pol.revents = 0;
          poll(&pol, 1, 50);
          if (((pol.revents) & POLLIN) == POLLIN){
            i = read(input(), buffer, BUFFSIZE);
            buffer[i - 1] = '\0';
            if (verbose){
              printf("Wprowadzono %d znakow: %s\n", i, buffer);
              }

            if (buffer[0] == 0x1B){
              ptr = buffer + 7;
              if (debug){
                puts("Strange string occured:");
                for (j = 0; j < i; j++){
                  printf("%2u: [%02X/%0o] %c\n", j, buffer[j], buffer[j], buffer[j]);
                  }
                }
              }
            else{
              ptr = buffer;
              }
            if (0 == ncsstrncmp(ptr, "/quit ", 6) || 0 == ncsstrncmp(ptr, "/quit", 6))
              {
              /*run = 0;*/
              ppart = makemsg(ptr);
              sendpol(ppart, sfd);
              freepart(&ppart);
              }
            else if (0 == ncsstrncmp(ptr, "/lama ", 6) || 0 == ncsstrncmp(ptr, "/lama", 6))
              {
              ppart = makemsg("thankfully alert gauchos were able to save the llama "
                              "before it was swept into the blades of the turbine");
              sendpol(ppart, sfd);
              freepart(&ppart);
              }
            else if (0 == ncsstrncmp(ptr, "/nick ", 6) || 0 == ncsstrncmp(ptr, "/nick", 6)){
              printnicks(nicks);
              }
            else {
              ppart = makemsg(ptr);
              sendpol(ppart, sfd);
              freepart(&ppart);
              }
            }                                                  

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
              }
            }
          } while (run);

        freenicklist(&nicks);
        close(sfd);
        }
      else{
        printf("Unable to connect host: %s...\n", host);
        }
   
      }                            
    else{
      printf("Resolver problem...\n");
      }
      
   
    }
 
  puts("AmiX: Koniec pracy na dzis, polecam sie na przyszlosc");
  return 0;
  }
