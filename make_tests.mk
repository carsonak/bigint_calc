SHELL := bash
TIMEOUT := timeout
TIMEOUT_OPTS := --preserve-status --verbose

# Tests directories.
tests_bin_dir := $(TESTS_DIR)/bin
tests_src_dir := $(TESTS_DIR)/src
bi_utils_t_dir := $(tests_src_dir)/test_bigint_utils
bi_utils_t_bin_dir := $(tests_bin_dir)/test_bigint_utils
math_t_dir := $(tests_src_dir)/test_bigint_math
math_t_bin_dir := $(tests_bin_dir)/test_bigint_math
str_t_dir := $(tests_src_dir)/test_string_utils
str_t_bin_dir := $(tests_bin_dir)/test_string_utils
numstr_t_dir :=  $(tests_src_dir)/test_numstr
numstr_t_bin_dir :=  $(tests_bin_dir)/test_numstr
parsing_t_dir := $(tests_src_dir)/test_parsing
parsing_t_bin_dir := $(tests_bin_dir)/test_parsing

# Tests header files.
t_includes := $(shell find "$(tests_src_dir)" -mount -name '*.h' -exec dirname {} \+ | sort -u)

# Tests C files.
t_srcs := $(shell find "$(tests_src_dir)" -mount -name 'test_*.c' -type f | sort)
bi_utils_tests := $(filter $(bi_utils_t_dir)/%,$(t_srcs))
math_tests := $(filter $(math_t_dir)/%,$(t_srcs))
str_tests := $(filter $(str_t_dir)/%,$(t_srcs))
numstr_tests := $(filter $(numstr_t_dir)/%,$(t_srcs))
parsing_tests := $(filter $(parsing_t_dir)/%,$(t_srcs))

# Object files.
t_objs := $(t_srcs:$(tests_src_dir)/%.c=$(tests_bin_dir)/%.o)
alloc_objs := $(filter %xalloc.o,$(OBJS))
bi_utils_objs := $(filter $(OBJ_DIR)/bigint_utils/%.o,$(OBJS))
math_objs := $(filter $(OBJ_DIR)/bigint_math/%.o,$(OBJS))
ds_objs := $(filter $(OBJ_DIR)/data_structures/%.o,$(OBJS))
str_objs := $(filter $(OBJ_DIR)/string_utils/%.o,$(OBJS))
numstr_objs := $(filter $(OBJ_DIR)/numstr/%.o,$(OBJS))
parsing_objs := $(filter $(OBJ_DIR)/parsing/%.o,$(OBJS))

# Tests binary files.
t_bins := $(t_objs:%.o=%)
bi_utils_t_bins := $(bi_utils_tests:$(bi_utils_t_dir)/%.c=$(bi_utils_t_bin_dir)/%)
math_t_bins := $(math_tests:$(math_t_dir)/%.c=$(math_t_bin_dir)/%)
str_t_bins := $(str_tests:$(str_t_dir)/%.c=$(str_t_bin_dir)/%)
numstr_t_bins := $(numstr_tests:$(numstr_t_dir)/%.c=$(numstr_t_bin_dir)/%)
parsing_t_bins := $(parsing_tests:$(tests_src_dir)/%.c=$(tests_bin_dir)/%)

t_deps := $(t_objs:%.o=%.d)


.PHONY: tests
tests: bn-utils-tests math-tests str-tests numstr-tests parsing-tests

run_tests = $(shell export $(ASAN_OPTIONS) && export $(LSAN_OPTIONS); \
for tbin in $(1); \
do if [[ -f $$tbin && -x $$tbin ]]; \
	then printf "\033[1;94m%s\033[0m\n" "./$$tbin" >&2 \
		&& $(TIMEOUT) $(TIMEOUT_OPTS) ./$$tbin; \
	fi; \
done)

.PHONY: bn-utils-tests
bn-utils-tests: INCLUDE_DIRS += $(t_includes)
bn-utils-tests: LDLIBS += -lcriterion
bn-utils-tests: TIMEOUT_OPTS += --kill-after=7.0 5.0
bn-utils-tests: $(bi_utils_t_bins)
	$(call run_tests,$(bi_utils_t_bins))

.PHONY: math-tests
math-tests: INCLUDE_DIRS += $(t_includes)
math-tests: LDLIBS += -lcriterion
math-tests: TIMEOUT_OPTS += --kill-after=9.0 7.0
math-tests: $(math_t_bins)
	$(call run_tests,$(math_t_bins))

.PHONY: str-tests
str-tests: INCLUDE_DIRS += $(t_includes)
str-tests: LDLIBS += -lcriterion
str-tests: TIMEOUT_OPTS += --kill-after=7.0 5.0
str-tests: $(str_t_bins)
	$(call run_tests,$(str_t_bins))

.PHONY: numstr-tests
numstr-tests: INCLUDE_DIRS += $(t_includes)
numstr-tests: LDLIBS += -lcriterion
numstr-tests: TIMEOUT_OPTS += --kill-after=7.0 5.0
numstr-tests: $(numstr_t_bins)
	$(call run_tests,$(numstr_t_bins))

.PHONY: parsing-tests
parsing-tests: INCLUDE_DIRS += $(t_includes)
parsing-tests: LDLIBS += -lcriterion
parsing-tests: TIMEOUT_OPTS += --kill-after=7.0 5.0
parsing-tests: $(parsing_t_bins)
	$(call run_tests,$(parsing_t_bins))

