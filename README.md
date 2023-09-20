# 3dEngine

3dEngine is a 3d real-time rendering engine written in C++ using OpenGL, GLM and SDL2.

This personal project is used for my computer graphics lessons in the context of the [IAFA master](https://www.univ-tlse3.fr/decouvrir-nos-diplomes/master-parcours-intelligence-artificielle-fondements-et-applications-iafa) of the Paul Sabatier University. Some features (such as Bezier curves and surfaces) are implemented for exercises.

![render](/render.png)

### Informations

Current openGL version : 3.3 core.

### How to build it

3dEngine can only be built on a linux os.

To build it, you will need some libraries :

- OpenGL (usualy already installed with common linux distrib)
- SDL2
- GLM

Once you have installed all the libraries, download the repo and use make to build it. The executable is in a directory named build/. There are also two other directories : Models/ and Shader/. Shader/ is needed to run the executable. Models/ is not needed but there are basics .obj example files in it.

### How to use it

Run the executable named "engine" in a terminal.

Move the camera with the WSAD or ZQSD key. Move it up and down with the Q or A key and the E key.
Rotate it with the mouse while pressing the wheel. The first wheel input will rotate the camera in an unpredictable way and then it works well. This will be fixed soon (see [issue 12](https://github.com/grandch/3dEngine/issues/12)).

Presse Escape to quit.

3D objects and lights (only point light for now) can be instancited in the `Scene::initScene()` method and rendered in the `Scene::mainLoop()`. This is not conveniant and scene management will be ehanced soon (see [issue 13](https://github.com/grandch/3dEngine/issues/13)).

3D objects can be Bezier curves, Bezier surfaces or meshes.

### Compatibility

.obj files only, with no texture coordinate (uv). Files with a number of vertex > 1000 can be slow to load (a few seconds).
Including materials is useless for now.

### Perspectives

Some ehancements or additions are considered (e.g. Blinn-Phong model for BRDF or scene management ehancement).

This project will evolve with future exercises and lesson of my master over the year.

You can check what is planned to be done here : [3DEngine GitHub Project](https://github.com/users/grandch/projects/2/views/1).