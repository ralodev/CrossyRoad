<div id="top"></div>

<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![License: GPL v3.0](https://img.shields.io/badge/License-GPL%20v3.0-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
![C](https://img.shields.io/badge/Language-C-blue)
![C++](https://img.shields.io/badge/Language-C++-blue)
![OpenGL](https://img.shields.io/badge/Technology-OpenGL-red)
![GLUT](https://img.shields.io/badge/Technology-GLUT-red)
![Visual Studio Community](https://img.shields.io/badge/IDE-Visual%20Studio%20Community-blue)

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a https://github.com/Raul-Lopez-Cruz/CrossyRoad">
    <img src="imagenes/GAME.png" alt="Juego">
  </a>

  <h3 align="center">CrossyRoad</h3>
</div>


# CrossyRoad

A game based on the popular game Crossy Road, developed in C and C++ using OpenGL and GLUT for 3D graphics. It was created to pass the 3D Graphics `(SCC-1010)` subject in the Computer Systems Engineering career.


## Getting started
CrossyRoad is a .sln project, so it needs to be run from an IDE.
This game is a .sln project, meaning it is not a standalone executable. You need to have the correct libraries (included in the project) and the Visual Studio IDE installed to run it.

### Prerequisites
- Visual Studio Community (available at https://visualstudio.microsoft.com/es/vs/older-downloads/)
- Required libraries and .dll files are included in the project.

### Installation
Once you have Visual Studio Community installed:
1. Clone or download the repository
```
git clone https://github.com/ralo-dev/CrossyRoad.git
```
2. Open the project in Visual Studio by double-clicking on the solution file: CrossyRoad.sln.
3. Run the project in "DEBUG" mode with x86 architecture using the Local Windows Debugger.

## How to Play

The game has a simple mechanic: help the chicken cross the road. You gain points each time you reach the end of the map. If you get hit by a car, you go back to the starting point.

### Controls
- Use W, A, S, D to move the chicken around
- Use the spacebar to make the chicken jump forward
- Left-click and move the mouse to move the camera
- Right-click and move the mouse up and down to adjust the zoom

## License
This project is licensed under the GNU GPL 3.0 License. See the LICENSE.txt file for more information.

### Authors
- [Jaanaip](https://github.com/jaanaip)
- [Ralo-dev](https://github.com/ralo-dev)

### Sources
- [CrossyRoad](https://www.crossyroad.com/)
- [Learn OpenGL](https://learnopengl.com/)
- [GL programming](https://www.glprogramming.com/red/index.html)
- [OpenGL wiki](https://www.khronos.org/opengl/wiki/OpenGL_Type)
- [Jumpy Road by mluders](https://github.com/mluders/jumpy-road)
