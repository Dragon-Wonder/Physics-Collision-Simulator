/**********************************************************************************************************************************************************************/
#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string>
/**********************************************************************************************************************************************************************/
//Defines
#define PI 3.1415926535897
#define DRAG
/**********************************************************************************************************************************************************************/
//Constants
const double Rad_Convert = PI / 180.0;
const double Drag_cofficient = 0.47;
/**********************************************************************************************************************************************************************/
//Structs and type defs
typedef unsigned char uchar;
typedef unsigned int uint;
struct ball {
	uint x;
	uint y;
	double velx;
	double vely;
	double area;
	double mass;
};
typedef struct ball cannonball;
/**********************************************************************************************************************************************************************/
//Gloabls
cannonball cann_ball;
double acc_y = -9.81;
double acc_x = 0.00;
SDL_Surface* ball = NULL, *screen = NULL;
bool bln_SDL_started = false;
/**********************************************************************************************************************************************************************/
//Protytpe Functions
namespace Screen {
	void start(void);
	void update(void);
	SDL_Surface* loadIMG(std::string);
	void applySurface(int, int, SDL_Surface*, SDL_Surface*);
	void end(void);
	void FreeIMG(SDL_Surface*, ...);
	void failure(void);
	
	const uint width = 640;
	const uint height = 480;
	const uint bpp = 32;
};
namespace Ball {
	void make(void);
	void update(void);
	
	const uchar radius = 5;
	const uchar fire_angle = 45;
	const uchar fire_velocity = 50;
};
namespace Tick {
	void wait(void);
	void sleep(uint);
	
	const uchar fps = 60;
	
	float deltat = 0;
};
namespace Drag {
	void calc_values(void);
	void update_acc(void);
};
/**********************************************************************************************************************************************************************/
int main(void) {
	Ball::make();
	Screen::start();
	if (bln_SDL_started == false) {Screen::failure(); return 1;}
	Tick::deltat = (float)(1.00 / Tick::fps);
	#ifdef DRAG
		Drag::calc_values();
	#endif
	do {
		#ifdef DRAG
			Drag::update_acc();
		#endif
		Ball::update();
		Screen::update();
		if (bln_SDL_started == false) {Screen::failure(); return 1;}
		Tick::wait();
	} while (cann_ball.y >= 5);
	Screen::end();
	return 0;
}
/**********************************************************************************************************************************************************************/
void Screen::start(void){
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		bln_SDL_started = false;
		return;
	} else {bln_SDL_started = true;}
	screen = SDL_SetVideoMode(Screen::width,Screen::height,Screen::bpp, SDL_SWSURFACE);
	if (screen == NULL) {
		bln_SDL_started = false;
		return;
	}
	SDL_WM_SetCaption("Cannon Physics Simulation", NULL);
	ball = Screen::loadIMG("res/ball.bmp");
	
	Screen::applySurface(0,5,ball,screen);
}
/**********************************************************************************************************************************************************************/
void Screen::update(void) {
	Screen::applySurface(cann_ball.x,cann_ball.y,ball,screen);
	if (SDL_Flip(screen) == -1) {bln_SDL_started = false; return;}
	
}
/**********************************************************************************************************************************************************************/
SDL_Surface* Screen::loadIMG(std::string filename) {
	SDL_Surface* temp = SDL_LoadBMP(filename.c_str());
	SDL_Surface* surface = NULL;
	
	if(temp) {
		surface = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
	}
	return surface;
}
/**********************************************************************************************************************************************************************/
void Screen::applySurface(int x, int y, SDL_Surface* source, SDL_Surface* destination) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    // Blit the surface:
    SDL_BlitSurface(source, NULL, destination, &offset);
}
/**********************************************************************************************************************************************************************/
void Screen::FreeIMG(SDL_Surface* surf, ...) {
    if (surf != NULL) {
        SDL_FreeSurface(surf);
    }
}
/**********************************************************************************************************************************************************************/
void Screen::end(void) {
	Screen::FreeIMG(ball);
	Screen::FreeIMG(screen);
	Tick::wait();
	getchar();
}
/**********************************************************************************************************************************************************************/
void Screen::failure(void) {
	printf("\nSDL_Init failed: %s\n", SDL_GetError());
	getchar();
}
/**********************************************************************************************************************************************************************/
void Ball::make(void) {
	cann_ball.x = 0;
	cann_ball.y = 5;
	cann_ball.velx = (double)(Ball::fire_velocity) * (cos(Ball::fire_angle * Rad_Convert));
	cann_ball.vely = (double)(Ball::fire_velocity) * (cos(Ball::fire_angle * Rad_Convert)); 
}
/**********************************************************************************************************************************************************************/
void Ball::update(void) {
	cann_ball.x = (uint) round(cann_ball.velx * Tick::deltat + 0.5 * acc_x * pow(Tick::deltat,2));
	cann_ball.velx = (cann_ball.velx + acc_x * Tick::deltat);
	cann_ball.y = (uint) round(cann_ball.vely * Tick::deltat + 0.5 * acc_y * pow(Tick::deltat,2));
	cann_ball.vely = (cann_ball.vely + acc_y * Tick::deltat);
}
/**********************************************************************************************************************************************************************/
void Tick::wait(void) {
	static int fSleep;
	if (fSleep == 0) {fSleep = (int) round(1000.0 / Tick::fps);}
	Tick::sleep(fSleep);
}
/**********************************************************************************************************************************************************************/
void Tick::sleep(uint milliseconds) {
	unsigned long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause ) {now = clock();}
}
/**********************************************************************************************************************************************************************/
void Drag::calc_values(void) {
	double density = 2.1546;
	double volume;
	cann_ball.area = (double) (2.0 * PI * pow(Ball::radius,2));
	volume = (double) ((4.0/3.0) * PI * pow(Ball::radius,3));
	cann_ball.mass = density * volume;
}
/**********************************************************************************************************************************************************************/
void Drag::update_acc(void) {
	double rho_air = 1.2754;
	double flow_velocity = pow(cann_ball.velx,2) + pow(cann_ball.vely,2);
	double Drag_Force = (double) (0.5 * rho_air * flow_velocity * Drag_cofficient * cann_ball.area);
	double Drag_Acc = (double) (Drag_Force / cann_ball.mass);
	
	double angle = atan(cann_ball.vely / cann_ball.velx);
	
	acc_x -= Drag_Acc * cos (angle);
	if (cann_ball.vely < 0) {acc_y += Drag_Acc * sin (angle);}
	else {acc_y -= Drag_Acc * sin (angle);}
}
/**********************************************************************************************************************************************************************/

