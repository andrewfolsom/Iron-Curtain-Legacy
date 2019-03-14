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
        bool startMenu = true;
        bool gamePlay = false;
		GLuint nickImage;
		GLuint andrewImage;
		GLuint spencerImage;
		GLuint chadImage;
		GLuint benImg;
        GLuint ironImage;
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
		int weaponType;

		//EDITS - Jackson
		enum MOVETYPE { RUSH, STRAFE, CIRCLING, BANK, DIAG_RUSH };
		Vec spawnPos;
		float angle;
		float angleSpd;
		float t;
		float destX, destY;
		int speedMul = 1;
		int initFlag = 0;
		int moveFlag = -1;
		int movPattern = RUSH;
		int destOffset = 700;

		//
	public:
		Ship();
};

/*
 *OPFOR Class
 *	Jackson
 */
class opForShip
{
	public:
		Vec pos;
		float vel[4];
		float speed;
		float color[3] = {255.0, 0.0, 0.0};
		int weaponType;
		int movPattern = 1;
		int speedMul = 1;

		float angle = 90.0;
		float slope;
		float t = 0.0;
		Vec spawnPos;
		int destOffset = 0.0;
		
		opForShip();
		opForShip(const opForShip&);
		opForShip& operator= (const opForShip&);
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
//-----------------------------
		//Jackson
		opForShip opFor[100];
		int numOpFor;
//------------------------------
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
