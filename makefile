CC = g++
CFLAGS = -Wall
BUILD_DIR = build
EXEC_NAME = engine
INCLUDES = main.cpp Scene/Scene.cpp Camera/Camera.cpp Importer/Importer.cpp Shaders/Shader.cpp Inputs/Input.cpp Mesh/Mesh.cpp Mesh/MeshVertex.cpp Mesh/MeshHalfEdge.cpp Mesh/MeshTriangle.cpp Mesh/MeshEdge.cpp Bezier/BezierCurve.cpp Bezier/BezierSurface.cpp Scene/Axis/Axis.cpp lib/stb_image.cpp Light/PointLight.cpp Scene/LightManager.cpp Scene/BezierManager.cpp Scene/MeshManager.cpp Scene/SceneManager.cpp
LIBS = -lGL -lSDL2
OBJ_FILES = main.o Scene.o Camera.o Importer.o Shader.o Input.o Mesh.o MeshVertex.o MeshHalfEdge.o MeshTriangle.o MeshEdge.o BezierCurve.o BezierSurface.o Axis.o stb_image.o PointLight.o LightManager.o MeshManager.o BezierManager.o SceneManager.o

all :
	if [ ! -d "./$(BUILD_DIR)/" ];then\
        mkdir $(BUILD_DIR)/;\
    fi

	if [ ! -d "./$(BUILD_DIR)/Models/" ];then\
        mkdir $(BUILD_DIR)/Models/;\
    fi

	if [ ! -d "./$(BUILD_DIR)/Shaders/" ];then\
        mkdir $(BUILD_DIR)/Shaders/;\
    fi
	
	$(CC) $(CFLAGS) -c $(INCLUDES)
	$(CC) -o $(BUILD_DIR)/$(EXEC_NAME) $(OBJ_FILES) $(LIBS)

	cp Shaders/*.vert $(BUILD_DIR)/Shaders/
	cp Shaders/*.frag $(BUILD_DIR)/Shaders/
	cp Shaders/*.jpg $(BUILD_DIR)/Shaders/
	cp Models/*.obj $(BUILD_DIR)/Models/

	cp convert.sh $(BUILD_DIR)/

	make clean

run : all
	$(BUILD_DIR)/./$(EXEC_NAME)
	make clean

clean :
	rm $(OBJ_FILES)

$(EXEC_NAME) : $(OBJ_FILES)
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)