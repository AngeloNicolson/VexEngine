#pragma once

#include "vex_device.h"

#include <string>
#include <vector>

namespace vex {
	struct PipelineConfigInfo {
	};
	class VexPipeline {
	public:
		VexPipeline(
			VexDevice& device,
			const std::string& vertFilepath,
			const std::string fragFilepath,
			const PipelineConfigInfo& configInfo);
		~VexPipeline() {}

		VexPipeline(const VexPipeline&) = delete;
		void operator=(const VexPipeline&) = delete;

		static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

	private:
		static std::vector<char> readFile(const std::string& filepath);

		void createGraphicsPipeline(
			const std::string& vertFilepath,
			const std::string& fragFilepath,
			const PipelineConfigInfo& configInfo);

		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		VexDevice& vexDevice;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragmentShaderModule;
	};
}