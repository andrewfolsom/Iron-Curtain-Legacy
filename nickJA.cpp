//Author: Nick Jackson
//Created: Feb 13 2019
//
//Contains:
//Personal Display Function
//MOVEMENT
//

#include <GL/glx.h>
#include "fonts.h"
#include "core.h"
#include <stdio.h>

extern Game g;

//DISPLAY
void displayNick(float x, float y, GLuint texture)
{
	float wid = 100.0f;
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glColor3ub(255, 255, 255);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
	glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
	glEnd();
	glPopMatrix();

	unsigned int color = 0x003366ff;
	Rect r;
	r.centerx = x;
	r.centery = y + 120;
	r.height = 100;
	r.width = 500;
	r.bot = r.centery - 15;
	r.left = r.centerx - 50;
	ggprint16(&r, 16, color, "Nick Jackson");
}
//SPAWN
//X & Y are spawn Coordinates
//movType acts as a flag to define movement behavior.
void spawnOpFor(int x, int y, int movType) 
{
	opForShip *newGuy = &g.opFor[g.numOpFor];
	newGuy->pos[0] = x;
	newGuy->pos[1] = y;
	newGuy->movPattern = movType;
	g.numOpFor++;

	printf("OpFor %i created at (%.2f, %.2f) with movePattern = %i\n",g.numOpFor, newGuy->pos[0], newGuy->pos[1], newGuy->movPattern);
}

//RENDEROPFOR
void renderOpFor() 
{

	opForShip *draw;;

	for (int i = 0; i < g.numOpFor; i++) {
		draw = &g.opFor[i];
		glColor3fv(draw->color);
		glPushMatrix();
		glTranslatef(draw->pos[0], draw->pos[1], draw->pos[2]);
		glBegin(GL_QUADS);
		glVertex2f(-20.0, -20.0);
		glVertex2f(-20.0,  20.0);
		glVertex2f( 20.0,  20.0);
		glVertex2f( 20.0, -20.0);
		glEnd();
		glPopMatrix();
		}
}

//MOVEMENT TYPES

//1 - Rush
//Enemy begins at the top of the screen and rushes straight to the bottom.
void upDateRush() 
{

}

//2 - Strafing
//Enemy Oscillates between to points while moving to the bottom of the screen.
void upDateStrafe()
{

}

//3 - Circling
//Enemy follows the path of a circle whose center is consistently moving
//towards the bottom of the screen.
void upDateCircle() 
{

}

//4 - Bank
//Enemy follows a curve while moving from the top to bottom of the screen.
void upDateBank()
{

}
