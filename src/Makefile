CXX = g++
CXXFLAGS = -std=c++20 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

BUILD_DIR = ../build
CORE_DIR = core
GRAPHICS_DIR = graphics

# List all the source files
SRCS = $(wildcard $(CORE_DIR)/*.cpp) $(wildcard $(GRAPHICS_DIR)/*.cpp) main.cpp

# Derive object file names from source files in the build directory
OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(SRCS)))

# Final executable name in the build directory
TARGET = $(BUILD_DIR)/VulkanTest

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Pattern rule to build .o files from .cpp files
$(BUILD_DIR)/%.o: $(CORE_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(GRAPHICS_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

