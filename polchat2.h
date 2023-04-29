/*AmiX - polchat2.h - v. 0.2 - (c) by ABUKAJ (J.M.Kowalski)*/
#ifndef AMIX_POLCHAT2_H
#define AMIX_POLCHAT2_H

#include <string>

#include "polchat1.h"

#define POLCHAT_BAD_PASSWORD_MSG "nieprawidłowe hasło lub identyfikator użytkownika"
#define POLCHAT_NOOP_REPLY_MSG "<font color=red>** nieznana komenda <b>/noop</b></font>"
#define POLCHAT_NICK_BUSY_MSG "wybrany identyfikator jest aktualnie w użyciu"

class part
{
  public:
    short headerlen;
    short nstrings;
    short *header;
    std::string *strings;
    part *next;

    part(tank &);
    //part(const char *);
    part(std::string &);
    //part(const char *, std::string &);
    part(std::string &, std::string &);
    part(std::string, std::string, std::string, std::string, std::string);
    ~part();
    void dump();
};

part *readpart(int);
void processpart(part *, int);


int sendpol(part *, int);

void putmsg(part *);
void sendnext(int);

extern part *tosend;
extern time_t last;

#endif
