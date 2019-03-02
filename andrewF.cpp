#include <GL/glx.h>
#include <stdio.h>
#include "andrewF.h"
#include <cmath>
#include "fonts.h"
#include "andrewF.h"
#include "core.h"

#define VecDot(a,b) ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0];(c)[1]=(a)[1]-(b)[1];\
(c)[2]=(a)[2]-(b)[2]

const int MAX_BULLETS = 1000;
const int MAX_MISSILES = 1;
extern float convertToRads(float angle);
extern double getTimeSlice(timespec*);
extern void timeCopy(struct timespec *dest, struct timespec *source);
extern Game g;

/**
 * Displays my picture and name
 * @param float x           Desired x position of image
 * @param float y           Desired y position of image
 * @param GLuint texture    Texture mapped image of my face
 */
void displayAndrew(float x, float y, GLuint texture)
{
    float res = 100.0f;
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glColor3ub(255, 255, 255);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(-res,-res);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(-res, res);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(res, res);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(res, -res);
    glEnd();
    glPopMatrix();

    unsigned int color = 0x00bfb418;
    Rect r;
    r.width = 250.0;
    r.height = 100.0;
    r.center = x;
    r.left = x;
    r.bot = y - 125;

    ggprint16(&r, 16, color, "Andrew Folsom");
}

//===========================================================
//                GENERAL UTILITY FUNCTIONS
//===========================================================

/**
 * Adjusts an objects velocity according to the angle at which
 * it should travel.
 * @param vel   Array containing the objects X/Y velocities
 * @param angle The desired angle of fire
 */
void angularAdjustment(float *vel, float angle)
{
    vel[0] = vel[1] * cos(convertToRads(angle));
    vel[1] = vel[1] * sin(convertToRads(angle));
}

/**
 * Updates objects position so that it continually moves towards
 * the target.
 * @param m      Missile object to be moved
 * @param target Array containing targets X/Y coordinates
 * @param t      Time value for the Bezier curve
 */
void tracking(Missile *m, float *target, float t)
{
    float mid[3] = {0.0, 0.0, 0.0};
    if (m->start[0] < 500.0)
        mid[1] = m->start[1];

    if (m->start[0] > 500.0) {
        mid[0] = 900.0;
        mid[1] = m->start[1];
    }

    m->pos[0] = (pow(1 - t, 2.0) * m->start[0]) + (2 * (1 - t) * t * mid[0]
        ) + (pow(t, 2.0) * target[0]);
    m->pos[1] = (pow(1 - t, 2.0) * m->start[1]) + (2 * (1 - t) * t * mid[1]
        ) + (pow(t, 2.0) * target[1]);
}

//===========================================================
//    DEFINITION OF WEAPON CLASS AND IT'S DERIVED CLASSES
//===========================================================

/**
 * Weapon class constructor
 */
Weapon::Weapon() { }

/**
 * Weapon class destructor
 */
Weapon::~Weapon() { }

/**
 * Sets the initial position of generated Bullet objects.
 * @param float *s  position array of firing ship
 * @param float *b  position array of fired bullet
 */
void Weapon::setPosition(float *s, float *b)
{
    b[0] = s[0];
    b[1] = s[1];
    b[2] = s[2];
}

/**
 * Sets the velocity of the generated Bullet object. This implementation does
 * not account for bullet spread.
 * @param float *vel    velocity array of fired bullet
 */
void Weapon::setVelocity(float *vel)
{
    vel[0] = 0.0;
    vel[1] = bulletSpeed;
}

/**
 * Sets the color of the generated Bullet object.
 * @param float *c  color array of the fired bullet
 */
void Weapon::setColor(float *c)
{
    c[0] = color[0];
    c[1] = color[1];
    c[2] = color[2];
}

/**
 * Basic weapon class constructor
 */
Basic::Basic()
{
    fireRate = 0.5;
    bulletSpeed = 15.0;
    color[0] = 1.0;
    color[1] = 0.0;
    color[2] = 0.0;
}

/**
 * Implementation of the fire function for the Basic weapon
 */
void Basic::fire()
{
    struct timespec bt;
    if (getTimeSlice(&bt) > fireRate) {
        timeCopy(&g.bulletTimer, &bt);
        if (g.nbullets < MAX_BULLETS) {
            Bullet *b = &g.barr[g.nbullets];
            timeCopy(&b->time, &bt);
            setPosition(g.ship.pos, b->pos);
            setVelocity(b->vel);
            setColor(b->color);
            g.nbullets++;
        }
    }
}

/**
 * Rapid weapon class constructor
 */