$(tests_bin_dir)/%.o: $(tests_src_dir)/%.c
	@mkdir -vp $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# bigint utils tests
$(bi_utils_t_bin_dir)/test_bi_alloc: $(alloc_objs)
$(bi_utils_t_bin_dir)/test_bi_compare: $(filter %bi_trim.o,$(bi_utils_objs))
$(bi_utils_t_bin_dir)/test_%: $(bi_utils_t_bin_dir)/test_%.o  $(OBJ_DIR)/bigint_utils/%.o
	@mkdir -vp $(@D)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@ $(LDLIBS) $(LDFLAGS)

$(bi_utils_t_bin_dir)/test_bi_alloc_fail: $(bi_utils_t_bin_dir)/test_bi_alloc_fail.o $(tests_bin_dir)/dummy_xalloc.o $(filter %bi_alloc.o %bi_swap.o,$(bi_utils_objs))
	@mkdir -vp $(@D)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@ $(LDLIBS) $(LDFLAGS)

# bigint math tests
$(math_t_bin_dir)/test_bi_add_int: $(filter %int_to_bigint.o %bi_add.o,$(math_objs))
$(math_t_bin_dir)/test_bi_sub_int: $(filter %int_to_bigint.o %bi_sub.o,$(math_objs))
$(math_t_bin_dir)/test_bi_iadd_int: $(filter %int_to_bigint.o %bi_iadd.o,$(math_objs))
$(math_t_bin_dir)/test_bi_isub_int: $(filter %int_to_bigint.o %bi_isub.o,$(math_objs))
$(math_t_bin_dir)/test_bi_mul: $(filter %bi_iadd.o,$(math_objs))
$(math_t_bin_dir)/test_bi_div: $(filter-out %bi_div.o %bi_pow.o,$(math_objs))
$(math_t_bin_dir)/test_bi_mod: $(filter-out %bi_div.o %bi_pow.o,$(math_objs))
$(math_t_bin_dir)/test_bi_pow: $(filter-out %bi_pow.o,$(math_objs))
$(math_t_bin_dir)/test_%: $(math_t_bin_dir)/test_%.o $(OBJ_DIR)/bigint_math/%.o $(bi_utils_objs) $(alloc_objs)
	@mkdir -vp $(@D)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@ $(LDLIBS) $(LDFLAGS)

$(math_t_bin_dir)/test_bi_alloc_fail: $(tests_bin_dir)/dummy_xalloc.o $(math_objs) $(filter-out %xalloc.o,$(alloc_objs))
$(math_t_bin_dir)/test_bi_mod: $(filter-out %bi_pow.o,$(math_objs)) $(alloc_objs)
$(math_t_bin_dir)/test_%: $(math_t_bin_dir)/test_%.o $(bi_utils_objs)
	@mkdir -vp $(@D)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@ $(LDLIBS) $(LDFLAGS)


# string utils tests
$(str_t_bin_dir)/test_filter_str: $(alloc_objs)
$(str_t_bin_dir)/test_%: $(str_t_bin_dir)/test_%.o $(OBJ_DIR)/string_utils/%.o
	@mkdir -vp $(@D)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@ $(LDLIBS) $(LDFLAGS)

# numstr utils tests
$(numstr_t_bin_dir)/test_str_to_numstr: $(str_objs)
$(numstr_t_bin_dir)/test_anybase_to_bigint: $(filter %char_to_int.o,$(str_objs)) $(math_objs) $(bi_utils_objs)
$(numstr_t_bin_dir)/test_numstr_to_bigint: $(filter %count_digits.o,$(numstr_objs)) $(bi_utils_objs)
$(numstr_t_bin_dir)/test_bigint_to_numstr: $(filter %count_digits.o,$(numstr_objs))
$(numstr_t_bin_dir)/test_%: $(OBJ_DIR)/numstr/%.o $(numstr_t_bin_dir)/test_%.o $(filter %numstr_alloc.o,$(numstr_objs)) $(alloc_objs)
	@mkdir -vp $(@D)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@ $(LDLIBS) $(LDFLAGS)

# parsing tests
$(parsing_t_bin_dir)/test_lexer: $(ds_objs) $(math_objs)
$(parsing_t_bin_dir)/test_shunting_yard: $(ds_objs)
$(parsing_t_bin_dir)/test_%: $(OBJ_DIR)/parsing/%.o $(parsing_t_bin_dir)/test_%.o $(str_objs) $(numstr_objs) $(bi_utils_objs) $(alloc_objs)
	@mkdir -vp $(@D)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@ $(LDLIBS) $(LDFLAGS)

$(parsing_t_bin_dir)/test_uint_array_to_str: $(parsing_t_bin_dir)/test_uint_array_to_str.o $(filter %printing.o,$(parsing_objs)) $(filter %count_digits.o,$(numstr_objs)) $(alloc_objs)
	@mkdir -vp $(@D)
	$(CC) $(CFLAGS) $(filter-out %.h,$^) -o $@ $(LDLIBS) $(LDFLAGS)

.PHONY: tclean
tclean: clean
	@$(RM) -vdr --preserve-root -- $(t_objs) $(t_bins) $(t_deps) $(tests_bin_dir)

.PHONY: retests
retests: tclean tests

.PRECIOUS: $(t_objs)

sinclude $(t_deps)
