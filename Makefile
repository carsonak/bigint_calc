# Beware of spaces after variable assignments.
BINARY := math
CC := gcc
SRC_DIR := src
# All the sub-directories with .c files
CHILD_DIRS := $(shell find $(SRC_DIR) -mount -mindepth 2 -name '*.c' -exec dirname {} \; | sort -u)
# All .c files
SRC := $(shell find $(SRC_DIR) $(CHILD_DIRS) -mount -maxdepth 1 -name '*.c' -type f | sort)

OBJ_DIR := obj
# OBJ_DIR will have the same file system structure as in the SRC_DIR
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
# The dependency files have rules that track included files (i.e .h files)
DEP_FILES := $(OBJ:.o=.d)

# Linker flags
LDLIBS := -lm
LDFLAGS =
# Include flags so all header files are discovered
INC_FLAGS := -I$(SRC_DIR) $(addprefix -I,$(CHILD_DIRS))
DEBUG_FLAGS := -g -pedantic -fsanitize=undefined -fsanitize-undefined-trap-on-error -fstack-protector-all
WARN_FLAGS := -std=c17 -Wall -Werror -Wextra
CFLAGS := $(WARN_FLAGS) $(INC_FLAGS) -MMD $(DEBUG_FLAGS)
# https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html#index-MMD


all: $(BINARY)

# @^ - names of all the prerequisites
# $@ - the name of the target
$(BINARY): $(OBJ)
	$(CC) $^ -o $@ $(LDLIBS) $(LDFLAGS)

# @< - name of only the first prequisite
# @D - the directory of the target
$(OBJ):$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
# Static rule for object files heading to obj directory
# https://www.gnu.org/software/make/manual/html_node/Static-versus-Implicit.html
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

release: CFLAGS := $(WARN_FLAGS) -MMD -O3
# Redefining CFLAGS for release build
# https://www.gnu.org/software/make/manual/html_node/Target_002dspecific.html
release: fclean $(BINARY)

oclean:
	@$(RM) -vrd $(OBJ) $(DEP_FILES)

fclean:
	@$(RM) -vrd $(OBJ_DIR) $(BINARY)

re: oclean all

.PHONY: all oclean fclean release re

# include will "paste" the rules it finds in the icluded files at this
# location, therefore best to place it at end of file so as not to interfere
# with other rules. The "-" suppresses missing file errors, as .d files are
# generated automatically by gcc.
# https://www.gnu.org/software/make/manual/html_node/Automatic-Prerequisites.html
-include $(DEP_FILES)
include make_tests.mk
