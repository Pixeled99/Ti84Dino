# ----------------------------
# Makefile Options
# ----------------------------

NAME = DINO
DESCRIPTION = "Chome Dino Game go Brrrrr"
COMPRESSED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
