export MAKEFLAGS := -s
SRC_DIR := src
BIN_DIR := bin
OBJ_DIR := obj
export INCLUDE_PATH := -Iinclude
export EXE_FILE := $(BIN_DIR)/server
export SOURCE_FILES := $(wildcard $(SRC_DIR)/*.cpp)
export OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCE_FILES))
export DEP_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.dep, $(SOURCE_FILES))
export DATABASE := users_data.json

$(EXE_FILE) : $(OBJ_FILES)
	-mkdir -p $(BIN_DIR)
	-g++ $^ -o $@ -pthread -std=c++17
	-echo "MADE"

./$(OBJ_DIR)/%.o : ./$(SRC_DIR)/%.cpp
	-gcc -c $(INCLUDE_PATH) $< -o $@ -std=c++17

./$(OBJ_DIR)/%.dep : ./$(SRC_DIR)/%.cpp
	-mkdir -p $(OBJ_DIR)
	-gcc -M $(INCLUDE_PATH) $< -o $@ -std=c++17

.PHONY : clean
clean :
	-rm -rf $(OBJ_DIR)
	-echo "CLEANED"

.PHONY : distclean
distclean : clean
	-rm -rf $(BIN_DIR) database

.PHONY : run
run : $(EXE_FILE) $(DATABASE)
	-./$(EXE_FILE)

$(DATABASE) :
	-mkdir -p database
	-if [ ! -f database/$(DATABASE) ]; then echo "{\"users\":[]}" > database/$(DATABASE); fi

-include $(DEP_FILES)
