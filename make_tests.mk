T_BINDIR := $(TESTS_DIR)/bin
T_SRCDIR := $(TESTS_DIR)/src

UTILITIES := $(SRC_DIR)/mem_funcs.c $(SRC_DIR)/string_funcs.c $(SRC_DIR)/array_funcs.c
INCLUDE_DIRS += $(TESTS_DIR)/include

T_SRCS = $(shell find $(T_SRCDIR) -mount -name 'test_*.c' -type f | sort)
T_BINS = $(T_SRCS:$(T_SRCDIR)/%.c=$(T_BINDIR)/%)
T_DEPS = $(T_BINS:%=%.d)

tests: OPTIMISATION_FLAGS :=
tests: ADDRESS_SANITISER :=
tests: CFLAGS += $(LDFLAGS) $(LDLIBS)
tests: $(T_BINDIR) $(T_SRCS) $(T_BINS)
	$(shell for test_bin in $(T_BINS); do if [[ -f $$test_bin && -x $$test_bin ]]; then ./$$test_bin; fi; done)

$(T_BINDIR):
	@mkdir -p $@

$(T_BINDIR)/test_infiX_mul: $(SRC_DIR)/infiX_add.c

$(T_BINDIR)/test_%: $(T_SRCDIR)/test_%.c $(SRC_DIR)/%.c $(UTILITIES)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@

$(T_BINDIR)/test_infiX_div: $(T_SRCDIR)/test_infiX_div.c $(SRC_DIR)/infiX_add.c $(SRC_DIR)/infiX_sub.c $(SRC_DIR)/infiX_mul.c $(UTILITIES)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@

tclean:
	@$(RM) -vdr --preserve-root -- $(T_BINDIR)

retests: tclean tests

.PHONY: tests retest tclean

-include $(T_DEPS)
