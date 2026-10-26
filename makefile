CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic

TARGET := game_uno++
BUILD_DIR := build
SRC_DIR := src
TEST_DIR := tests

# version and commit embedded in the executable
VERSION := $(shell cat VERSION 2>/dev/null || echo 0.0.0)
GIT_COMMIT := $(shell git rev-parse --short HEAD 2>/dev/null || echo unknown)
BUILD_INFO := $(BUILD_DIR)/build_info.h

CPPFLAGS ?=
CPPFLAGS += -I$(SRC_DIR) -I$(BUILD_DIR)

SOURCES := $(wildcard $(SRC_DIR)/*.cc)
OBJECTS := $(patsubst $(SRC_DIR)/%.cc,$(BUILD_DIR)/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)

TEST_SOURCES := $(TEST_DIR)/uno_test.cc \
                $(SRC_DIR)/card_class.cc \
                $(SRC_DIR)/card_pack_class.cc \
                $(SRC_DIR)/player_class.cc \
                $(SRC_DIR)/game_class.cc

.PHONY: all clean run test FORCE

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc $(BUILD_INFO) | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

# rewrite only when the content changes, so objects rebuild only on a new
# version or commit
$(BUILD_INFO): FORCE | $(BUILD_DIR)
	@printf '#pragma once\n#define UNO_VERSION "%s"\n#define UNO_GIT_COMMIT "%s"\n' \
		'$(VERSION)' '$(GIT_COMMIT)' > $@.tmp
	@cmp -s $@.tmp $@ || mv $@.tmp $@
	@rm -f $@.tmp

FORCE:

run: $(TARGET)
	./$(TARGET)

test: $(BUILD_DIR)/uno_test
	./$(BUILD_DIR)/uno_test

$(BUILD_DIR)/uno_test: $(TEST_SOURCES) $(BUILD_INFO) | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(TEST_SOURCES) -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

-include $(DEPS)
