CC = g++
CFLAGS = -Wall
EXEC_NAME = engine
INCLUDES = main.cpp Scene.cpp Shaders/Shader.cpp Models/Cube.cpp Inputs/Input.cpp
LIBS = -lGL -lSDL2
OBJ_FILES = main.o Scene.o Shader.o Cube.o Input.o
INSTALL_DIR = /usr/bin

all :
	$(CC) $(CFLAGS) -c $(INCLUDES) $(LIBS)
	$(CC) $(CFLAGS) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)

start : all
	./$(EXEC_NAME)

clean :
	rm $(EXEC_NAME) $(OBJ_FILES)

$(EXEC_NAME) : $(OBJ_FILES)
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

%.o: %.cc
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

%.o: %.c
	gcc $(CFLAGS) $(INCLUDES) -o $@ -c $<

install :
	cp $(EXEC_NAME) $(INSTALL_DIR)