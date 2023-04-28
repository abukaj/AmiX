.SILENT:

CC = g++
LD = g++
CFLAGS = -g -Wall -pedantic
LDFLAGS = -lm -lncursesw
RM = rm -f
OBJS = client.o polchat.o temp.o polchat1.o polchat2.o interface.o version.o image.o
HEADERS = temp.h polchat.h polchat1.h polchat2.h interface.h version.h image.h
PROG = amix

all: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(PROG)

interface.h: polchat.h

polchat1.h: interface.h

polchat2.h: polchat1.h

$(HEADERS):
	touch $@
	echo $@

client.o: client.cpp interface.h version.h polchat.h polchat2.h temp.h
	$(CC) $(CFLAGS) -c client.cpp
	echo $@

image.o: image.cpp image.h
	$(CC) $(CFLAGS) -c image.cpp
	echo $@

polchat.o: polchat.cpp interface.h polchat.h version.h temp.h
	$(CC) $(CFLAGS) -c polchat.cpp
	echo $@

polchat1.o: polchat1.cpp polchat1.h polchat2.h interface.h temp.h
	$(CC) $(CFLAGS) -c polchat1.cpp
	echo $@

polchat2.o: polchat2.cpp interface.h version.h polchat2.h polchat.h temp.h
	$(CC) $(CFLAGS) -c polchat2.cpp
	echo $@

temp.o: temp.cpp temp.h
	$(CC) $(CFLAGS) -c temp.cpp
	echo $@

interface.o: interface.cpp interface.h polchat.h temp.h image.h
	$(CC) $(CFLAGS) -c interface.cpp
	echo $@

version.o: version.cpp version.h polchat.h interface.h
	$(CC) $(CFLAGS) -c version.cpp
	echo $@

cleanup:
	$(RM) $(PROG) $(OBJS)
	echo $@

clean:
	$(RM) $(OBJS)
	echo $@

install: $(PROG)
	cp $(PROG) /usr/bin
	echo $@

uninstall:
	$(RM) /usr/bin/$(PROG)
	echo $@

