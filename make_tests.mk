T_BINDIR := $(TESTS_DIR)/bin
T_SRCDIR := $(TESTS_DIR)/src

UTILITIES := $(SRC_DIR)/mem_funcs.c $(SRC_DIR)/string_funcs.c $(SRC_DIR)/array_funcs.c

T_SRCS = $(shell find $(T_SRCDIR) -mount -name 'test_*.c' -type f | sort)
T_BINS = $(T_SRCS:$(T_SRCDIR)/%.c=$(T_BINDIR)/%)
T_DEPS = $(T_BINS:%=%.d)

ADDRESS_SANITISER :=
LDLIBS += -lcriterion
CFLAGS += $(LDFLAGS) $(LDLIBS)

tests: unit-tests

$(T_BINDIR):
	@mkdir -p $@

$(T_BINDIR)/test_infiX_mul: $(SRC_DIR)/infiX_add.c

$(T_BINDIR)/test_%: $(T_SRCDIR)/test_%.c $(SRC_DIR)/%.c $(UTILITIES)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@

$(T_BINDIR)/test_infiX_div: $(T_SRCDIR)/test_infiX_div.c $(SRC_DIR)/infiX_add.c $(SRC_DIR)/infiX_sub.c $(SRC_DIR)/infiX_mul.c $(UTILITIES)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@

unit-tests: $(T_BINDIR) $(T_SRCS) $(T_BINS)
	$(shell \
		for test_bin in $(T_BINS); \
			do if [[ -f $$test_bin && -x $$test_bin ]]; \
				then printf "\033[1;94m%s\033[0m\n" "./$$test_bin" >&2 && $(TIMEOUT) ./$$test_bin; \
			fi; \
		done\
	)

tclean:
	@$(RM) -vdr --preserve-root -- $(T_BINDIR)

retests: tclean tests

.PHONY: tests retest tclean unit-tests

-include $(T_DEPS)
