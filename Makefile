BINARY := infiX_math
CC := gcc

# Main directory for all source files
SRCS_DIR := src
# Sub-directories with source files
SRCDIRS := $(shell find $(SRCS_DIR) -mount -name '*.c' -exec dirname {} \; | sort -u)
SRC := $(shell find '$(SRCS_DIR)' -mount -name '*.c' -type f)

OBJS_DIR := obj
# Recreating subdirectories in main objects directory
OBJDIRS := $(subst $(SRCS_DIR),$(OBJS_DIR),$(SRCDIRS))
OBJ := $(SRC:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)
DEP_FILES := $(OBJ:.o=.d)

LDLIBS := -lm -lrt
LDFLAGS =
INC_FLAGS := $(addprefix -I,$(SRCDIRS))
DEBUG_FLAGS := -g -pedantic -fsanitize=undefined -fsanitize-undefined-trap-on-error -fstack-protector-all
WARN_FLAGS := -std=c17 -Wall -Werror -Wextra
CFLAGS := $(WARN_FLAGS) $(INC_FLAGS) -MMD -MP $(DEBUG_FLAGS)

-include $(DEP_FILES)

all: $(OBJDIRS) $(OBJ)
	$(CC) $(OBJ) -o $(BINARY) $(LDLIBS)

# @< - Every item in prequisite is processed one by one
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	$(CC) $(CFLAGS) $< -c
	@mv -u $(shell basename $< .c)?? $(dir $@)

$(OBJDIRS):
	@mkdir -p $@

oclean:
	$(RM) -rd $(OBJDIRS)

re: oclean $(OBJ)

.PHONY: all oclean re
