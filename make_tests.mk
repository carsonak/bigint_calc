T_BINDIR := $(TESTS_DIR)/bin
T_SRCDIR := $(TESTS_DIR)/src

T_SRCS = $(shell find $(T_SRCDIR) -mount -name 'test_*.c' -type f | sort)
INCLUDE_DIRS += $(TESTS_DIR)/include

T_BINS := $(T_SRCS:$(T_SRCDIR)/%.c=$(T_BINDIR)/%)
T_DEPS := $(T_BINS:%=%.d)

test: ADDRESS_SANITISER :=
test: CFLAGS += $(LDFLAGS) $(LDLIBS)
test: $(T_BINDIR) $(T_BINS)
	@for test_bin in $(T_BINS); do ./$$test_bin; done

$(T_BINDIR)/test_infiX_%: $(T_SRCDIR)/test_infiX_%.c $(filter $(SRC_DIR)/infiX_%.c,$(SRC)) $(SRC_DIR)/memfuncs.c # $(SRC_DIR)/plugs.c
	$(CC) $(CFLAGS) $^ -o $@

$(T_BINDIR):
	@mkdir -p $@

tclean:
	@$(RM) -vdr --preserve-root -- $(T_BINDIR)

retest: tclean test

.PHONY: test retest tclean

-include $(T_DEPS)
