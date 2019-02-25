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

//Class Definitions

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
void spawnOpFor(int x, int y, int movType, opForShip opFor)
{
	opFor.pos[0] = x;
	opFor.pos[1] = y;
	opFor.movPattern = movType;

	printf("OpFor created at (%.2f, %.2f) with movePattern = %i\n", opFor.pos[0], opFor.pos[1], opFor.movPattern);
}
//RENDEROPFOR

void renderOpFor(Game g) {
	if (g.numOpFor > 0) 
	{
		for (int i = 0; i < g.numOpFor; i++) {
			glColor3fv(g.opFor[i].color);
			glPushMatrix();
			glTranslatef(g.opFor[i].pos[0], g.opFor[i].pos[1], g.opFor[i].pos[2]);
			glBegin(GL_QUADS);
			glVertex2f(-1.0, -1.0);
			glVertex2f(-1.0,  1.0);
			glVertex2f( 1.0,  1.0);
			glVertex2f( 1.0, -1.0);
			glEnd();
			glPopMatrix();
		}
	}
}
