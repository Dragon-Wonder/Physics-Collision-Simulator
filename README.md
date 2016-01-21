# Physics-Collision-Simulator

A really simple program that uses SDL library to simulate cannonballs being fired and colliding and bouncing off of each other.

I used it to test the waters on using SDL for graphics, and collisions. Putting it on GitHub so that I may easily reference it later.

There are a few branches of this that use different programming languages.

# License

This program has been released in the Public Domain meaning you can use it however you wish, read the license file [here](https://github.com/GamerMan7799/Physics-Collision-Simulator/blob/master/LICENSE.md)
or you can read [here](http://unlicense.org) for infomation of the Unlicense in general.

## Summary of what license means

* First and foremost, this code is "AS IS", meaning we cannot be sued if it doesn't work

* You can modify this code any way you wish without credit (but we would certainty appreciate it)

* You can any/all of this code for private and/or commerical use

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

The first time you run the program a "Config.ini" will be created it should look something like below:

```
Config File for the Cannon.exe
1.0.1-RC
Screen Width: 640
Screen Height: 480
Log Ball's path: 0
Enable Drag Mode (experimental): 0
Draw Ball path on screen: 0
Collision Method: 1
```

You can use this to change things like the screen height and width, and other stuff.

"Log Ball's Path" (if enabled with a 1 instead of 0), will cause a a file called "logfile.log", which will contain the placement of every ball on every update (it gets quite big as you can guess).

"Enable drag Mode" (if enabled with a 1 instead of 0), will cause the cannonballs to be effected by drag. It is listed as experimental because I based the equations used off of several things I found
online and I cannot verify how accurate it is.

"Draw Ball path on Screen" (if enabled with a 1 instead of 0), will cause small pixels to trail after each ball.

"Collision Method" you can change how the balls will interact with each other when they collide based on the number in the table below.

| Name                    | Number | Explanation                                                                                                |
|-------------------------|--------|------------------------------------------------------------------------------------------------------------|
| CollideElastic          | 0      | This is normal collision in a perfect world without  friction, they hit and bounce off, no energy is lost. |
| CollideInelastic        | 1      | This is the real life situation. Balls collide and,  bounce off of each other losing some energy.          |
| CollidePerfectInelastic | 2      | In this method the balls collide and then stick together.                                                  |
| CollideNone             | 3      | The balls do not collide and pass through each other.                                                      |


Once you start the program you will be greeted with a beautiful black screen. To create a new cannonball you click (and hold down the button)
you can now move your move around to change the angle that the new cannonball will be fired at; as well as the longer the line the faster it will go.
When you are happy with the angle and speed simply release the mouse button and it will be created at the point where you clicked the mouse button down.

The longer you hold down the mouse button the larger the radius of the ball will be and as a result, the more mass the ball will have. You can get an
idea of how much mass a ball has based on how transparent it is, the more transparent it is the lighter its mass.

You can also press the following keys to yield effects

```
K = All Balls will be "killed".
R = All motion will be stopped.
Q / ESC = Program will exit.
```
