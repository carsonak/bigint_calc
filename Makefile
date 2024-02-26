BINARY := infiX_math
CC := gcc

# Main directory for all source files
SRCS_DIR := src
# Finding all the sub-directories with .c files
SRCDIRS := $(shell find $(SRCS_DIR) -mount -name '*.c' -exec dirname {} \; | sort -u)
SRC := $(shell find '$(SRCDIRS)' -mount -name '*.c' -type f)

# Main directory for all the object files
OBJS_DIR := obj
# Renaming subdirectories of source dorectory into obj subdirectories
OBJDIRS := $(subst $(SRCS_DIR),$(OBJS_DIR),$(SRCDIRS))
OBJ := $(SRC:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)
# Make dependecy files track headers and directories
DEP_FILES := $(OBJ:.o=.d)

#Linker flags
LDLIBS := -lm -lrt
LDFLAGS =
# Include flags so all header files are discovered
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

# $@ - allows us to make only the needed directory
$(OBJDIRS):
	@mkdir -p $@

oclean:
	$(RM) -rd $(OBJDIRS)

re: oclean $(OBJ)

.PHONY: all oclean re
