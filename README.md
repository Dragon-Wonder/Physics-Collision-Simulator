# Cannon-Simulation

A really simple program that uses SDL library to simulate a cannon ball being fired.

I used to to test the waters on using SDL for graphics. Putting it on GitHub so that I may easily reference it later.

There are a few branches of this that use different programming languages.

It has been released in the Public Domain meaning you can use it however you wish.

# Running the program

In order to run the program properly you will need to download SDL2-2.0.3 or higher AND SDL2-image (these are two different libraries). 

A link for SDL2 can be found [here] (https://www.libsdl.org/download-2.0.php).

A link for SDL2 image can be found [here](https://www.libsdl.org/projects/SDL_image/) .

You will want to download the Runtime Binaries (if you have windows I recommend 32 bit over 64 bit; even if you have a 64 bit computer).

When all is said and done you should have the following files in your folder. (Also note that you should be able to put the dll files in your C drive).

```
FolderName\
FolderName\Cannon.exe
FolderName\SDL2.dll
FolderName\SDL2_image.dll
```

Let me know if I am missing a dependences that I might have forgotten about due to my own system set up.

# Using the Program

The first time you run the program a Config.ini will be created it should look something like below:

```
Config File for the Cannon.exe
1.2.4.60-b
Screen Width: 640
Screen Height: 480
Log Ball's path: 0
Enable Drag Mode (experimental): 0
Draw Ball path on screen: 0
```

You can use this to change things like the screen height and width, and other stuff.

"Log Ball's Path" (if enabled with a 1 instead of 0), will cause a a file called "logfile.log", which will contain the placement of every ball on every update (it gets quite big as you can guess).

"Enable drag Mode" (if enabled with a 1 instead of 0), will cause the cannonballs to be effected by drag. It is listed as experimental because I based the equations used off of several things I found 
online and I cannot verify how accurate it is.

"Draw Ball path on Screen" (if enabled with a 1 instead of 0), will cause small pixels to trail after each ball.

Once you start the program you will be greeted with a beautiful white screen. To create a new cannonball you click (and hold down the button) 
you can now move your move around to change the angle that the new cannonball will be fired at; as well as the longer the line the faster it will go. 
When you are happy with the angle and speed simply release the mouse button and it will be created at the point where you clicked the mouse button down.

You can also press the following keys to yield effects

```
K = All Balls will be "killed".
R = All motion will be stopped.
Q / ESC = Program will exit.
```
