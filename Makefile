TARGET= dex
INC= -I./include

CXX= clang++
CXXFLAGS= -Wall -m64 -std=c++14 $(INC)

LIBS= -larchive -lz -lbz2 -lpthread -lmagic -ltsk
LINKER= clang++
LFLAGS= -Wall $(INC) $(LIBS)

DBG_OPT= -g
REL_OPT= -03

EXEC = bin/dex

SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) -o $@ $(LFLAGS) $(OBJECTS)
	@echo "Linking complete"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS)
	@echo "Cleanup complete"

.PHONY: remove
remove: clean
	rm -f $(BINDIR)/$(TARGET)
	@echo "Executable removed"
