/* Source:	BenjaminG.cpp
 * @Author:	Benjamin A Garza III
 * Course:	CMPS 3350: Software Engineering
 * Assignment:	Lab 4 - Group Project Setup
 * Date:	February 14, 2019
 * Purpose:	Provide a function that will be called from the main
 * 		function of our game to display my name and a picture.
 */

#include <iostream>
#include <unistd.h>
#include <GL/glx.h>
#include "fonts.h"

using namespace std;

void BenjaminG(float x, float y, GLuint texture) {
    Rect r;
    //glClear(GL_COLOR_BUFFER_BIT);
    r.width = 250.0;
    r.height = 100.0;
    r.center = x;
    r.left = x - 10;
    r.bot = y - 125;
    
    float wid = 100.0f;
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(wid, wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(wid, -wid);
    glEnd();
    glPopMatrix();

    ggprint8b(&r, 16, 0x00ffffff, "Benjamin A Garza");
}

