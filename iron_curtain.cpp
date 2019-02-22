//                                                                           
//program: asteroids.cpp                                                     
//author:  Gordon Griesel                                                    
//date:    2014 - 2018                                                       
//mod spring 2015: added constructors                                        
//This program is a game starting point for a 3350 project.                  
//            

#include <iostream>                                                          
#include <cstdlib>                                                           
#include <cstring>                                                           
#include <unistd.h>                                                          
#include <ctime>                                                             
#include <cmath>                                                             
#include <X11/Xlib.h>                                                        
//#include <X11/Xutil.h>                                                     
//#include <GL/gl.h>                                                         
//#include <GL/glu.h>                                                        
#include <X11/keysym.h>                                                      
#include <GL/glx.h>                                                          
//#include "log.h"
#include "fonts.h"
#include <stdio.h>
#include "core.h"
#include "andrewF.h"

//defined types
typedef float Flt;
typedef float Vec[3];
typedef Flt Matrix[4][4];

//macros
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%a)
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b) ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
							 (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define PI 3.141592653589793
#define ALPHA 1
const int MAX_BULLETS = 1000;
const float MAX_VELOCITY = 15;
const Flt MINIMUM_ASTEROID_SIZE = 60.0;

//-------------------------------------------------------------------------- :                                                                         
//Setup timers                                                               
const double physicsRate = 1.0 / 60.0;                                       
const double oobillion = 1.0 / 1e9;                                          
extern struct timespec timeStart, timeCurrent;                               
extern struct timespec timePause;                                            
extern double physicsCountdown;                                              
extern double timeSpan;                                                      
extern double timeDiff(struct timespec *start, struct timespec *end);        
extern void timeCopy(struct timespec *dest, struct timespec *source);        
extern void displayNick(float x, float y, GLuint texture);
extern void displayChad(float x, float y, GLuint texture);
extern void displayAndrew(float x, float y, GLuint texture);
extern void displaySpencer(float x, float y, GLuint texture);
extern void BenjaminG(float x, float y, GLuint texture);
//-------------------------------------------------------------------------- 

Image img[5] = {
	"./img/NICKJA.jpg",
	"./img/andrewimg.png",
	"./img/spencerA.jpg",
	"./img/chad-egg.jpg",
	"./img/BGarza.jpg"
};

Global& gl = Global::getInstance();

Game g;

X11_wrapper x11;

Weapon *wpn = new Basic;

//Function Prototypes
double getTimeSlice(timespec* bt);
float convertToRads(float angle);
void init_opengl(void);
int check_keys(XEvent *e);
void physics();
void render();

int main()
{
	init_opengl();
	srand(time(NULL));
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
	int done = 0;
	while (!done) {
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			done = check_keys(&e);
		}
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		timeCopy(&timeStart, &timeCurrent);
		physicsCountdown += timeSpan;
		while (physicsCountdown >= physicsRate) {
			physics();
			physicsCountdown -= physicsRate;
		}
		render();
		x11.swapBuffers();
		x11.clearWindow();
	}
	cleanup_fonts();
	return 0;
}

