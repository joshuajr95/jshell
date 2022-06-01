#  -*- Makefile -*-

CC=clang++
CFLAGS=  -g -Wall -O0 -std=c++11 --verbose

SDIR=src
INCL=include
ODIR=obj
FILES = main parse job builtin signal_handlers
OBJS = $(patsubst %, $(ODIR)/%.o, $(FILES))
TESTDIR = test
BIN_NAME = josh
BINPATH = /usr/local/bin
BINARY = $(patsubst %, $(BINPATH)/%, $(BIN_NAME))

# execute scripts to auto-generate the necessary header files
$(shell chmod +x generate_builtins.py)
$(shell chmod +x generate_sighandlers.py)
$(shell ./generate_builtins.py)
$(shell ./generate_sighandlers.py)


# makes the final binary for the shell
all: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(BIN_NAME)


# makes the intermediate object files to build the final binary
$(ODIR)/%.o: $(SDIR)/%.cc
	$(CC) -I $(INCL) $(CFLAGS) -c $^ -o $@


# makes an individual test file from the target object file and the test object file
test_%: $(TESTDIR)/$(ODIR)/test_%.o $(ODIR)/%.o $(ODIR)/job.o
	$(CC) -g $(CFLAGS) $^ -o $(TESTDIR)/$@

# builds the test object file
$(TESTDIR)/$(ODIR)/test_%.o: $(TESTDIR)/$(SDIR)/test_%.cc
	$(CC) -I $(INCL) $(CFLAGS) -c $^ -o $@


.PHONY: install uninstall clean cleantest


install:
	cp $(BIN_NAME) $(BINARY)


uninstall:
	rm $(BINARY)


clean:
	rm $(ODIR)/*.o
	rm josh

cleantest:
	rm $(TESTDIR)/test_*
	rm $(TESTDIR)/$(ODIR)/*.o