.SILENT:

CC = g++
LD = g++
CFLAGS = -g -Wall -pedantic
LDFLAGS = -lm -lncursesw
RM = rm -f
OBJS = client.o polchat.o temp.o polchat1.o polchat2.o interface.o version.o
PROG = amix

all: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(PROG)

client.o: client.cpp interface.h version.h
	$(CC) $(CFLAGS) -c client.cpp

polchat.o: polchat.cpp interface.h
	$(CC) $(CFLAGS) -c polchat.cpp

polchat1.o: polchat1.cpp
	$(CC) $(CFLAGS) -c polchat1.cpp

polchat2.o: polchat2.cpp interface.h version.h
	$(CC) $(CFLAGS) -c polchat2.cpp

temp.o: temp.cpp
	$(CC) $(CFLAGS) -c temp.cpp

interface.o: interface.cpp interface.h
	$(CC) $(CFLAGS) -c interface.cpp

version.o: version.cpp version.h
	$(CC) $(CFLAGS) -c version.cpp

cleanup:
	$(RM) $(PROG) $(OBJS)

clean:
	$(RM) $(OBJS)
