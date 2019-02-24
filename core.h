/*
 * Core Class Declerations for Iron Curtain
 */

#ifndef CORE
#define CORE

#include "andrewF.h"

typedef float Flt;
typedef float Vec[3];

/*
 * Image Class
 */

class Image
{
	public:
		int width, height;
		unsigned char *data;
		Image(const char *);
		~Image();
};

/*
 * Global Class
 */

class Global
{
	public:
		int xres, yres;
		char keys[65536];
		bool creditPage = false;
		GLuint nickImage;
		GLuint andrewImage;
		GLuint spencerImage;
		GLuint chadImage;
		GLuint benImg;
		static Global& getInstance();
	private:
		Global();
		Global(Global const&);
		Global& operator=(Global const&);
};

/*
 * Ship Class
 */

class Ship
{
	public:
		Vec pos;
		float vel[4];
		float speed;
		float color[3];
		float radius;
		struct EnemyShip *nextShip;
		struct EnemyShip *prevShip;
		int weaponType;
	public:
		Ship();
};

/*
 * Bullet Class
 */

class Bullet
{
	public:
		Vec pos;
		Vec vel;
		float color[3];
		timespec time;
	public:
		Bullet();
};

/*
 * Game Class
 */

class Game
{
	public:
		Ship ship;
		Bullet *barr;
		Missile *marr;
		int nbullets;
		int nmissiles;
		Flt radius;
		timespec bulletTimer;
		timespec missileTimer;
		timespec thrustTimer;
		bool thrustOn;
	public:
		Game();
		~Game();
};

/*
 * X11 Wrapper Class
 */

class X11_wrapper
{
	private:
		Display *dpy;
		Window win;
		GLXContext glc;
	public:
		X11_wrapper();
		~X11_wrapper();
		void set_title();
		void setup_screen_res(const int, const int);
		void swapBuffers();
		void clearWindow();
		bool getXPending();
		XEvent getXNextEvent();
};

#endif
