T_SRCDIR := $(TESTS_DIR)/src
T_BINDIR := $(TESTS_DIR)/bin

T_SRC := $(wildcard $(T_SRCDIR)/*.c)
T_BIN := $(T_SRC:$(T_SRCDIR)/%.c=$(T_BINDIR)/%)
T_DEP := $(T_BIN:%=%.d)

test: $(T_BIN)
	@for xtest in $(T_BIN); do printf "./%s\n" $$xtest; ./$$xtest; echo >&2; done

# https://www.gnu.org/software/make/manual/html_node/Target_002dspecific.html
$(T_BINDIR)/%: OBJ := $(filter-out %main.o,$(OBJ))
$(T_BINDIR)/%: LDLIBS += -lcriterion
$(T_BINDIR)/%: INC_FLAGS += -I$(T_SRCDIR)

$(T_BINDIR)/%: $(T_SRCDIR)/%.c $(OBJ)
	$(CC) $(CFLAGS) $< $(OBJ) -o $@ $(LDLIBS)

tclean:
ifdef T_BINDIR
	@$(RM) -vdr --preserve-root -- $(T_BINDIR)/*
else
	@echo "Tests' bin directory has not been defined."
endif

retest: tclean test

.PHONY: test tclean retest
-include $(T_DEP)
