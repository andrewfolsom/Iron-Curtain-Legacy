#include <GL/glx.h>
#include <X11/Xlib.h>
#include "core.h"
#include "chadM.h"
#include "fonts.h"

extern Global& gl;

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

EnemyShip::EnemyShip()
{
	int health = 100;
	pos[0] = pos[1] = 500;
	pos[2] = 0;
	color[0] = color[1] = color[2] = 0.35;
	numberOfEnemies = 0;
}

int EnemyShip::armadaSize() 
{
	return numberOfEnemies;
}

void renderShip(Ship ship)
{
	glColor3fv(ship.color);
	glPushMatrix();
	glTranslatef(ship.pos[0], ship.pos[1], ship.pos[2]);
	glBegin(GL_QUADS);
	glVertex2f(-20.0f, -20.0f);
	glVertex2f(-20.0f, 20.0f);
	glVertex2f(20.0f, 20.0f);
	glVertex2f(20.0, -20.0);
	glEnd();
	glPopMatrix();
}
