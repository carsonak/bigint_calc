RUBY := $(shell which ruby)
# Unity generator script
GENARATOR := Unity/auto/generate_test_runner.rb
# Unity result parsing script
RESULT_PARSER := Unity/auto/parse_output.rb
# Error file parser script
ERROR_PARSER := $(TESTS_DIR)/aggregate_errors.awk
YAML_CONFIG := $(TESTS_DIR)/tests_config.yaml

T_BINARY := $(TESTS_DIR)/bin
T_SOURCES := $(TESTS_DIR)/src
T_RESULTS := $(TESTS_DIR)/results
T_RUNNERS := $(T_SOURCES)/runners
UNITY_SRCDIR := Unity/src $(shell find Unity/extras -name 'src' -type d | grep -E -ve '.*fixture.*')

UNITY_SCRIPTS := $(shell find $(UNITY_SRCDIR) -mount -name '*.c' -type f)
ASSERT_SCRIPTS := $(wildcard $(T_SOURCES)/*.c)
RUNNER_SCRIPTS := $(addsuffix _Runner.c,$(basename $(ASSERT_SCRIPTS:$(T_SOURCES)/%=$(T_RUNNERS)/%)))

T_BINS := $(ASSERT_SCRIPTS:$(T_SOURCES)/%.c=$(T_BINARY)/%)
T_DEPS := $(T_BINS:%=%.d)
RES_OUT := $(T_RESULTS)/output.txt
RES_ERR := $(T_RESULTS)/errors.txt

test: $(T_BINARY) $(T_RESULTS) $(T_RUNNERS) $(T_BINS)
	@for xtest in $(wordlist 4, $(words $^), $^); do ./$$xtest; done 1> $(RES_OUT) 2> $(RES_ERR) &
	@$(RUBY) $(RESULT_PARSER) $(RES_OUT)
	@echo
	@./$(ERROR_PARSER) $(RES_ERR)

$(T_RUNNERS)/%_Runner.c: $(T_SOURCES)/%.c
	@./$(GENARATOR) $< $@
	@echo 'Generated $@'

# https://www.gnu.org/software/make/manual/html_node/Target_002dspecific.html
$(T_BINARY)/%: INC_FLAGS += $(addprefix -I,$(T_SOURCES) $(UNITY_SRCDIR))
$(T_BINARY)/%: $(T_SOURCES)/%.c $(T_RUNNERS)/%_Runner.c $(UNITY_SCRIPTS) $(OBJ)
	$(CC) $(CFLAGS) $(filter-out %.h %main.o,$^) -o $@ $(LDLIBS)

$(T_BINARY):
	@mkdir -p $@

$(T_RESULTS):
	@mkdir -p $@

$(T_RUNNERS):
	@mkdir -p $@

tclean:
	@$(RM) -vdr --preserve-root -- $(T_BINARY) $(T_RUNNERS) $(T_RESULTS)

retest: tclean test

.PHONY: test retest tclean
# Prevent automatic deletion of intermediate files
# https://www.gnu.org/software/make/manual/html_node/Special-Targets.html#index-_002ePRECIOUS
.PRECIOUS: $(RUNNER_SCRIPTS)
-include $(T_DEPS)
