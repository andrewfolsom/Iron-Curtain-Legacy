#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include <string.h>


void displayNick(float x, float y, GLuint texture)
{
	float wid = 500.0f;
	glPushMatrix();	
	glTranslatef(x, y, 0.0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
 	glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
 	glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
 	glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
	glEnd();
	glPopMatrix();

}
