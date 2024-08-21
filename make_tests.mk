SHELL := bash
TIMEOUT := timeout
TIMEOUT_OPTS := --preserve-status
T_BINDIR := $(TESTS_DIR)/bin
T_SRCDIR := $(TESTS_DIR)/src

T_SRCS := $(shell find "$(T_SRCDIR)" -mount -name 'test_*.c' -type f | sort)
T_INCLUDES := $(shell find "$(T_SRCDIR)" -mount -name '*.h' -exec dirname {} \+ | sort -u)
T_OBJS := $(T_SRCS:$(T_SRCDIR)/%.c=$(T_BINDIR)/%.o)
MATH_OBJS := $(filter $(OBJ_DIR)/bignum_math/%.o %xalloc.o,$(OBJS))
TEXT_OBJS := $(filter $(OBJ_DIR)/parsing/%.o %xalloc.o,$(OBJS))
UTILITY_OBJS := $(filter %alloc.o %_funcs.o,$(OBJS))
T_BINS := $(T_OBJS:%.o=%)
T_DEPS := $(T_OBJS:%.o=%.d)

MATH_TESTS := $(filter $(T_SRCDIR)/test_bignum_math/%,$(T_SRCS))
MATH_TBINS := $(MATH_TESTS:$(T_SRCDIR)/%.c=$(T_BINDIR)/%)
PARSING_TESTS := $(filter $(T_SRCDIR)/test_parsing/%,$(T_SRCS))
TEXT_TBINS := $(PARSING_TESTS:$(T_SRCDIR)/%.c=$(T_BINDIR)/%)


tests: math-tests parsing-tests

run_tests = $(shell export $(ASAN_OPTIONS) && export $(LSAN_OPTIONS); \
for tbin in $(1); \
do if [[ -f $$tbin && -x $$tbin ]]; \
	then printf "\033[1;94m%s\033[0m\n" "./$$tbin" >&2 \
		&& $(TIMEOUT) $(TIMEOUT_OPTS) ./$$tbin; \
	fi; \
done)

math-tests: INCLUDE_DIRS += $(T_INCLUDES)
math-tests: LDLIBS += -lcriterion
math-tests: TIMEOUT_OPTS += --kill-after=10.0 7.0
math-tests: $(MATH_TBINS)
	$(call run_tests,$(MATH_TBINS))

parsing-tests: INCLUDE_DIRS += $(T_INCLUDES)
parsing-tests: LDLIBS += -lcriterion
parsing-tests: TIMEOUT_OPTS += --kill-after=10.0 7.0
parsing-tests: $(TEXT_TBINS)
	$(call run_tests,$(TEXT_TBINS))

$(T_BINDIR)/%.o: $(T_SRCDIR)/%.c
	@mkdir -vp $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(T_BINDIR)/test_bignum_math/test_cmp_bignum: $(T_BINDIR)/test_bignum_math/test_cmp_bignum.o $(filter %alloc.o %_funcs.o,$(MATH_OBJS))
	@mkdir -vp $(@D)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@ $(LDLIBS) $(LDFLAGS)

$(T_BINDIR)/test_bignum_math/test_bn_mul: $(filter %bn_add.o,$(MATH_OBJS))
$(T_BINDIR)/test_bignum_math/test_bn_div: $(filter-out %bn_div.o %bn_pow.o,$(MATH_OBJS))
$(T_BINDIR)/test_bignum_math/test_bn_pow: $(filter-out %bn_pow.o,$(MATH_OBJS))
$(T_BINDIR)/test_bignum_math/test_%: $(T_BINDIR)/test_bignum_math/test_%.o $(OBJ_DIR)/bignum_math/%.o $(filter %alloc.o %_funcs.o,$(MATH_OBJS))
	@mkdir -vp $(@D)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@ $(LDLIBS) $(LDFLAGS)

$(T_BINDIR)/test_bignum_math/test_bn_mod: $(T_BINDIR)/test_bignum_math/test_bn_mod.o $(filter-out %bn_pow.o,$(MATH_OBJS))
	@mkdir -vp $(@D)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@ $(LDLIBS) $(LDFLAGS)


$(T_BINDIR)/test_parsing/test_base_conversion: $(filter %base_conversion.o,$(TEXT_OBJS)) $(MATH_OBJS)
# https://www.gnu.org/software/make/manual/html_node/Static-Pattern.html
$(TEXT_TBINS):$(T_BINDIR)/%: $(T_SRCDIR)/%.c $(filter-out %base_conversion.o %shunting_yard.o %deque.o %stack.o,$(TEXT_OBJS)) $(filter %bignum_alloc.o %_funcs.o,$(MATH_OBJS))
	@mkdir -vp $(@D)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@ $(LDLIBS) $(LDFLAGS)

tclean: clean
	@$(RM) -vdr --preserve-root -- $(T_BINS) $(T_DEPS) $(T_BINDIR)

retests: tclean tests

.PHONY: tests retest tclean math-tests parsing-tests

sinclude $(T_DEPS)
