<a name="top"></a> [![LinkedIn][linkedin-shield]][linkedin-url]

<br />
<div align="center">
  <h1 align="center">VexEngine</h1>

<p align="center">
    A hands-on exploration of graphics programming and game engine development.
    <br/>
    <a href="https://github.com/AngeloNicolson/VexEngine">Explore</a>
    •
    <a href="https://github.com/AngeloNicolson/VexEngine/issues">Report a Bug</a>
    •
    <a href="https://github.com/AngeloNicolson/VexEngine/issues">Suggest a Feature</a>
  </p>
</div>

______________________________________________________________________

<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#about-the-project">About the Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li><a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

______________________________________________________________________

## About the Project

VexEngine is a personal project created to deepen my understanding of
real-time graphics and engine architecture. While there are many great
engines available, building one from the ground up offers insight no tutorial
alone can provide.

The long-term goal is to develop a lightweight, flexible engine capable of:

- Procedurally generating environments
- Supporting simple AI systems
- Building functional menus with minimal setup

While still in early stages, the intention is to eventually build a
foundation for prototyping my own game ideas without financial barriers or
engine bloat. This is purely so I can understand every aspect of a game
engine and work on becoming highly skilled in memory and resource management.

This is a learning project, but steeped in personal ambition.

<p align="right">(<a href="#top">back to top</a>)</p>

______________________________________________________________________

### Built With

- [![C++]][c++-url]
- [![vulkan]][vulkan-url]

<p align="right">(<a href="#top">back to top</a>)</p>

______________________________________________________________________

## Getting Started

To get the current build running locally, make sure you have the following
dependencies installed:

### Prerequisites

- [GLFW](https://www.glfw.org/)
- [glm](https://github.com/g-truc/glm)
- [Vulkan SDK](https://vulkan.lunarg.com/)

### Installation

1. Clone the repository: $$ git clone
   https://github.com/AngeloNicolson/VexEngine.git $$
1. A make build system is currently being added to simplify setup.

<p align="right">(<a href="#top">back to top</a>)</p>

______________________________________________________________________

## Usage

At this stage, VexEngine primarily serves as a sandbox for myself to learn
and test renderingi, animation and lighting systems.

For more details on the underlying implementation, I recommend the excellent
[Vulkan Tutorial](https://vulkan-tutorial.com/), which guided much of this
setup.

<p align="right">(<a href="#top">back to top</a>)</p>

______________________________________________________________________

## Roadmap

- [x] Initial README
- [x] Refactor project structure
- [ ] Fragment Interpolation
- [ ] Add billboard rendering
- [ ] Support multiple dynamic lights
- [ ] Add specular lighting support
- [ ] Implement alpha blending

See the [open issues](https://github.com/AngeloNicolson/VexEngine/issues) for
a full list of planned features and improvements.

<p align="right">(<a href="#top">back to top</a>)</p>

______________________________________________________________________

## Contributing

Contributions are always welcome!

If you have an idea, bug fix, or improvement in mind:

1. Fork the repository
1. Create a feature branch\
   $$ git checkout -b feature/my-feature $$
1. Commit your changes\
   $$ git commit -m 'Add my feature' $$
1. Push to your fork\
   $$ git push origin feature/my-feature $$
1. Open a Pull Request

Feel free to open an issue with the "enhancement" tag or drop a suggestion.
And if you find this project helpful, consider giving it a star—thank you!

<p align="right">(<a href="#top">back to top</a>)</p>

______________________________________________________________________

## Contact

Connect with me on [![LinkedIn][linkedin-shield]][linkedin-url] or reach out
via [![email][email-shield]][email-url].

Project Link: [VexEngine](https://github.com/AngeloNicolson/VexEngine)

<p align="right">(<a href="#top">back to top</a>)</p>

______________________________________________________________________

## Acknowledgments

- [Alexander Overvoorde](https://vulkan-tutorial.com/) – Vulkan tutorials
- [ChooseALicense.com](https://choosealicense.com) – Open source license
  reference
- [Shields.io](https://shields.io) – Badge generator
- [React Icons](https://react-icons.github.io/react-icons/search)

<p align="right">(<a href="#top">back to top</a>)</p>

______________________________________________________________________

<!-- MARKDOWN LINKS & IMAGES -->

[c++]: https://img.shields.io/badge/C++-blue?style=for-the-badge&logo=cplusplus&logoColor=white
[c++-url]: http://www.cplusplus.org/
[email-shield]: https://img.shields.io/badge/outlook-black.svg?style=for-the-badge&logo=email&colorB=555
[email-url]: mailto:AngeloNicolson.github@outlook.com
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/angelo-nicolson-5ab4b772/
[vulkan]: https://img.shields.io/badge/Vulkan-darkred?style=for-the-badge&logo=vulkan&logoColor=white
[vulkan-url]: https://www.vulkan.org/
