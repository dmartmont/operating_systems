CC 			= g++
CFLAGS 		= -g # -Wall
LIB 		=

SRCDIR		= src
BUILDDIR	= build
BINDIR		= $(BUILDDIR)/bin
TARGET 		= interprete

SRCEXT 		= cpp
SOURCES		= $(shell find $(SRCDIR) -type f -name '*.$(SRCEXT)')
OBJECTS 	= $(SOURCES:$(SRCDIR)/%.$(SRCEXT)=$(BUILDDIR)/%.o)

all: directories $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	$(CXX) $(CFLAGS) -c $< -o $@

directories:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(BINDIR) 

clean:
	@echo "Cleaning..."
	$(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean
