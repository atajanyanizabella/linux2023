PING_EXE:=bin/ping
PONG_EXE:=bin/pong
PISRC:=ping.c
POSRC:=pong.c

.PHONY: all
all: $(PING_EXE) $(PONG_EXE)

$(PING_EXE) : $(PISRC)
	mkdir -p bin
	g++ $^ -o $@ -pthread

$(PONG_EXE) : $(POSRC)
	mkdir -p bin
	g++ $^ -o $@ -pthread

clean:
	rm -rf bin
