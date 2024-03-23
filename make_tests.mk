T_SRCDIR := $(TESTS_DIR)/src
T_BINDIR := $(TESTS_DIR)/bin
UNITY_SRCDIR := Unity/src $(shell find Unity/extras -name 'src' -type d | grep -E -ve '*fixture*')
GENARATOR := Unity/auto/generate_test_runner.rb
YAML_CONFIG := src/tests_config.yaml

UNITY_SCRIPTS := $(shell find $(UNITY_SRCDIR) -mount -name '*.c' -type f)
ASSERT_SCRIPTS := $(shell find tests/src -name "*.c" -type f | grep -E -ve '.*Runner.c')
RUN_SCRIPTS := $(addsuffix _Runner.c,$(basename $(ASSERT_SCRIPTS)))

T_BIN := $(ASSERT_SCRIPTS:$(T_SRCDIR)/%.c=$(T_BINDIR)/%)
T_DEP := $(T_BIN:%=%.d)

test: $(T_BIN)
	@for xtest in $(T_BIN); do printf "./%s\n" $$xtest; ./$$xtest; echo >&2; done

# https://www.gnu.org/software/make/manual/html_node/Target_002dspecific.html
$(T_BINDIR)/%: OBJ := $(filter-out %main.o,$(OBJ))
$(T_BINDIR)/%: INC_FLAGS += $(addprefix -I,$(T_SRCDIR) $(UNITY_SRCDIR))

$(T_BINDIR)/%: $(T_SRCDIR)/%.c $(T_SRCDIR)/%_Runner.c $(UNITY_SCRIPTS) $(OBJ)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(T_SRCDIR)/%_Runner.c: $(T_SRCDIR)/%.c
	./$(GENARATOR) ./$< ./$@

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

.PHONY: test retest tclean tclean_bin tclean_run_scripts
-include $(T_DEP)
