CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
CPPFLAGS ?= -Isrc

TARGET := game_uno++
BUILD_DIR := build
SRC_DIR := src
TEST_DIR := tests

SOURCES := $(wildcard $(SRC_DIR)/*.cc)
OBJECTS := $(patsubst $(SRC_DIR)/%.cc,$(BUILD_DIR)/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)

TEST_SOURCES := $(TEST_DIR)/uno_test.cc \
                $(SRC_DIR)/card_class.cc \
                $(SRC_DIR)/card_pack_class.cc \
                $(SRC_DIR)/player_class.cc \
                $(SRC_DIR)/game_class.cc

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

run: $(TARGET)
	./$(TARGET)

test: $(BUILD_DIR)/uno_test
	./$(BUILD_DIR)/uno_test

$(BUILD_DIR)/uno_test: $(TEST_SOURCES) | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(TEST_SOURCES) -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

-include $(DEPS)
