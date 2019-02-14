#include <iostream>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include <string.h>


using namespace std;

void showSpencerPicture(int x, int y, GLuint textid) {
    
    static int wid = 30; 
    
    glColor3ub(255,255,0);
    glPushMatrix();
    glTranslatef(x,y,0.0);
    glBindTexture(GL_TEXTURE_2D, textid);
    glBegin (GL_QUADS);
    	glTexCoord2f(0.0f,1.0f); glVertex2i(-wid,-wid);
    	glTexCoord2f(0.0f,0.0f); glVertex2i(-wid,wid);
    	glTexCoord2f(1.0f,0.0f); glVertex2i(wid,wid);
    	glTexCoord2f(1.0f,1.0f); glVertex2i(wid,-wid);
	
    glEnd();
    glPopMatrix();
}
