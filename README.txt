Dylan Ko / dko@mymail.mines.edu
Trevor Kerr / tkerr@mymail.mines.edu
Kyle Moran / ksmoran@mymail.mines.edu

MorioKart : Konnan, Inti, HankHill
Final Project

Description: Drive the kart around the map. Drive as fast as you can
Lights: There is an unseen light in the middle of the map and the start light also affects the kart. When using the easter egg, the car is effected by another yellow light that follows you.(The starting light is our active one, super sain light is also active)
    materials are used on the kart as well as a texture.
Animation: The penguin flags and waddles and shakes his head.
Textures: Lots of textures, more than 2
Shaders:
    Lots of shader stuff.
    Light up when you use easter egg, go small when you go the wrong way too much
    See shaders for more, multipass rendering is used for the map (worked on ink, might not be done by the time we turn this in :) )
There is an easter egg


=======
Usage:
Drive around using the WASD keys and switch camera with the P key. 
After pushing P, click and drag to move the camera
******EASTER EGG******
Push up, left, left, right, right, down, down, down on the arrow keys
This gives you super speed.
**********************

Usage:

Compiling: make -B && fp.exe

Bugs: Penguin flashing weirdly

File Format for input:

/ = none
x = road
P = penguin
1-9 = check targets to make sure you go the right way
O = finish line
S = car start


Responsibilities:
Dylan - Built base map layout and file input, built start of the Kart being shown and most of the kart functionings and such. Lights on kart
Trevor Kerr: Did the penguin animation, movement, shader. Fixed lighting in several places. Helped with car movement.
Kyle Moran: Added the minimap and made pushed the screen to a frame buffer. Textured the kart. 
How Long:
40 Hours

Labs: 10

Fun: 10
