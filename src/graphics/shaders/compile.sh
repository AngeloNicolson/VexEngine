#!/bin/bash

# Path to glslc (Vulkan SDK's GLSL compiler)
GLSLC="/home/Angel/Programs/Vulkan_SDK/1.4.313.0/x86_64/bin/glslc"

# Source shader file paths
VERTEX_SHADER="simple_shader.vert"
FRAGMENT_SHADER="simple_shader.frag"

# Output SPIR-V file paths
OUTPUT_VERTEX_SPIRV="../../../build/Shaders/simple_shader.vert.spv"
OUTPUT_FRAGMENT_SPIRV="../../../build/Shaders/simple_shader.frag.spv"

# Compile shaders to SPIR-V
$GLSLC $VERTEX_SHADER -o $OUTPUT_VERTEX_SPIRV
$GLSLC $FRAGMENT_SHADER -o $OUTPUT_FRAGMENT_SPIRV

echo "Shader compilation completed."

