#include <vector>
#include <string>

namespace GameEngine
{
  namespace Graphics
  {
    class GraphicsPipeline
    {
    public:
      GraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath);

    private:
      /* @breif Read in a file
       *
       *@param (filepath) reads in a shader file path
       */
      static std::vector<char> readFile(const std::string& filepath);

      // Helper Function
      void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath);
    };
  } // namespace Graphics
} // namespace GameEngine
