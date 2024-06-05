#!/usr/bin/make -f
# Beware of trailing white spaces.
BINARY := math
CC := gcc
SRC_DIR := src
OBJ_DIR := obj
TESTS_DIR := tests
# All the sub-directories with .h files
INCLUDE_DIRS = include $(shell find $(SRC_DIR) -mount -name '*.h' -exec dirname {} \; | sort -u)

# All .c files
SRC = $(shell find $(SRC_DIR) -mount -name '*.c' -type f | sort)
# OBJ_DIR will have the same file tree as in the SRC_DIR
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
# The dependency files have rules that track include files (i.e .h files)
DEP_FILES = $(OBJ:.o=.d)

# https://clang.llvm.org/docs/AddressSanitizer.html
ADDRESS_SANITISER := -fsanitize=address
# https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
UNDEFINED_SANITISER := -fsanitize=undefined
# https://www.gnu.org/software/libc/manual/html_node/Source-Fortification.html
HARDENING_FLAGS := -D_FORTIFY_SOURCE=2
ifeq ($(CC),gcc)
# https://gcc.gnu.org/onlinedocs/gcc-13.3.0/gcc/Instrumentation-Options.html#index-fstack-protector-strong
  STACK_CHECKER := -fstack-protector-strong
endif

# Include flags
INCL_FLAGS = $(addprefix -I,$(INCLUDE_DIRS))
# Linker flags
LDLIBS := -lm
LDFLAGS := -Wl,-z,relro
# https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html#index-MMD
CPPFLAGS := -MMD
DEBUG_FLAGS := -g -Og -fno-omit-frame-pointer
WARN_FLAGS := -std=c17 -Wall -Wextra -Wformat=2 -pedantic -Werror
INSTRUMENTATION_FLAGS = $(ADDRESS_SANITISER) $(UNDEFINED_SANITISER) $(STACK_CHECKER) -fsanitize-trap=all
CFLAGS = $(WARN_FLAGS) $(INCL_FLAGS) $(CPPFLAGS) $(DEBUG_FLAGS) $(INSTRUMENTATION_FLAGS) $(HARDENING_FLAGS)
CXXFLAGS = $(subst -std=c17,-std=c++17,$(CFLAGS))

all: $(BINARY)

# @^ - names of all the prerequisites
# $@ - the name of the target
$(BINARY): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS) $(LDFLAGS)

# @< - name of only the first prequisite
# @D - the directory of the target
# Static rule for object files compiled to obj directory
# https://www.gnu.org/software/make/manual/html_node/Static-versus-Implicit.html
$(OBJ):$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Redefining CFLAGS for release build
# https://www.gnu.org/software/make/manual/html_node/Target_002dspecific.html
release: DEBUG_FLAGS := -O3
release: INSTRUMENTATION_FLAGS :=
release: HARDENING_FLAGS :=
release: fclean all

clean:
	@$(RM) -vrd --preserve-root -- $(OBJ) $(DEP_FILES)

fclean:
ifdef OBJ_DIR
	@$(RM) -vrd --preserve-root -- $(shell find $(OBJ_DIR) -mount -mindepth 1 ! -name ".gitignore") $(BINARY)
else
	@$(RM) -vrd --preserve-root -- $(BINARY)
endif

re: fclean all

.PHONY: all clean fclean release re

# include will "paste" the rules it finds in the included files at this
# location, therefore best to place it at end of file so as not to interfere
# with other rules. The "-" suppresses missing file errors, as .d files are
# generated automatically by gcc.
# https://www.gnu.org/software/make/manual/html_node/Automatic-Prerequisites.html
-include $(DEP_FILES)
include ./tests/make_tests.mk
