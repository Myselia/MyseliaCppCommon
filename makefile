# Variables
CC=g++
CFLAGS=-I include -std=c++11 -c
LDFLAGS=
LIBFLAGS=-lboost_system -lboost_thread
EXECUTABLE=MyseliaCppCommon
SRCDIR=src
OBJDIR=obj
SOURCES = $(shell find $(SRCDIR)/ -name "*.cpp")
OBJECTS = $(subst $(SRCDIR)/,$(OBJDIR)/,$(SOURCES:.cpp=.o))

# PHONY (non-file targets)
.PHONY: clean cppcommon all

# Entry targets

all: cppcommon

cppcommon: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $(EXECUTABLE) $(LDFLAGS)  $(OBJECTS) $(LIBFLAGS)

clean:
	rm -f -R $(OBJDIR) $(EXECUTABLE)

# Targets who are dependencies

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp |
	-mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

