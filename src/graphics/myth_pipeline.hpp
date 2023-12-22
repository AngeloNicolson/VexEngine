//===-- myth_pipeline.hpp -------------------------------------------------===//
//
// Part of the Myth Engine, a Vulkan-based graphics engine.
//
// This file defines the MythPipeline class, managing the creation and
// configuration of a graphics pipeline.
//
//===----------------------------------------------------------------------===//

#ifndef MYTH_PIPELINE_H
#define MYTH_PIPELINE_H

#include "myth_engine_device.hpp"

#include <string>
#include <vector>

namespace myth_engine {

/**
 * @file
 * Part of the Myth Engine, a Vulkan-based graphics engine.
 */

/**
 * @brief Struct used to organize data to configure the pipeline.
 *
 * This struct is separated from the pipeline class to allow easy configuration
 * at the application layer and to facilitate sharing configuration between
 * multiple pipelines.
 */
struct PipelineConfigInfo {};

/**
 * @brief Handles the creation and management of a graphics pipeline.
 */
class MythPipeline {

public:
  /**
   * @brief Constructs a MythPipeline with provided file paths for vertex and
   * fragment shaders.
   * @param vertFilepath The file path for the vertex shader.
   * @param fragFilepath The file path for the fragment shader.
   */
  MythPipeline(const std::string &vertFilepath,
               const std::string &fragFilepath);

private:
  /**
   * @brief Reads the contents of a file and returns them as a vector of
   * characters.
   * @param filepath The file path of the file to be read.
   * @return The contents of the file as a vector of characters.
   */
  static std::vector<char> readFile(const std::string &filepath);

  /**
   * @brief Creates the graphics pipeline using the specified vertex and
   * fragment shader file paths.
   * @param vertFilepath The file path for the vertex shader.
   * @param fragFilepath The file path for the fragment shader.
   */
  void createGraphicsPipeline(const std::string &vertFilepath,
                              const std::string &fragFilepath);
};

} // namespace myth_engine

#endif
