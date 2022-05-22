#  -*- Makefile -*-

CC=clang++
CFLAGS= -Wall -O0 -std=c++11 --verbose

SDIR=src
INCL=include
ODIR=obj
FILES = jshell parse job
OBJS = $(patsubst %, $(ODIR)/%.o, $(FILES))
TESTDIR = test
BIN_NAME = josh
BINPATH = /usr/local/bin
BINARY = $(patsubst %, $(BINPATH)/%, $(BIN_NAME))

# makes the final binary for the shell
default: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(BINARY) $(BIN_NAME)


# makes the intermediate object files to build the final binary
$(ODIR)/%.o: $(SDIR)/%.cc
	$(CC) -I $(INCL) $(CFLAGS) -c $^ -o $@


# makes an individual test file from the target object file and the test object file
test_%: $(TESTDIR)/$(ODIR)/test_%.o $(ODIR)/%.o $(ODIR)/job.o
	$(CC) $(CFLAGS) $^ -o $(TESTDIR)/$@

# builds the test object file
$(TESTDIR)/$(ODIR)/test_%.o: $(TESTDIR)/$(SDIR)/test_%.cc
	$(CC) -I $(INCL) $(CFLAGS) -c $^ -o $@


.PHONY: install
install:
	cp $(BIN_NAME) $(BINARY)


.PHONY: uninstall
uninstall:
	rm $(BINARY)


.PHONY: clean
clean:
	rm $(ODIR)/*.o
	rm josh


.PHONY: cleantest
cleantest:
	rm $(TESTDIR)/test_*
	rm $(TESTDIR)/$(ODIR)/*.o