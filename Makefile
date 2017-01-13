TARGET= dex
INC= -I./include

CXX= clang++
CXXFLAGS= -Wall -m64 -std=c++14 -ggdb $(INC)

LIBS= -larchive -lz -lbz2 -lpthread -lmagic -ltsk -lmbedcrypto -lcurl -lboost_system
LINKER= clang++
LFLAGS= -Wall -ggdb $(INC) $(LIBS)

SRCDIR= src
INCDIR= include
OBJDIR= obj
BINDIR= bin

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

rm= rm -f

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) -o $@ $(LFLAGS) $(OBJECTS)
	@echo "Linking complete"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled "$<" sucessfully"

.PHONY: clean
clean:
	$(rm) $(OBJECTS)
	@echo "Cleanup complete"

.PHONY: remove
remove: clean
	$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed"
