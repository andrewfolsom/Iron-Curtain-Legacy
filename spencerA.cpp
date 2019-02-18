
#include <GL/glx.h>
#include "fonts.h"

void displaySpencer(float x, float y, GLuint texture)
{
    float wid = 100.0f;
    unsigned int color = 0x00bfb418;
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glColor3ub(255, 255, 255);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(wid, wid);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(wid, -wid);
    glEnd();
    glPopMatrix();
    
    Rect r;
    r.width = 250.0;
    r.height = 100.0;
    r.center = 0;
    r.left = x-65;
    r.bot = y- 125;

    ggprint16(&r, 16, color, "Spencer Austin");
}

