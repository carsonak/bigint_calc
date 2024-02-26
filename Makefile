CC := gcc
SRC_DIR := src
SRC := $(shell find '$(SRC_DIR)' -mount \( -name '*.c' -type f \) | sort)
OBJ_DIR := obj
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEP_FILES := $(OBJ:%.o=%.d)
BINARY := infiX_math

LDLIBS := -lm -lrt
LDFLAGS =
INC_FLAGS := $(addprefix -I,$(SRC_DIR))
DEBUG_FLAGS := -g -pedantic -fsanitize=undefined -fsanitize-undefined-trap-on-error -fstack-protector-all
CFLAGS := -std=c17 -Wall -Werror -Wextra $(INC_FLAGS) -MMD -MP $(DEBUG_FLAGS)

-include $(DEP_FILES)

all: $(OBJ)
	$(CC) $^ -o $(BINARY) $(LDLIBS)

$(OBJ_DIR):
	@mkdir -p $@

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $@ -c $^

clean:
	$(RM) $(BINARY)

oclean:
	$(RM) -rd $(OBJ)

re: oclean $(OBJ)

.PHONY: all clean oclean re $(OBJ_DIR)