void init_opengl(void)
{
	glEnable(GL_TEXTURE_2D);

	glViewport(0, 0, gl.xres, gl.yres);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	
	glGenTextures(1, &gl.nickImage);
	glGenTextures(1, &gl.andrewImage);
	glGenTextures(1, &gl.spencerImage);
	glGenTextures(1, &gl.chadImage);
	glGenTextures(1, &gl.benImg);


	//Clear the screen to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

void normalize2d(Vec v)
{
	Flt len = v[0]*v[0] + v[1]*v[1];
	if (len == 0.0f) {
		v[0] = 1.0;
		v[1] = 0.0;
		return;
	}
	len = 1.0f / sqrt(len);
	v[0] *= len;
	v[1] *= len;
}

int check_keys(XEvent *e)
{
	if (e->type != KeyPress && e->type != KeyRelease)
		return 0;
	int key = XLookupKeysym(&e->xkey, 0);
	if (e->type == KeyPress) {
		switch (key) {
			case XK_a:
				//moveLeft();
				gl.keys[XK_a] = 1;
				break;
			case XK_d:
				//moveRight();
				gl.keys[XK_d] = 1;
				break;
			case XK_w:
				gl.keys[XK_w] = 1;
				break;
			case XK_s:
				gl.keys[XK_s] = 1;
				break;
			case XK_space:
				gl.keys[XK_space] = 1;
				break;
			case XK_c : 
				gl.creditPage ^= 1;
				break;
			case XK_Escape:
				return 1;
			case XK_l:
				g.ship.weaponType ^= 1;
				break;
		}
	}

	if (e->type == KeyRelease) {
		switch (key) {
			case XK_a:
				//s->vel[0] = 10.0;
				gl.keys[XK_a] = 0;
				break;
			case XK_d:
				//s->vel[0] = 10.0;
				gl.keys[XK_d] = 0;
				break;
			case XK_w:
				gl.keys[XK_w] = 0;
				break;
			case XK_s:
				gl.keys[XK_s] = 0;
				break;
			case XK_space:
				gl.keys[XK_space] = 0;
				break;
		}
	}
	return 0;
}

void physics()
{
	//float spdLeft, spdRight, spdUp, spdDown;
	Ship *s = &g.ship;
	if (s->pos[0] < 20.0) {
		s->pos[0] = 20.0;
	} else if (s->pos[0] > gl.xres - 20.0) {
		s->pos[0] = gl.xres - 20;
	} else if (s->pos[1] < 20.0) {
		s->pos[1] = 20.0;
	} else if (s->pos[1] > gl.yres - 20.0) {
		s->pos[1] = gl.yres - 20.0;
	}

	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	int i = 0;
	while (i < g.nbullets) {
		Bullet *b = &g.barr[i];
		if (b->pos[1] > gl.yres + 10) {
			memcpy(&g.barr[i], &g.barr[g.nbullets - 1], sizeof(Bullet));
			g.nbullets--;
			continue;
		}
		b->pos[0] += b->vel[0];
		b->pos[1] += b->vel[1];
		i++;
	}

	if (gl.keys[XK_a]) {
		s->pos[0] -= s->vel[0];
		s->vel[0] += s->speed;
		if (s->vel[0] > MAX_VELOCITY)
			s->vel[0] = MAX_VELOCITY;
	} else {
		if(s->vel[0] > 0.0) {
			s->pos[0] -= s->vel[0];
			s->vel[0] -= s->speed;
		} else {
			s->vel[0] = 0.0;
		}
	}


	if (gl.keys[XK_d]) {
		s->pos[0] += s->vel[1];
		s->vel[1] += s->speed;
		if (s->vel[1] > MAX_VELOCITY)
			s->vel[1] = MAX_VELOCITY;
	} else {
		if(s->vel[1] > 0.0) {
			s->pos[0] += s->vel[1];
			s->vel[1] -= s->speed;
		} else {
			s->vel[1] = 0.0;
		}
	}

	if (gl.keys[XK_w]) {
		s->pos[1] += s->vel[2];
		s->vel[2] += s->speed;
		if (s->vel[2] > MAX_VELOCITY)
			s->vel[2] = MAX_VELOCITY;
	} else {
		if(s->vel[2] > 0.0) {
			s->pos[1] += s->vel[2];
			s->vel[2] -= s->speed;
		} else {
			s->vel[2] = 0.0;
		}
	}

	if (gl.keys[XK_s]) {
		s->pos[1] -= s->vel[3];
		s->vel[3] += s->speed;
		if (s->vel[3] > MAX_VELOCITY)
			s->vel[3] = MAX_VELOCITY;
	} else {
		if(s->vel[3] > 0.0) {
			s->pos[1] -= s->vel[3];
			s->vel[3] -= s->speed;
		} else {
			s->vel[3] = 0.0;
		}
	}

	if (gl.keys[XK_space]) {
		struct timespec bt;
		clock_gettime(CLOCK_REALTIME, &bt);
		double ts = timeDiff(&g.bulletTimer, &bt);
		if (s->weaponType == 0) {
			wpn->fire();
		} else if (s->weaponType == 1) {
			if (ts > 0.3) {
				timeCopy(&g.bulletTimer, &bt);
				if (g.nbullets < MAX_BULLETS) {
					for (int i = 0; i < 3; i++) {
						Bullet *b = &g.barr[g.nbullets];
						timeCopy(&b->time, &bt);
						b->pos[0] = s->pos[0];
						b->pos[1] = s->pos[1] + 30;
						if (i == 0) {
							b->vel[0] = -15.0;
							b->vel[1] = 30.0;
						} else if (i == 1) {
							b->vel[0] = 0.0;
							b->vel[1] = 30.0;
						} else if (i == 2) {
							b->vel[0] = 15.0;
							b->vel[1] = 30.0;
						}
						b->color[0] = 0.5;
						b->color[1] = 1.0;
						b->color[2] = 1.0;
						g.nbullets++;

					}
				}
			}
		}
	}
	if (g.thrustOn) {
		struct timespec mtt;
		clock_gettime(CLOCK_REALTIME, &mtt);
		double tdif = timeDiff(&mtt, &g.thrustTimer);
		if (tdif < -0.3)
			g.thrustOn = false;
	}
	return;
}

void render()
{
	//If 'c' was pressed then render credit screen
	if(gl.creditPage) { 

		int w = img[0].width;
		int h = img[0].height;

		glBindTexture(GL_TEXTURE_2D, gl.nickImage);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img[0].data);

		w = img[1].width;
		h = img[1].height;
		glBindTexture(GL_TEXTURE_2D, gl.andrewImage);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
				img[1].data);

		w = img[2].width;
		h = img[2].height;
		glBindTexture(GL_TEXTURE_2D, gl.spencerImage);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img[2].data);

		w = img[3].width;
		h = img[3].height;
		glBindTexture(GL_TEXTURE_2D, gl.chadImage);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img[3].data);
		initialize_fonts();

		w = img[4].width;
		h = img[4].height;
		glBindTexture(GL_TEXTURE_2D, gl.benImg);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img[4].data);


		glClear(GL_COLOR_BUFFER_BIT);


		displaySpencer( 700  , 250, gl.spencerImage); 
		displayNick(gl.xres/2, gl.yres/2, gl.nickImage);
		displayAndrew(gl.xres/4, gl.yres/4, gl.andrewImage);
		displayChad( 700, gl.yres/2 + 250, gl.chadImage); 
		BenjaminG(gl.xres/4, 3*gl.yres/4, gl.benImg);
	} else {

		glClear(GL_COLOR_BUFFER_BIT);
		//Draw player ship
		glColor3fv(g.ship.color);
		glPushMatrix();
		glTranslatef(g.ship.pos[0], g.ship.pos[1], g.ship.pos[2]);
		glBegin(GL_QUADS);
		glVertex2f(-20.0f, -20.0f);
		glVertex2f(-20.0f, 20.0f);
		glVertex2f(20.0f, 20.0f);
		glVertex2f(20.0, -20.0);
		glEnd();
		glPopMatrix();

		for (int i = 0; i < g.nbullets; i++) {
			Bullet *b = &g.barr[i];
			glColor3fv(b->color);
			glPushMatrix();
			glTranslatef(b->pos[0], b->pos[1], b->pos[2]);
			glBegin(GL_QUADS);
			glVertex2f(-5.0, -5.0);
			glVertex2f(-5.0, 5.0);
			glVertex2f(5.0, 5.0);
			glVertex2f(5.0, -5.0);
			glEnd();
			glPopMatrix();
		}
	}
}

// Utility Functions

/**
 * Gets the time differential between the last time an event occured and now
 * @param timespec *bt 	Pointer to a timespec struct
 * @return double ts 	Time slice representing the elapsed time
 */
double getTimeSlice(timespec *bt)
{
    clock_gettime(CLOCK_REALTIME, bt);
    double ts = timeDiff(&g.bulletTimer, bt);
    return ts;
}

/**
 * Converts degrees to radians
 * @param float angle 		The angle to be converted to radians
 * @return float radian 	Value representing the inputed angle
 */
float convertToRads(float angle)
{
    float rads = angle * (PI / 180);
    return rads;
}