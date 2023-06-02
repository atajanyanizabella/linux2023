# Monster Makefile

# Variables
CC := gcc
CFLAGS := -Wall -Wextra
LDFLAGS :=

# Directories
SUBPROJECT1_DIR := messenger_server
SUBPROJECT2_DIR := messenger_app

# Phony targets
.PHONY: all clean

# Default target
all :
	@$(MAKE) -C $(SUBPROJECT1_DIR)
	@$(MAKE) -C $(SUBPROJECT2_DIR)

server :
	@$(MAKE) -C $(SUBPROJECT1_DIR)

app :
	@$(MAKE) -C $(SUBPROJECT2_DIR)

server-run :
	@$(MAKE) -C $(SUBPROJECT1_DIR) run

app-run :
	@$(MAKE) -C $(SUBPROJECT2_DIR) run

# Clean objects
clean:
	@$(MAKE) -C $(SUBPROJECT1_DIR) clean
	@$(MAKE) -C $(SUBPROJECT2_DIR) clean

# Clean all generated files
distclean:
	@$(MAKE) -C $(SUBPROJECT1_DIR) distclean
	@$(MAKE) -C $(SUBPROJECT2_DIR) distclean
