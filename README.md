# 3dEngine

3dEngine is a 3d graphic engine written in c++ using OpenGL and the SDL.

### How to build it  

3dEngine can only be built on a linux os.

To build it, you will need some library :

- OpenGL (usualy already installed with common linux distrib)
- SDL2
- GLM

Once you have installed all the libraries, download the repo and use make to build it.
The executable is in a directory named 3dEngine/. There are also two other directories : Models/ and Shader/. Shader/ is needed to run the executable. Models/ is not needed but there are .obj example files in it.

### How to use it

Run the executable named "engine" in a terminal. Write the relative path of the .obj file you want to render. Example files are in the Models/ directory.

Move the camera with the WSAD or ZQSD key. Move it up and down with the Q or A key and the E key.
Rotate it with the mouse.

Presse Enter to save a .png screenshot in the directory of the executable.

Presse Escape to quit.

### Compatibility

.obj files only, with no texture coordinate (uv). Files with a number of vertex > 1000 can be slow to load (a few seconds).
Including materials is useless for now.