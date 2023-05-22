SERVER_EXE:=bin/server
SSRC:= $(wildcard src/*.cpp)
HEAD:= $(wildcard src/*.hpp)

.PHONY: all
all: $(SERVER_EXE)

$(SERVER_EXE) : $(SSRC) $(HEAD)
	-mkdir -p bin
	-g++ $(SSRC) -o $@ -pthread

clean:
	-rm -rf bin

run: $(SERVER_EXE)
	-./$(SERVER_EXE)
