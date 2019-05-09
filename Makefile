INCLDIR = include
SRCDIR = src
BUILDDIR = build

EXECNAME=transport

# Assume each header represents an object to build
INCLS = $(wildcard $(INCLDIR)/*.h)
OBJECTS = $(patsubst $(INCLDIR)%, $(BUILDDIR)%, $(filter %.o, $(INCLS:.h=.o )))

SRCS  = $(wildcard $(SRCDIR)/*.cpp)

# All objects holds both the class objects and program object
ALLOBJECTS = $(patsubst $(SRCDIR)%, $(BUILDDIR)%, $(filter %.o, $(SRCS:.cpp=.o )))
# ALLOBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))


CFLAGS= -c -std=c++14 -O2 -W -I$(shell root-config --incdir) -I$(INCLDIR)
COMPILER= g++
LINKOPTION = $(shell root-config --libs)


default: $(EXECNAME)

debug: CFLAGS += -g -DDEBUG
debug: $(EXECNAME)

$(EXECNAME): $(BUILDDIR)/$(EXECNAME).o $(OBJECTS)
	$(COMPILER) -o $(EXECNAME) $(BUILDDIR)/$(EXECNAME).o $(OBJECTS) $(LINKOPTION)

$(BUILDDIR)/%.o : $(SRCDIR)/%.cpp
	$(COMPILER) $(CFLAGS) $< -o $@

clean:
	rm -f $(BUILDDIR)/*.o $(EXECNAME)

