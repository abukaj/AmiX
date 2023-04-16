.SILENT:

CC = gcc
LD = gcc
CFLAGS = -g -Wall
LDFLAGS = -lm -lncurses
RM = rm -f
OBJS = client.o polchat.o temp.o polchat1.o polchat2.o interfeace.o version.o
PROG = amix

all: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(PROG)

client.o: client.c interfeace.h version.h
	$(CC) $(CFLAGS) -c client.c

polchat.o: polchat.c interfeace.h
	$(CC) $(CFLAGS) -c polchat.c

polchat1.o: polchat1.c
	$(CC) $(CFLAGS) -c polchat1.c

polchat2.o: polchat2.c interfeace.h version.h
	$(CC) $(CFLAGS) -c polchat2.c

temp.o: temp.c
	$(CC) $(CFLAGS) -c temp.c

interfeace.o: interfeace.c interfeace.h
	$(CC) $(CFLAGS) -c interfeace.c

version.o: version.c version.h
	$(CC) $(CFLAGS) -c version.c

cleanup:
	$(RM) $(PROG) $(OBJS)

clean:
	$(RM) $(OBJS)
