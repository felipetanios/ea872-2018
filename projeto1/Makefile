UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	LIBRARIES = -lglut -lGL -lGLU -D__linux__ -std=c++11
else 
	LIBRARIES = -framework GLUT -framework OpenGL -DTARGET_OS_MAC -std=c++11
endif



game:clean
	g++ src/main.cpp ./src/model/* ./src/view/* -Iinc $(LIBRARIES) -o game

	
all: game

clean: 
	rm game || echo "haaaaa";

