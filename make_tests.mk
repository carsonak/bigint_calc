SHELL := bash
TIMEOUT := timeout
TIMEOUT_OPTS := --preserve-status
T_BINDIR := $(TESTS_DIR)/bin
T_SRCDIR := $(TESTS_DIR)/src

T_SRCS := $(shell find "$(T_SRCDIR)" -mount -name 'test_*.c' -type f | sort)
T_INCLUDES := $(shell find "$(T_SRCDIR)" -mount -name '*.h' -exec dirname {} \+ | sort -u)
# Object files.
T_OBJS := $(T_SRCS:$(T_SRCDIR)/%.c=$(T_BINDIR)/%.o)
ALLOC_OBJS := $(filter %xalloc.o,$(OBJS))
MATH_OBJS := $(filter $(OBJ_DIR)/bignum_math/%.o,$(OBJS))
BN_UTILS_OBJS := $(filter $(OBJ_DIR)/bignum_utils/%.o,$(OBJS))
PARSING_OBJS := $(filter $(OBJ_DIR)/parsing/%.o,$(OBJS))
DS_OBJS := $(filter $(OBJ_DIR)/data_structures/%.o,$(OBJS))
# Binaries
T_BINS := $(T_OBJS:%.o=%)
T_DEPS := $(T_OBJS:%.o=%.d)

MATH_TESTS := $(filter $(T_SRCDIR)/test_bignum_math/%,$(T_SRCS))
MATH_TBINS := $(MATH_TESTS:$(T_SRCDIR)/%.c=$(T_BINDIR)/%)
PARSING_TESTS := $(filter $(T_SRCDIR)/test_parsing/%,$(T_SRCS))
PARSING_TBINS := $(PARSING_TESTS:$(T_SRCDIR)/%.c=$(T_BINDIR)/%)
BN_UTILS_TESTS := $(filter $(T_SRCDIR)/test_bignum_utils/%,$(T_SRCS))
BN_UTILS_TBINS := $(BN_UTILS_TESTS:$(T_SRCDIR)/%.c=$(T_BINDIR)/%)

tests: utils-tests math-tests parsing-tests

run_tests = $(shell export $(ASAN_OPTIONS) && export $(LSAN_OPTIONS); \
for tbin in $(1); \
do if [[ -f $$tbin && -x $$tbin ]]; \
	then printf "\033[1;94m%s\033[0m\n" "./$$tbin" >&2 \
		&& $(TIMEOUT) $(TIMEOUT_OPTS) ./$$tbin; \
	fi; \
done)

utils-tests: INCLUDE_DIRS += $(T_INCLUDES)
utils-tests: LDLIBS += -lcriterion
utils-tests: TIMEOUT_OPTS += --kill-after=10.0 7.0
utils-tests: $(BN_UTILS_TBINS)
	$(call run_tests,$(BN_UTILS_TBINS))

math-tests: INCLUDE_DIRS += $(T_INCLUDES)
math-tests: LDLIBS += -lcriterion
math-tests: TIMEOUT_OPTS += --kill-after=10.0 7.0
math-tests: $(MATH_TBINS)
	$(call run_tests,$(MATH_TBINS))

parsing-tests: INCLUDE_DIRS += $(T_INCLUDES)
parsing-tests: LDLIBS += -lcriterion
parsing-tests: TIMEOUT_OPTS += --kill-after=10.0 7.0
parsing-tests: $(PARSING_TBINS)
	$(call run_tests,$(PARSING_TBINS))

$(T_BINDIR)/%.o: $(T_SRCDIR)/%.c
	@mkdir -vp $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# utils tests
$(T_BINDIR)/test_bignum_utils/test_bn_compare: $(filter %trim_bignum.o,$(BN_UTILS_OBJS))
$(T_BINDIR)/test_bignum_utils/test_int_to_bignum: $(filter %int_to_bignum.o %bn_alloc.o,$(BN_UTILS_OBJS)) $(ALLOC_OBJS)
$(T_BINDIR)/test_bignum_utils/test_%: $(T_BINDIR)/test_bignum_utils/test_%.o  $(OBJ_DIR)/bignum_utils/%.o
	@mkdir -vp $(@D)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@ $(LDLIBS) $(LDFLAGS)

# bignum math tests
$(T_BINDIR)/test_bignum_math/test_bn_mul: $(filter %bn_iadd.o,$(MATH_OBJS))
$(T_BINDIR)/test_bignum_math/test_bn_div: $(filter-out %bn_div.o %bn_pow.o,$(MATH_OBJS))
$(T_BINDIR)/test_bignum_math/test_bn_pow: $(filter-out %bn_pow.o,$(MATH_OBJS))
$(T_BINDIR)/test_bignum_math/test_%: $(T_BINDIR)/test_bignum_math/test_%.o $(OBJ_DIR)/bignum_math/%.o $(BN_UTILS_OBJS) $(ALLOC_OBJS)
	@mkdir -vp $(@D)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@ $(LDLIBS) $(LDFLAGS)

$(T_BINDIR)/test_bignum_math/test_bn_mod: $(T_BINDIR)/test_bignum_math/test_bn_mod.o $(filter-out %bn_pow.o,$(MATH_OBJS)) $(BN_UTILS_OBJS) $(ALLOC_OBJS)
	@mkdir -vp $(@D)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@ $(LDLIBS) $(LDFLAGS)


# parsing tests
$(T_BINDIR)/test_parsing/test_base_conversion: $(filter %base_conversion.o,$(PARSING_OBJS)) $(MATH_OBJS)
$(T_BINDIR)/test_parsing/test_lexer: $(filter %lexer.o,$(PARSING_OBJS)) $(DS_OBJS)
$(T_BINDIR)/test_parsing/test_shunting_yard: $(filter %shunting_yard.o,$(PARSING_OBJS)) $(DS_OBJS)
$(T_BINDIR)/test_parsing/test_uint_array_to_str: $(filter %printing.o,$(PARSING_OBJS))
$(T_BINDIR)/test_parsing/test_str_to_numstr: $(filter %str_bignum_conversion.o,$(PARSING_OBJS))
$(T_BINDIR)/test_parsing/test_numstr_to_bignum: $(filter %str_bignum_conversion.o,$(PARSING_OBJS))
# https://www.gnu.org/software/make/manual/html_node/Static-Pattern.html
$(PARSING_TBINS):$(T_BINDIR)/%: $(T_SRCDIR)/%.c $(filter %count_digits.o %numstr_alloc.o %funcs.o,$(PARSING_OBJS)) $(BN_UTILS_OBJS) $(ALLOC_OBJS)
	@mkdir -vp $(@D)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@ $(LDLIBS) $(LDFLAGS)

tclean: clean
	@$(RM) -vdr --preserve-root -- $(T_OBJS) $(T_BINS) $(T_DEPS) $(T_BINDIR)

retests: tclean tests

.PHONY: tests retest tclean utils-tests math-tests parsing-tests

sinclude $(T_DEPS)
