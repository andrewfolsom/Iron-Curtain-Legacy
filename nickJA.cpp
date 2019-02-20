//Author: Nick Jackson
//Created: Feb 13 2019
//
//Contains:
//Personal Display Function
//MOVEMNT
//

#include <GL/glx.h>
#include "fonts.h"
#include "core.h"

//Class Definitions

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

void spawnOpFor(int x, int y, int movType, Game g)
{
	opForShip newGuy;
	newGuy.pos[0] = x;
	newGuy.pos[1] = y;
	newGuy.movPattern = movType;
	g.numOpFor++;
}
