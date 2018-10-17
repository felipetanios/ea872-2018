UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	LIBRARIES = -lglut -lGL -lGLU -lrt -lpthread -lasound -ljack -lportaudio -D__linux__ -std=c++11 -w
else 
	LIBRARIES = -framework GLUT -framework OpenGL -lportaudio  -DTARGET_OS_MAC -std=c++11 -w
endif




clean: 
	rm game || echo "" > /dev/null;

game:clean
	g++ src/main.cpp ./src/model/* ./src/view/* ./src/controller/* -Iinc $(LIBRARIES) -o game

play:game
	./game
	
all: play




cleanClient:
	rm client || echo "" > /dev/null;


buildClient: cleanClient
	g++ src/main.cpp  ./src/view/* ./src/controller/* -Iinc $(LIBRARIES) -o client

client:buildClient
	./client
