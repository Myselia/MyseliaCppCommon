# Variables
CC=g++
CFLAGS=-I include -std=c++11 -c
LDFLAGS=
LIBFLAGS=-lboost_system -lboost_thread
EXECUTABLE=MyseliaCppCommon
LIBNAME=MyseliaCppCommon.a
MAINFILE=main.cpp
SRCDIR=src
OBJDIR=obj
SOURCES = $(shell find $(SRCDIR)/ -name "*.cpp")
OBJECTS = $(subst $(SRCDIR)/,$(OBJDIR)/,$(SOURCES:.cpp=.o))

# PHONY (non-file targets)
.PHONY: clean all cppcommon library

all: cppcommon library

cppcommon: $(EXECUTABLE)

clean:
	rm -f -R $(OBJDIR) $(EXECUTABLE) $(LIBNAME)

library: $(LIBNAME)

$(LIBNAME): $(OBJECTS)
	ar rs $(LIBNAME) $(OBJECTS)

$(EXECUTABLE): $(OBJECTS) $(OBJDIR)/main.o
	$(CC) -o $(EXECUTABLE) $(LDFLAGS)  $(OBJECTS) $(OBJDIR)/main.o $(LIBFLAGS)

$(OBJDIR)/main.o: $(MAINFILE)
	$(CC) $(CFLAGS) $(MAINFILE) -o $(OBJDIR)/main.o

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	-mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

