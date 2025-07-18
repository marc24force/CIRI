# === Config ===
CXX := g++
CXXFLAGS := --std=c++11 -Iinclude -Iinih/cpp -Wall -Wextra -MMD -MP
LDFLAGS := 

SRC_DIR := src
BUILD_DIR := build
INIH_DIR := inih
INIH_CPP := $(INIH_DIR)/cpp
INIH_SRC := $(INIH_CPP)/INIReader.cpp $(INIH_DIR)/ini.c

OBJS := $(BUILD_DIR)/Ciri.o \
        $(BUILD_DIR)/INIReader.o \
        $(BUILD_DIR)/ini.o

DEPS := $(OBJS:.o=.d)

TARGET := $(BUILD_DIR)/libciri.a

TEST_SRC := example/main.cpp
TEST_BIN := $(BUILD_DIR)/ciri_test

# === Rules ===
.PHONY: all build test clean

all: build

build: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	ar rcs $@ $^

# Compile rules with separate object files in build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/INIReader.o: $(INIH_CPP)/INIReader.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/ini.o: $(INIH_DIR)/ini.c
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Test rule
test: build
	$(CXX) $(CXXFLAGS) $(TEST_SRC) -o $(TEST_BIN) $(TARGET)
	./$(TEST_BIN)

# Clean rule
clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)

