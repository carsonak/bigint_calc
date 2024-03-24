RUBY := $(shell which ruby)
# Unity generator script
GENARATOR := Unity/auto/generate_test_runner.rb
# Unity result parsing script
RESULT_PARSER := Unity/auto/parse_output.rb

T_SRCDIR := $(TESTS_DIR)/src
T_BINDIR := $(TESTS_DIR)/bin
T_RESDIR := $(TESTS_DIR)/results
UNITY_SRCDIR := Unity/src $(shell find Unity/extras -name 'src' -type d | grep -E -ve '.*fixture.*')

UNITY_SCRIPTS := $(shell find $(UNITY_SRCDIR) -mount -name '*.c' -type f)
ASSERT_SCRIPTS := $(shell find tests/src -name "*.c" -type f | grep -E -ve '.*Runner.c')
RUN_SCRIPTS := $(addsuffix _Runner.c,$(basename $(ASSERT_SCRIPTS)))
T_BIN := $(ASSERT_SCRIPTS:$(T_SRCDIR)/%.c=$(T_BINDIR)/%)
T_DEP := $(T_BIN:%=%.d)
T_RESULT := $(T_RESDIR)/output.txt
YAML_CONFIG := $(TESTS_DIR)/tests_config.yaml

test: $(T_BINDIR) $(T_RESDIR) $(T_BIN)
	@for xtest in $(wordlist 3, $(words $^), $^); do ./$$xtest; done > $(T_RESULT); $(RUBY) $(RESULT_PARSER) $(T_RESULT)

$(T_SRCDIR)/%_Runner.c: $(T_SRCDIR)/%.c
	@./$(GENARATOR) $< $@
	@echo 'Generated $@'

# https://www.gnu.org/software/make/manual/html_node/Target_002dspecific.html
$(T_BINDIR)/%: INC_FLAGS += $(addprefix -I,$(T_SRCDIR) $(UNITY_SRCDIR))
$(T_BINDIR)/%: $(T_SRCDIR)/%.c $(T_SRCDIR)/%_Runner.c $(UNITY_SCRIPTS) $(OBJ)
	$(CC) $(CFLAGS) $(filter-out %.h %main.o,$^) -o $@ $(LDLIBS)

$(T_BINDIR):
	@mkdir -p $@

$(T_RESDIR):
	@mkdir -p $@

tclean_bin:
ifdef T_BINDIR
	@$(RM) -vdr --preserve-root -- $(T_BINDIR)/*
else
	@echo "Tests' bin directory has not been defined."
endif

tclean_run_scripts:
	@$(RM) -vdr --preserve-root -- $(RUN_SCRIPTS)

tclean: tclean_bin tclean_run_scripts

retest: tclean test

# Prevent automatic deletion of intermediate files
# https://www.gnu.org/software/make/manual/html_node/Special-Targets.html#index-_002ePRECIOUS
.PRECIOUS: $(RUN_SCRIPTS)
.PHONY: test retest tclean tclean_bin tclean_run_scripts
-include $(T_DEP)
