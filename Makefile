CXX = g++
AR = ar

STATE_FILE = .namespace
.DELETE_ON_ERROR:

ifeq ($(OS),Windows_NT)
$(shell if exist $(STATE_FILE) del /Q $(STATE_FILE))
EXE_EXT = .exe
RM = del /Q
SEP = \\
ECHO = @echo $(1)
PRINT_NS = @if not exist "$(STATE_FILE)" (echo [$(1)] & echo $(1) > "$(STATE_FILE)") else (for /f %%a in ('type "$(STATE_FILE)"') do (if not "%%a"=="$(1)" (echo [$(1)] & echo $(1) > "$(STATE_FILE)")))
else
$(shell rm -f $(STATE_FILE))
EXE_EXT =
RM = rm -f
SEP = /
ECHO = @echo "$(1)"
define PRINT_NS
@if [ ! -f $(STATE_FILE) ] || [ "$$(cat $(STATE_FILE))" != "$(1)" ]; then \
	echo "[$(1)]"; \
	echo "$(1)" > $(STATE_FILE); \
	fi
endef
endif

SRC = src$(SEP)core.cpp src$(SEP)type_getters.cpp src$(SEP)parser.cpp src$(SEP)replace.cpp
OBJ = $(SRC:.cpp=.o)
	LIB = libciri.a
	TARGET = example$(SEP)test_ciri
	MAIN = example$(SEP)main.cpp
	MAIN_OBJ = example$(SEP)main.o

CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -ffunction-sections -fdata-sections -Iinclude -Isrc$(SEP)include
LDFLAGS = -Wl,--gc-sections
ARFLAGS = rcs

all: $(LIB)

$(LIB): $(OBJ)
	$(call PRINT_NS,Makefile)
	$(call ECHO, - Packing libciri)
	@$(AR) $(ARFLAGS) $@ $(OBJ)

src$(SEP)%.o: src$(SEP)%.cpp
	$(call PRINT_NS,Makefile)
	$(call ECHO, - Building $*)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

TARGET_EXE = $(TARGET)$(EXE_EXT)

test: $(TARGET_EXE)
	$(call PRINT_NS,test_ciri)
	@./$(TARGET_EXE)

$(TARGET_EXE): $(LIB) $(MAIN_OBJ)
	$(call PRINT_NS,Makefile)
	$(call ECHO, - Linking test_ciri)
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) $(MAIN_OBJ) $(LIB) -o $(TARGET_EXE)

$(MAIN_OBJ): $(MAIN)
	$(call PRINT_NS,Makefile)
	$(call ECHO, - Building main)
	@$(CXX) $(CXXFLAGS) -c $(MAIN) -o $(MAIN_OBJ)

clean:
	$(call PRINT_NS,Makefile)
	$(call ECHO, - Cleaning files)
	@$(RM) $(OBJ) $(LIB) $(MAIN_OBJ) $(TARGET)$(EXE_EXT) $(STATE_FILE)

