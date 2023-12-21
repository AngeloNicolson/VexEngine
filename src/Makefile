CXX = g++
CXXFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

CORE_DIR = core
GRAPHICS_DIR = graphics

# List all the source files
SRCS = $(wildcard $(CORE_DIR)/*.cpp) $(wildcard $(GRAPHICS_DIR)/*.cpp) main.cpp

# Derive object file names from source files
OBJS = $(SRCS:.cpp=.o)

# Final executable name
TARGET = VulkanTest

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Pattern rule to build .o files from .cpp files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

