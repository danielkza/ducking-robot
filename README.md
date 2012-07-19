ducking-robot
=============

Building instructions:

# Needed libraries and programs

* SDL 1.2
* SDL_image 1.2
* SDL_mixer 1.2
* SDL_ttf 2.0
* CMake 2.8
* Doxygen (optional for documentation generation)

# Build commands
Create a build folder:

* cd ducking_robot
* mkdir build
* cd build
* cmake ..

* Possibly choose paths for necessary libs.

* build with your build system of choice (e.g: make)
* Documentation built with 'make docs', to build/game/docs
* Some libraries are automatically copied to binary directory (which is build/bin),
  together with game assets.