Rapid::Rapid()
{
    fireRate = 0.25;
}

/**
 * Scatter weapon class constructor
 */
Scatter::Scatter()
{
    fireRate = 0.5;
    bulletSpeed = 15;
    color[0] = 0.5;
    color[1] = 1.0;
    color[2] = 0.5;
    shotsFired = 4;
    spread = 30.0;
    start = 90 - (spread / 2);
    increment = spread / (shotsFired - 1);
    temp = 0.0;
}

/**
 * Generate bullets that will move within a specified spread
 * @param int shots     Number of bullets within the spread
 * @param float spread  The degree of spread desired
 * @param float *bull   Array of bullet objects to be adjusted
 */
void Scatter::bulletSpread(float *vel)
{
    vel[0] = vel[1] * cos(convertToRads(temp));
    vel[1] = vel[1] * sin(convertToRads(temp));
}

/**
 * Initializes multiple instances of Bullet objects for
 * a burst like fire mode.
 */
void Scatter::fire()
{
    temp = start;
    struct timespec bt;
    if (getTimeSlice(&bt) > fireRate) {
        timeCopy(&g.bulletTimer, &bt);
        for (int i = 0; i < shotsFired; i++) {
            if(g.nbullets < MAX_BULLETS) {
                Bullet *b = &g.barr[g.nbullets];
                timeCopy(&b->time, &bt);
                setPosition(g.ship.pos, b->pos);
                setVelocity(b->vel);
                bulletSpread(b->vel);
                setColor(b->color);
                temp += increment;
                g.nbullets++;
            }
        }
    }
}

/**
 * Ring weapon class constructor
 */
Ring::Ring()
{
    fireRate = 5.0;
    bulletSpeed = 2.0;
    color[0] = 0.5;
    color[1] = 1.0;
    color[2] = 0.5;
    shotsFired = 45;
    spread = 360.0;
    start = 90 - (spread / 2);
    increment = spread / (shotsFired - 1);
    temp = 0.0;
};

/**
 * Pinwheel weapon class constructor
 */
Pinwheel::Pinwheel()
{
    fireRate = 0.6;
    bulletSpeed = 1.0;
    color[0] = 0.5;
    color[1] = 1.0;
    color[2] = 0.5;
    shotsFired = 4;
    spread = 0.0;
    increment = 2.0;
}

/**
 * Generates multiple Bullet objects while constantly
 * incrementing their angle of fire to create a pinwheel
 * like effect.
 */
void Pinwheel::fire()
{
    struct timespec bt;
    if (getTimeSlice(&bt) > fireRate) {
        timeCopy(&g.bulletTimer, &bt);
        for (int i = 0; i < shotsFired; i++) {
            if(g.nbullets < MAX_BULLETS) {
                Bullet *b = &g.barr[g.nbullets];
                timeCopy(&b->time, &bt);
                setPosition(g.ship.pos, b->pos);
                b->vel[0] = b->vel[1] = bulletSpeed;
                b->vel[0] *= cos(convertToRads(spread));
                b->vel[1] *= sin(convertToRads(spread));
                setColor(b->color);
                spread += 90.0;
                g.nbullets++;
            }
        }
    }
    spread = 0.0 + increment;
    increment += 2.0;
    if (increment > 90)
        increment = 0.0;
}

/**
 * Secondary weapon class constructor
 */
Secondary::Secondary()
{
    fireRate = 1.0;
    bulletSpeed = 5.0;
    color[0] = 1.5;
    color[1] = 0.5;
    color[2] = 1.0;
}

/**
 * Secondary weapon specific implementation of setVelocity.
 * Sets both X and Y velocity to bulletSpeed so that they
 * can be adjusted with trig functions.
 * @param vel Array containing the objects X/Y velocity
 */
void Secondary::setVelocity(float *vel)
{
    vel[0] = bulletSpeed;
    vel[1] = bulletSpeed;
}

/**
 * Generates a Missile object. Special implementation required
 * to account for tracking starting and ending position.
 */
void Secondary::fire()
{
    struct timespec bt;
    if (getTimeSlice(&bt) > fireRate) {
        timeCopy(&g.missileTimer, &bt);
        if (g.nmissiles < MAX_MISSILES) {
            Missile *m = &g.marr[g.nmissiles];
            timeCopy(&m->time, &bt);
            setPosition(g.ship.pos, m->pos);
            setPosition(g.ship.pos, m->start);
            setColor(m->color);
            g.nmissiles++;
        }
    }
}

/**
 * Missile object constructor
 */
Missile::Missile() { }