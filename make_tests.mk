SHELL := bash
TIMEOUT := timeout
TIMEOUT_OPTS := --preserve-status
T_BINDIR := $(TESTS_DIR)/bin
T_SRCDIR := $(TESTS_DIR)/src
UTILITY_FUNCS := $(SRC_DIR)/mem_funcs.c $(SRC_DIR)/array_funcs.c

T_SRCS := $(shell find "$(T_SRCDIR)" -mount -name 'test_*.c' -type f | sort)
T_BINS := $(T_SRCS:$(T_SRCDIR)/%.c=$(T_BINDIR)/%)
T_DEPS := $(T_BINS:%=%.d)

MATH_TESTS := $(filter $(T_SRCDIR)/test_bignum_math/%,$(T_SRCS))
MATH_TBINS := $(MATH_TESTS:$(T_SRCDIR)/%.c=$(T_BINDIR)/%)
PARSING_TESTS := $(filter $(T_SRCDIR)/test_text_processing/%,$(T_SRCS))
PARSING_TBINS := $(PARSING_TESTS:$(T_SRCDIR)/%.c=$(T_BINDIR)/%)


tests: math-tests parsing-tests

$(T_BINDIR)/test_bignum_math/test_bn_mul: $(filter %bn_add.c,$(MATH_SRC))
$(T_BINDIR)/test_bignum_math/test_bn_div: $(filter-out %bn_div.c,$(MATH_SRC))
$(T_BINDIR)/test_bignum_math/test_bn_mod: $(T_SRCDIR)/test_bignum_math/test_bn_mod.c $(MATH_SRC) $(UTILITY_FUNCS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@

$(T_BINDIR)/test_bignum_math/test_bn_%: $(T_SRCDIR)/test_bignum_math/test_bn_%.c $(SRC_DIR)/bignum_math/bn_%.c $(UTILITY_FUNCS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@

# https://www.gnu.org/software/make/manual/html_node/Static-Pattern.html
$(PARSING_TBINS):$(T_BINDIR)/%: $(T_SRCDIR)/%.c $(UTILITY_FUNCS) $(PARSING_SRC) $(MATH_SRC)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@

run_tests = $(shell export $(ASAN_OPTIONS) && export $(LSAN_OPTIONS); \
for tbin in $(1); \
do if [[ -f $$tbin && -x $$tbin ]]; \
	then printf "\033[1;94m%s\033[0m\n" "./$$tbin" >&2 \
		&& $(TIMEOUT) $(TIMEOUT_OPTS) ./$$tbin; \
	fi; \
done)

math-tests: INCLUDE_DIRS += $(shell find "$(T_SRCDIR)" -mount -name '*.h' -exec dirname {} \+ | sort -u)
math-tests: LDLIBS += -lcriterion
math-tests: CFLAGS += $(LDFLAGS) $(LDLIBS)
math-tests: TIMEOUT_OPTS += --kill-after=10.0 7.0
math-tests: $(MATH_TBINS)
	$(call run_tests,$(MATH_TBINS))

parsing-tests: INCLUDE_DIRS += $(shell find "$(T_SRCDIR)" -mount -name '*.h' -exec dirname {} \+ | sort -u)
parsing-tests: LDLIBS += -lcriterion
parsing-tests: CFLAGS += $(LDFLAGS) $(LDLIBS)
parsing-tests: TIMEOUT_OPTS += --kill-after=10.0 7.0
parsing-tests: $(PARSING_TBINS)
	$(call run_tests,$(PARSING_TBINS))

tclean:
	@$(RM) -vdr --preserve-root -- $(T_BINS) $(T_DEPS) $(T_BINDIR)

retests: tclean tests

.PHONY: tests retest tclean math-tests parsing-tests

sinclude $(T_DEPS)
