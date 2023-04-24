/*AmiX - polchat1.h - (c) by ABUKAJ (J.M.Kowalski)*/
#ifndef POLCHAT1_H
#define POLCHAT1_H

#include <time.h>

#include <string>

#include "interface.h"

//class wrappedString
//{
//  protected:
//    char * content;
//
//
//  public:
//    //wrappedString(std::string);
//
//    wrappedString(const char *);
//
//    char * c_str();
//
//    ~wrappedString()
//    {
//      delete content;
//    }
//
//    int size()
//    {
//      return wraplength(content);
//    }
//
//    std::string string()
//    {
//      return std::string(content + 2);
//    }
//
//    static int wraplength(const char * str)
//    {
//      return ((unsigned char) str[0]) * 256 + ((unsigned char) str[1]) + 3;
//    }
//};

class tank
{
  protected:
    long length;
    char *data;

  public:
    tank(int, char *);
    ~tank();
    int send(int);
    void dump();

    long len()
    {
      return length;
    }

    const char * get()
    {
      return data;
    }

};

//char * wrapstring(const char *);
//unsigned int wrapsize(const char *);
//char *unwrapstring(const char*);
tank *readtank(int);

#endif
