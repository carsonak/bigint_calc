SHELL := bash
TIMEOUT := timeout
TIMEOUT_OPTS := --preserve-status
T_BINDIR := $(TESTS_DIR)/bin
T_SRCDIR := $(TESTS_DIR)/src
UTILITY_FUNCS := $(SRC_DIR)/mem_funcs.c $(SRC_DIR)/array_funcs.c

T_SRCS := $(shell find "$(T_SRCDIR)" -mount -name 'test_*.c' -type f | sort)
T_BINS := $(T_SRCS:$(T_SRCDIR)/%.c=$(T_BINDIR)/%)
MATH_TESTS := $(filter $(T_SRCDIR)/test_infiX_%,$(T_SRCS))
MATH_BINS := $(MATH_TESTS:$(T_SRCDIR)/%.c=$(T_BINDIR)/%)
PARSING_TESTS := $(filter $(T_SRCDIR)/test_parsing_%,$(T_SRCS))
PARSING_BINS := $(PARSING_TESTS:$(T_SRCDIR)/%.c=$(T_BINDIR)/%)
T_DEPS := $(T_BINS:%=%.d)


tests: math-tests parsing-tests

$(T_BINDIR):
	@mkdir -p $@

$(T_BINDIR)/test_infiX_mul: $(SRC_DIR)/infiX_add.c
$(T_BINDIR)/test_infiX_div: $(SRC_DIR)/infiX_add.c $(SRC_DIR)/infiX_sub.c $(SRC_DIR)/infiX_mul.c
$(T_BINDIR)/test_infiX_mod: $(T_SRCDIR)/test_infiX_mod.c $(SRC_DIR)/infiX_*.c $(UTILITY_FUNCS)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@

$(T_BINDIR)/test_infiX_%: $(T_SRCDIR)/test_infiX_%.c $(SRC_DIR)/infiX_%.c $(UTILITY_FUNCS)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@

# https://www.gnu.org/software/make/manual/html_node/Static-Pattern.html
$(PARSING_BINS):$(T_BINDIR)/test_%: $(T_SRCDIR)/test_%.c $(UTILITY_FUNCS) $(SRC_DIR)/parsing.c
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@

run_tests = $(shell export $(ASAN_OPTIONS) && export $(LSAN_OPTIONS); \
for tbin in $(1); \
do if [[ -f $$tbin && -x $$tbin ]]; \
	then printf "\033[1;94m%s\033[0m\n" "./$$tbin" >&2 \
		&& $(TIMEOUT) $(TIMEOUT_OPTS) ./$$tbin; \
	fi; \
done)

math-tests: LDLIBS += -lcriterion
math-tests: CFLAGS += $(LDFLAGS) $(LDLIBS)
math-tests: TIMEOUT_OPTS += --kill-after=10.0 7.0
math-tests: $(T_BINDIR) $(MATH_BINS)
	$(call run_tests,$(MATH_BINS))

parsing-tests: LDLIBS += -lcriterion
parsing-tests: CFLAGS += $(LDFLAGS) $(LDLIBS)
parsing-tests: TIMEOUT_OPTS += --kill-after=10.0 7.0
parsing-tests: $(T_BINDIR) $(PARSING_BINS)
	$(call run_tests,$(PARSING_BINS))

tclean:
	@$(RM) -vdr --preserve-root -- $(T_BINS) $(T_DEPS) $(T_BINDIR)

retests: tclean tests

.PHONY: tests retest tclean math-tests parsing-tests

sinclude $(T_DEPS)
