#!/usr/bin/make -f
# Beware of trailing white spaces.
BINARY := math
CC := gcc
SRC_DIR := src
OBJ_DIR := obj
TESTS_DIR := tests
# All the sub-directories with .h files
INCLUDE_DIRS := $(shell find "$(SRC_DIR)" -mount -name '*.h' -exec dirname {} \+ | sort -u)

# All .c files
SRCS := $(shell find "$(SRC_DIR)" -mount -name '*.c' -type f | sort)
# OBJ_DIR will have the same file tree as in the SRC_DIR
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
# The dependency files have rules that track include files (i.e .h files)
DEP_FILES := $(OBJS:.o=.d)

# https://clang.llvm.org/docs/AddressSanitizer.html
ADDRESS_SANITISER := -fsanitize=address -fno-common
ASAN_OPTIONS := ASAN_OPTIONS=detect_leaks=1
LSAN_OPTIONS := LSAN_OPTIONS=suppressions="lsan.supp":print_suppressions=0
# https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
UNDEFINED_SANITISER := -fsanitize=undefined
# https://www.gnu.org/software/libc/manual/html_node/Source-Fortification.html
HARDENING := -D_FORTIFY_SOURCE=0
# https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html#index-fstack-protector-strong
STACK_CHECKS := -fstack-protector-strong -fstack-clash-protection
# https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html#index-fcf-protection
CONTROL_TRANSFER_CHECKS := -fcf-protection=full

# Include flags
INCL_FLAGS = $(addprefix -I,$(INCLUDE_DIRS))
# Linker flags
LDLIBS :=
LDFLAGS := -Wl,-z,relro
# https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html#index-MMD
CPPFLAGS := -MMD
OPTIMISATION_FLAGS := -Og
DEBUG_FLAGS := -g3 -fno-omit-frame-pointer
WARN_FLAGS := --std=c17 -pedantic -Wall -Wextra -Wformat=2 -Wshadow -Werror
INSTRUMENTATION_FLAGS = $(ADDRESS_SANITISER) $(UNDEFINED_SANITISER) $(STACK_CHECKS) $(CONTROL_TRANSFER_CHECKS) -fsanitize-trap=all
CFLAGS = $(WARN_FLAGS) $(INCL_FLAGS) $(CPPFLAGS) $(OPTIMISATION_FLAGS) $(DEBUG_FLAGS) $(INSTRUMENTATION_FLAGS) $(HARDENING)
CXXFLAGS = $(subst -std=c17,-std=c++17,$(CFLAGS))

all: $(BINARY)

# @^ - names of all the prerequisites
# $@ - the name of the target
$(BINARY): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS) $(LDFLAGS)

# @< - name of only the first prequisite
# @D - the directory of the target
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -vp $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Redefining CFLAGS for release build
# https://www.gnu.org/software/make/manual/html_node/Target_002dspecific.html
release: OPTIMISATION_FLAGS := -O3
release: DEBUG_FLAGS :=
release: ADDRESS_SANITISER :=
release: HARDENING := -D_FORTIFY_SOURCE=2
release: fclean all

clean:
	@$(RM) -vrd --preserve-root -- $(OBJS) $(DEP_FILES)

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
# with other rules. The "s"/"-" suppresses missing file errors, as .d files are
# generated automatically by gcc.
# https://www.gnu.org/software/make/manual/html_node/Automatic-Prerequisites.html
sinclude $(DEP_FILES)
include make_tests.mk
