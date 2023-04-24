/*HTTP client - client.c - v. 0.1 - J.M.Kowalski "ABUKAJ"*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "interfeace.h"
#include "version.h"
#include "polchat.h"

#define TM const struct tm
#define MAX_CL 10240
#define VALID_CACHE 10
#define SA const struct sockaddr
#define WHOIS_PORT 43

int checkupdate(){
  char buffer[1024];
  char *string = NULL;
  int sfd = -1;
  struct addrinfo hints, *res, *tres;
  int length = 0;
  int state = 0;
 
      memset(&hints, 0, sizeof(hints));
      hints.ai_family = PF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      
      if (0 == getaddrinfo("infomax.net.pl", "http", &hints, &res)){
        for (tres = res; tres; tres = tres->ai_next){
          if ((sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) != -1){
            if ((connect(sfd, res->ai_addr, res->ai_addrlen)) < 0){
              window_put("Connection to infomax.net.pl failed...");
              window_nl();
             
              close(sfd);
              sfd = -1;
              continue;
              }
          
            }
          else{
            window_put("Unable to create socket :-(...");
            window_nl();
            }
        
          break;
          }
      
        if (sfd >= 0){
          strcpy(buffer, "GET /~kowalskijan/amix/version.txt  HTTP/1.1\r\nHost: infomax.net.pl\r\n\r\n");
                                                  
          if (write(sfd, buffer, strlen(buffer) + 1))
            {
            while ((state != 6) && read(sfd, buffer, 1))
              {
              switch (state)
                {
                case 0:
                  if (buffer[0] == '\r'){
                    state = 1;
                    }
                  else{
                    /*putchar(buffer[0]);*/
                    }
                  break;
                case 1:
                  if (buffer[0] == '\n'){
                    state = 2;
                    /*putchar('\n');*/
                    }
                  else{
                    state = 0;
                    /*putchar('\r');
                    putchar(buffer[0]);*/
                    }
                  break;
                case 2:
                  if (buffer[0] == '\r')
                    state = 3;
                  break;
                case 3:
                  if (buffer[0] == '\n')
                    state = 4;
                  else
                    state = 2;
                  break;
                case 4:
                  if (buffer[0] == '\r')
                    state = 5;
                  else
                    state = 2;
                  break;
                case 5:
                  if (buffer[0] == '\n'){
                    state = 6;
                    }
                  else
                    state = 2;
                  break;
                }
              }

            if (NULL != (string = readline(sfd))){
              if (strcmp(VERCODE, string) < 0)
                {
                window_addstr("<b><blink>UWAGA!</blink> Jest nowa wersja programu AmiX!</b>");
                free(string);
                }
              while (NULL != (string = readline(sfd)))
                {
                window_addstr(string);
                free(string);
                }
              
              wnoutrefresh(chatwindow);
              doupdate();                                    
              }
            }
          else
            {
            window_put("Unable to send http request.");
            window_nl();
            }
          close(sfd);
          }
        else
          {
          if (debug)
            {
            window_put("Unable to connect infomax.net.pl .");
            window_nl();
            }
          }
        freeaddrinfo(res);
        }
      else
        {
        if (debug)
          {
          window_put("Resolver problem (unable to resolve infomax.net.pl).");
          window_nl();
          }
        }
    
  return length;
  }


char *readline(int sfd)
  {
  int state = 0;
  int inbuf = 0;
  int length = 0;
  static char buffer[1024];
  char c;
  char *string = NULL;
  char *tmp;
  
  while ((state != 2) && read(sfd, buffer + inbuf, 1))
    {
    c = buffer[inbuf];
    switch (c)
      {
      case '\n':
        state = 2;
        break;
      default:
        state = 1;
        if (inbuf >= 1024)
          {
          if (NULL != string)
            {
            if (NULL != (tmp = realloc(string, length + 1024)))
              {
              string = tmp;
              strncpy(string + length, buffer, 1024);
              length += 1024;
              }
            else
              {
              free(string);
              string = NULL;
              }
            }
          else
            {
            if (NULL != (string = calloc(1024, sizeof(char))))
              {
              strncpy(string, buffer, 1024);
              length = 1024;
              }
            }
          inbuf = 0;
          }                
        buffer[inbuf++] = c;
        break;
      }                           
    }
  
  if (state != 0)
    {
    if (NULL != string)
      {
      if (NULL != (tmp = realloc(string, length + inbuf + 1)))
        {
        string = tmp;
        strncpy(string + length, buffer, inbuf);
        length += inbuf;
        string[length] = '\0';
        }
      else
        {
        free(string);
        string = NULL;
        }
      }
    else
      {
      if (NULL != (string = calloc(inbuf + 1, sizeof(char))))
        {
        strncpy(string, buffer, inbuf);
        length = inbuf;
        string[inbuf] = '\0';
        }
      }
    }
  return string;
  }


