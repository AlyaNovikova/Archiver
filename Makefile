target := huffman
test_target := test_huffman

BINDIR := bin
SRCDIR := src
INCLDIR := src
TESTDIR := test

CC := g++
CFLAGS := -O2 -Wall -Wextra -std=c++14 -pedantic -I$(INCLDIR)/
LDFLAGS :=

TARGET_OBJECTS := $(patsubst $(SRCDIR)/%.cpp, $(BINDIR)/$(target)_%.o, $(wildcard $(SRCDIR)/*.cpp))
TEST_OBJECTS := $(patsubst $(TESTDIR)/%.cpp, $(BINDIR)/$(test_target)_%.o, $(wildcard $(TESTDIR)/*.cpp)) $(filter-out $(BINDIR)/$(target)_main.o, $(TARGET_OBJECTS))


all: $(target)

$(BINDIR):
	mkdir -p $(BINDIR)

$(target): $(TARGET_OBJECTS)
	$(CC) $(CFLAGS) $(TARGET_OBJECTS) -o $(target) $(LDFLAGS)

$(test_target): $(TEST_OBJECTS)
	$(CC) $(CFLAGS) $(TEST_OBJECTS) -o $(test_target) $(LDFLAGS)

$(BINDIR)/$(target)_%.o: $(SRCDIR)/%.cpp | $(BINDIR)
	$(CC) $(CFLAGS) -c -MMD -o $@ $<

$(BINDIR)/$(test_target)_%.o: $(TESTDIR)/%.cpp | $(BINDIR)
	$(CC) $(CFLAGS) -c -MMD -o $@ $<

include $(wildcard $(BINDIR)/*.d)

clean:
	rm -rf $(target) $(test_target) $(BINDIR)

.PHONY: all clean
