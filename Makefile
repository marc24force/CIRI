CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -ffunction-sections -fdata-sections -Iinclude -Isrc/include
LDFLAGS = -Wl,--gc-sections
AR = ar
ARFLAGS = rcs

SRC = src/core.cpp src/type_getters.cpp src/parser.cpp src/replace.cpp
OBJ = $(SRC:.cpp=.o)
LIB = libciri.a
TARGET = example/test_ciri
MAIN = example/main.cpp

ifeq ($(OS),Windows_NT)
	EXE_EXT = .exe
	RM = del /Q
else
	EXE_EXT =
	RM = rm -f
endif

STATE_FILE = .namespace
.DELETE_ON_ERROR:

define PRINT_NS
	@if [ ! -f $(STATE_FILE) ] || [ "$$(cat $(STATE_FILE))" != "$(1)" ]; then \
		echo "[$(1)]"; \
		echo "$(1)" > $(STATE_FILE); \
	fi
endef

# Ensure cleanup after make finishes
MAKE_TERM_OUT := $(shell trap 'rm -f $(STATE_FILE)' EXIT)

all: $(LIB)

$(LIB): $(OBJ)
	$(call PRINT_NS,Makefile)
	@echo " - Packing libciri"
	@$(AR) $(ARFLAGS) $@ $(OBJ)

src/%.o: src/%.cpp
	$(call PRINT_NS,Makefile)
	@echo " - Building $*"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

TARGET_EXE = $(TARGET)$(EXE_EXT)
MAIN_OBJ = example/main.o

test: $(TARGET_EXE)
	$(call PRINT_NS,test_ciri)
	@./$(TARGET_EXE)

$(TARGET_EXE): $(LIB) $(MAIN_OBJ)
	$(call PRINT_NS,Makefile)
	@echo " - Linking test_ciri"
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) $(MAIN_OBJ) $(LIB) -o $(TARGET_EXE)

$(MAIN_OBJ): $(MAIN)
	$(call PRINT_NS,Makefile)
	@echo " - Building main"
	@$(CXX) $(CXXFLAGS) -c $(MAIN) -o $(MAIN_OBJ)

clean:
	$(call PRINT_NS,Makefile)
	@echo " - Cleaning files"
	@$(RM) $(OBJ) $(LIB) example/main.o $(TARGET)$(EXE_EXT) $(STATE_FILE)

