#include <GL/glx.h>
#include "fonts.h"

void displayInfo(GLuint andrewTexture)
{
    float res = 100.0f;
    glPushMatrix();
    glTranslatef(400.0, 500.0, 0.0);
    glColor3ub(255, 255, 255);
    glBindTexture(GL_TEXTURE_2D, andrewTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(-res,-res);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(-res, res);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(res, res);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(res, -res);
    glEnd();
    glPopMatrix();
}