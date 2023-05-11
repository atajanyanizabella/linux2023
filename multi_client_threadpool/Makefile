SERVER_EXE:=bin/server
CLIENT_EXE:=bin/client
SSRC:=server.cpp threadpool.cpp
CSRC:=client.cpp

.PHONY: all
all: $(SERVER_EXE) $(CLIENT_EXE)

$(SERVER_EXE) : $(SSRC)
	mkdir -p bin
	g++ $^ -o $@ -pthread

$(CLIENT_EXE) : $(CSRC)
	mkdir -p bin
	g++ $^ -o $@ -pthread

clean:
	rm -rf bin
