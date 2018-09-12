UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	LIBRARIES = -lglut -lGL -lGLU
else 
	LIBRARIES = -framework GLUT -framework OpenGL 
endif



game: 
	g++ src/main.cpp ./src/model/* ./src/view/* -Iinc $(LIBRARIES) -o game

	
all: game


