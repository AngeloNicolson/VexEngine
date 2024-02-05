CXX = g++
CXXFLAGS = -std=c++20 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

BUILD_DIR = ../build
CORE_DIR = core
GRAPHICS_DIR = graphics
INPUT_DIR = input

# List all the source files
CORE_SRCS = $(wildcard $(CORE_DIR)/*.cpp)
GRAPHICS_SRCS = $(wildcard $(GRAPHICS_DIR)/*.cpp)
INPUT_SRCS = $(wildcard $(INPUT_DIR)/*.cpp)
SHADER_SRCS = $(wildcard $(GRAPHICS_DIR)/shaders/*.vert) $(wildcard $(GRAPHICS_DIR)/shaders/*.frag)
OTHER_SRCS = $(filter-out $(CORE_SRCS) $(GRAPHICS_SRCS) $(SHADER_SRCS) $(INPUT_SRCS), $(wildcard *.cpp))

# Derive object file names from source files in the build directory
CORE_OBJS = $(patsubst $(CORE_DIR)/%.cpp,$(BUILD_DIR)/$(CORE_DIR)/%.o,$(CORE_SRCS))
GRAPHICS_OBJS = $(patsubst $(GRAPHICS_DIR)/%.cpp,$(BUILD_DIR)/$(GRAPHICS_DIR)/%.o,$(GRAPHICS_SRCS))
INPUT_OBJS = $(patsubst $(INPUT_DIR)/%.cpp,$(BUILD_DIR)/$(INPUT_DIR)/%.o,$(INPUT_SRCS))
SHADER_OBJS = $(SHADER_SRCS:$(GRAPHICS_DIR)/shaders/%=$(BUILD_DIR)/$(GRAPHICS_DIR)/shaders/%.spv)
OTHER_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(OTHER_SRCS))

# Final executable name in the build directory
TARGET = $(BUILD_DIR)/VulkanTest

# Directory for shaders in the build directory
SHADERS_BUILD_DIR = $(BUILD_DIR)/$(GRAPHICS_DIR)/shaders

.PHONY: all clean shaders

all: $(TARGET) shaders

$(TARGET): $(CORE_OBJS) $(GRAPHICS_OBJS) $(INPUT_OBJS) $(OTHER_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Pattern rule to build .o files from .cpp files in the input directory
$(BUILD_DIR)/$(INPUT_DIR)/%.o: $(INPUT_DIR)/%.cpp | $(BUILD_DIR)/$(INPUT_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Pattern rule to build .o files from .cpp files in the core directory
$(BUILD_DIR)/$(CORE_DIR)/%.o: $(CORE_DIR)/%.cpp | $(BUILD_DIR)/$(CORE_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Pattern rule to build .o files from .cpp files in the graphics directory
$(BUILD_DIR)/$(GRAPHICS_DIR)/%.o: $(GRAPHICS_DIR)/%.cpp | $(BUILD_DIR)/$(GRAPHICS_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to build shaders
shaders: $(SHADER_OBJS)

$(BUILD_DIR)/$(GRAPHICS_DIR)/shaders/%.spv: $(GRAPHICS_DIR)/shaders/% | $(SHADERS_BUILD_DIR)
	glslc $< -o $@

# Rule to build main.o
$(BUILD_DIR)/main.o: main.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create necessary directories
$(BUILD_DIR)/$(INPUT_DIR) $(BUILD_DIR)/$(CORE_DIR) $(BUILD_DIR)/$(GRAPHICS_DIR) $(SHADERS_BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)

