# Pongueira
### o novo pong!

This is singleplayer pong-like game. 
In order to play the game you have to install this library:

- freeglut3-dev 
- portaudio (if using OSX)
- rt-audio

To install freeglut3-dev you can use the following command on your console window

`sudo apt-get install freeglut3-dev`
`brew install freeglut3-dev`


To install portaudio you can use the following command on yout console window

`brew install portaudio`


To install rt-audio you can use the following command on yout console window

`sudo apt-get install librtaudio-dev`
`brew install rt-audio`

To play you have to use the command
`make play`

To understand our directory structure:
- inc/ = directory with .hpp files
- src/ = directory with .cpp files
in each directory we have 3 subdirectories:
- model/
- view/
- controller/
We used this structure to follow more carefully the MVC pattern