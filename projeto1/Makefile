UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	LIBRARIES = -lglut -lGL -lGLU
else 
	LIBRARIES = -framework GLUT -framework OpenGL 




game: 
	g++ ./src/* ./src/*/* -I ./inc/* $(LIBRARIES) -o game

	
all: game


