CC = g++
CFLAGS = -Wall
EXEC_NAME = engine
INCLUDES = main.cpp Scene.cpp Camera/Camera.cpp Importer/Importer.cpp Shaders/Shader.cpp Models/Cube.cpp Inputs/Input.cpp Mesh/Mesh.cpp Mesh/MeshVertex.cpp Mesh/MeshHalfEdge.cpp Mesh/MeshTriangle.cpp Mesh/MeshEdge.cpp Bezier/BezierCurve.cpp Bezier/BezierSurface.cpp Axis/Axis.cpp
LIBS = -lGL -lSDL2
OBJ_FILES = main.o Scene.o Camera.o Importer.o Shader.o Cube.o Input.o Mesh.o MeshVertex.o MeshHalfEdge.o MeshTriangle.o MeshEdge.o BezierCurve.o BezierSurface.o Axis.o

all :
	if [ ! -d "./3dEngine/" ];then\
        mkdir 3dEngine/;\
    fi

	if [ ! -d "./3dEngine/Models/" ];then\
        mkdir 3dEngine/Models/;\
    fi

	if [ ! -d "./3dEngine/Shaders/" ];then\
        mkdir 3dEngine/Shaders/;\
    fi
	
	$(CC) $(CFLAGS) -c $(INCLUDES)
	$(CC) -o 3dEngine/$(EXEC_NAME) $(OBJ_FILES) $(LIBS)

	cp Shaders/*.vert 3dEngine/Shaders/
	cp Shaders/*.frag 3dEngine/Shaders/
	cp Models/*.obj 3dEngine/Models/

	cp convert.sh 3dEngine/

	make clean

run : all
	3dEngine/./$(EXEC_NAME)
	make clean

clean :
	rm $(OBJ_FILES)

$(EXEC_NAME) : $(OBJ_FILES)
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)