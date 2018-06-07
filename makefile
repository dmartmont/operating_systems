CC 			= g++
CFLAGS 		= -g -Wno-narrowing# -Wall
LIB 		=

SRCDIR		= src
INTDIR 		= interpreter
CTLDIR		= controller
BUILDDIR	= build
BINDIR		= $(BUILDDIR)/bin

SRCEXT 		= cpp
SRCINT 		= $(shell find $(SRCDIR)/$(INTDIR) -maxdepth 1 -type f -name '*.$(SRCEXT)')
SRCCTL 		= $(shell find $(SRCDIR)/$(CTLDIR) -maxdepth 1 -type f -name '*.$(SRCEXT)')

OBJINT		= $(SRCINT:$(SRCDIR)/%.$(SRCEXT)=$(BUILDDIR)/%.o)
OBJCTL		= $(SRCCTL:$(SRCDIR)/%.$(SRCEXT)=$(BUILDDIR)/%.o)

all: interewe controlewe

interewe: directories $(BINDIR)/interewe

controlewe: directories $(BINDIR)/controlewe

$(BINDIR)/interewe: $(OBJINT)
	$(CXX) $(CFLAGS) $^ -o $@ -lrt

$(BINDIR)/controlewe: $(OBJCTL)
	$(CXX) $(CFLAGS) $^ -o $@ -lrt

$(BUILDDIR)/$(INTDIR)/%.o: $(SRCDIR)/$(INTDIR)/%.$(SRCEXT)
	$(CXX) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/$(CTLDIR)/%.o: $(SRCDIR)/$(CTLDIR)/%.$(SRCEXT)
	$(CXX) $(CFLAGS) -c $< -o $@

mew: mew.o
	$(CXX) $^ -o $@

mew.o: src/files/fileMew.cpp
	$(CXX) -c $^ $(CFLAGS) -o $@

bew: bew.o
	$(CXX) $^ -o $@

bew.o: src/files/fileBew.cpp
	$(CXX) -c $^ $(CFLAGS) -o $@

directories:
	@mkdir -p $(BUILDDIR)/$(INTDIR)
	@mkdir -p $(BUILDDIR)/$(CTLDIR)
	@mkdir -p $(BINDIR) 

clean:
	$(RM) -r $(BUILDDIR)

.PHONY: clean
