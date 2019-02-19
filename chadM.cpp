#include <X11/Xlib.h>
#include <GL/glx.h>
#include "fonts.h"

void displayChad (float x , float y, GLuint textid) {
    float wid = 100.0;

    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glBindTexture(GL_TEXTURE_2D, textid);
    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(wid, wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(wid, -wid);
    glEnd();
    glPopMatrix();

    unsigned int color = 0xffff0000;
    Rect r;
    r.width = 250.0;
    r.height = 100.0;
    r.center = x;
    r.left = x - 10;
    r.bot = y - 125;

    ggprint16(&r, 16, color, "Chad Manning");
}
