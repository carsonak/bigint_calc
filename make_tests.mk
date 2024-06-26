SHELL := bash
TIMEOUT := timeout
T_BINDIR := $(TESTS_DIR)/bin
T_SRCDIR := $(TESTS_DIR)/src

TIMEOUT_OPTS := --preserve-status

UTILITY_FUNCS := $(SRC_DIR)/mem_funcs.c $(SRC_DIR)/string_funcs.c $(SRC_DIR)/array_funcs.c
T_SRCS = $(shell find "$(T_SRCDIR)" -mount -name 'test_*.c' -type f | sort)
T_BINS = $(T_SRCS:$(T_SRCDIR)/%.c=$(T_BINDIR)/%)
T_DEPS = $(T_BINS:%=%.d)

LDLIBS += -lcriterion
CFLAGS += $(LDFLAGS) $(LDLIBS)

tests: unit-tests

$(T_BINDIR):
	@mkdir -p $@

$(T_BINDIR)/test_infiX_mul: $(SRC_DIR)/infiX_add.c
$(T_BINDIR)/test_infiX_div: $(SRC_DIR)/infiX_add.c $(SRC_DIR)/infiX_sub.c $(SRC_DIR)/infiX_mul.c

$(T_BINDIR)/test_%: $(T_SRCDIR)/test_%.c $(SRC_DIR)/%.c $(UTILITY_FUNCS)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@

$(T_BINDIR)/test_infiX_mod: $(SRC_DIR)/infiX_div.c $(T_SRCDIR)/test_infiX_mod.c $(SRC_DIR)/infiX_add.c $(SRC_DIR)/infiX_sub.c $(SRC_DIR)/infiX_mul.c $(UTILITY_FUNCS)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@

$(T_BINDIR)/test_cmp_u4barray: $(T_SRCDIR)/test_cmp_u4barray.c $(SRC_DIR)/array_funcs.c
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@

unit-tests: TIMEOUT_OPTS += --kill-after=9.0 6.0
unit-tests: $(T_BINDIR) $(T_SRCS) $(T_BINS)
	$(shell export $(ASAN_OPTIONS) && export $(LSAN_OPTIONS); \
		for t_binary in $(T_BINS); \
		do if [[ -f $$t_binary && -x $$t_binary ]]; \
			then printf "\033[1;94m%s\033[0m\n" "./$$t_binary" >&2 \
				&& $(TIMEOUT) $(TIMEOUT_OPTS) ./$$t_binary; \
			fi; \
		done\
	)

tclean:
	@$(RM) -vdr --preserve-root -- $(T_BINDIR)

retests: tclean tests

.PHONY: tests retest tclean unit-tests

sinclude $(T_DEPS)
