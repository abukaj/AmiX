.SILENT:

CC = g++
LD = g++
CFLAGS = -g -Wall -pedantic
LDFLAGS = -lm -lncursesw
RM = rm -f
OBJS = client.o polchat.o temp.o polchat1.o polchat2.o interface.o version.o
HEADERS = temp.h polchat.h polchat1.h polchat2.h interface.h version.h
PROG = amix

all: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(PROG)

interface.h: polchat.h

polchat1.h: interface.h

polchat2.h: polchat1.h

$(HEADERS):
	touch $@

client.o: client.cpp interface.h version.h polchat.h polchat2.h temp.h
	$(CC) $(CFLAGS) -c client.cpp
	echo $@

polchat.o: polchat.cpp interface.h polchat.h version.h temp.h
	$(CC) $(CFLAGS) -c polchat.cpp

polchat1.o: polchat1.cpp polchat1.h polchat2.h interface.h temp.h
	$(CC) $(CFLAGS) -c polchat1.cpp

polchat2.o: polchat2.cpp interface.h version.h polchat2.h polchat.h temp.h
	$(CC) $(CFLAGS) -c polchat2.cpp

temp.o: temp.cpp temp.h
	$(CC) $(CFLAGS) -c temp.cpp

interface.o: interface.cpp interface.h polchat.h temp.h
	$(CC) $(CFLAGS) -c interface.cpp

version.o: version.cpp version.h polchat.h interface.h
	$(CC) $(CFLAGS) -c version.cpp

cleanup:
	$(RM) $(PROG) $(OBJS)

clean:
	$(RM) $(OBJS)

install: $(PROG)
	cp $(PROG) /usr/bin

uninstall:
	$(RM) /usr/bin/$(PROG)
