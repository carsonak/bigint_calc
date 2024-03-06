BINARY := debug_math
CC := gcc

# Main directory for all source files
SRCS_DIR := src
# Finding all the sub-directories with .c files
SRCDIRS := $(shell find $(SRCS_DIR) -mount -name '*.c' -exec dirname {} \; | sort -u)
# Extracting .c files from all source sub-directories
SRC := $(shell find $(SRCDIRS) -mount -maxdepth 1 -name '*.c' -type f | sort)

# Main directory for all the object files
OBJS_DIR := obj
# Creating object files by renaming .c to .o then replacing src/ with obj/
OBJ := $(SRC:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)
# The dependency files have rules that track included files (i.e .h files)
DEP_FILES := $(OBJ:.o=.d)

# Linker flags
LDLIBS := -lm
LDFLAGS =
# Include flags so all header files are discovered
INC_FLAGS := $(addprefix -I,$(SRCDIRS))
DEBUG_FLAGS := -g -pedantic -fsanitize=undefined -fsanitize-undefined-trap-on-error -fstack-protector-all
WARN_FLAGS := -std=c17 -Wall -Werror -Wextra
CFLAGS := $(WARN_FLAGS) $(INC_FLAGS) -MMD $(DEBUG_FLAGS)
# https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html#index-MMD

# https://www.gnu.org/software/make/manual/html_node/Automatic-Prerequisites.html
-include $(DEP_FILES)

all: $(BINARY)

# @^ - names of all the prerequisites
# $@ - the name of the target
$(BINARY): $(OBJ)
	$(CC) $^ -o $@ $(LDLIBS)

# @< - name of only the first prequisite
# @D - the directory of the target
$(OBJ):$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
# This static rule allows us to specify only the object files we want compiled
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

oclean:
	@$(RM) -vrd $(OBJ) $(DEP_FILES)

re: oclean all

.PHONY: all oclean re
