#  -*- Makefile -*-

CC=clang++
CFLAGS= -g -O0 -std=c++14

SDIR=src
INCL=include
ODIR=obj
FILES = jshell parse job
OBJS = $(patsubst %, $(ODIR)/%.o, $(FILES))
TESTDIR = test

# makes the final binary for the shell
josh: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# makes the intermediate object files to build the final binary
$(ODIR)/%.o: $(SDIR)/%.cc
	$(CC) -I $(INCL) $(CFLAGS) -c $^ -o $@


# makes an individual test file from the target object file and the test object file
test_%: $(TESTDIR)/$(ODIR)/test_%.o $(ODIR)/%.o $(ODIR)/job.o
	$(CC) $(CFLAGS) $^ -o $(TESTDIR)/$@

# builds the test object file
$(TESTDIR)/$(ODIR)/test_%.o: $(TESTDIR)/$(SDIR)/test_%.cc
	$(CC) -I $(INCL) $(CFLAGS) -c $^ -o $@


#.PHONY clean

clean:
	rm $(ODIR)/*.o
	rm josh


#.PHONY cleantest

cleantest:
	rm $(TESTDIR)/test_*
	rm $(TESTDIR)/$(ODIR)/*.o