CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++23 -MMD -MP -include src/macros.h

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

SRCS := $(shell find $(SRC_DIR) -name '*.cpp')

REL_OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/release/%.o)
DBG_OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/debug/%.o)

DEPS := $(REL_OBJS:.o=.d) $(DBG_OBJS:.o=.d)

all: release

release: $(BIN_DIR)/release/gub
debug: $(BIN_DIR)/debug/gub

# ==========================================
# Release Build Rules
# ==========================================
$(BIN_DIR)/release/gub: $(REL_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) -o $@ $^

$(BUILD_DIR)/release/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -O3 -c $< -o $@

# ==========================================
# Debug Build Rules
# ==========================================
$(BIN_DIR)/debug/gub: $(DBG_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) -o $@ $^

$(BUILD_DIR)/debug/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -g -O0 -c $< -o $@

# ==========================================
# Utilities
# ==========================================
clean:
	rm -fr $(BUILD_DIR) $(BIN_DIR)

-include $(DEPS)

.PHONY: all release debug clean
