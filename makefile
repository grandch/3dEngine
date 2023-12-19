CC = g++
CFLAGS = -Wall
BUILD_DIR = build
EXEC_NAME = engine
INCLUDES = laplacianTransform.cpp laplacianSmooth.cpp laplacianMat.cpp laplacianIt.cpp Scene/Scene.cpp Camera/Camera.cpp Importer/Importer.cpp Shaders/Shader.cpp Inputs/Input.cpp Mesh/Mesh.cpp Mesh/MeshVertex.cpp Mesh/MeshHalfEdge.cpp Mesh/MeshTriangle.cpp Mesh/MeshEdge.cpp Bezier/BezierCurve.cpp Bezier/BezierSurface.cpp Scene/Axis/Axis.cpp lib/stb_image.cpp Light/PointLight.cpp Scene/LightManager.cpp Scene/BezierManager.cpp Scene/MeshManager.cpp
LIBS = -lGL -lSDL2
OBJ_FILES = Scene.o Camera.o Importer.o Shader.o Input.o Mesh.o MeshVertex.o MeshHalfEdge.o MeshTriangle.o MeshEdge.o BezierCurve.o BezierSurface.o Axis.o stb_image.o PointLight.o LightManager.o MeshManager.o BezierManager.o

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
	
	$(CC) $(CFLAGS) -c $(INCLUDES) -I ~/eigen
	$(CC) -o $(BUILD_DIR)/iteratif laplacianIt.o $(OBJ_FILES) $(LIBS)
	$(CC) -o $(BUILD_DIR)/mat laplacianMat.o $(OBJ_FILES) $(LIBS)
	$(CC) -o $(BUILD_DIR)/smooth laplacianSmooth.o $(OBJ_FILES) $(LIBS)
	$(CC) -o $(BUILD_DIR)/transf laplacianTransform.o $(OBJ_FILES) $(LIBS)

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