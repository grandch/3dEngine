CC = g++
CFLAGS = -Wall
EXEC_NAME = engine
INCLUDES = main.cpp Scene.cpp Camera.cpp Importer.cpp Shaders/Shader.cpp Models/Cube.cpp Inputs/Input.cpp Mesh/Mesh.cpp Mesh/MeshVertex.cpp Mesh/MeshHalfEdge.cpp Mesh/MeshTriangle.cpp Mesh/MeshEdge.cpp
LIBS = -lGL -lSDL2
OBJ_FILES = main.o Scene.o Camera.o Importer.o Shader.o Cube.o Input.o Mesh.o MeshVertex.o MeshHalfEdge.o MeshTriangle.o MeshEdge.o
INSTALL_DIR = /usr/bin

all :
	$(CC) $(CFLAGS) -c $(INCLUDES)
	g++ -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)
	make clean

run : all
	./$(EXEC_NAME)
	make clean

clean :
	rm $(OBJ_FILES)

$(EXEC_NAME) : $(OBJ_FILES)
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

%.o: %.cc
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

%.o: %.c
	gcc $(CFLAGS) $(INCLUDES) -o $@ -c $<