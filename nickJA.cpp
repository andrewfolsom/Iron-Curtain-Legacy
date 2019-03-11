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
#include <math.h>
#include "chadM.h"
extern Game g;
extern Global gl;
//const int RUSH = 0;
//const int STRAFE = 1;
//const int CIRCLING = 2;
//const int BANK = 3;

enum MOVETYPE { RUSH, STRAFE, CIRCLING, BANK, DIAG_RUSH};

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
	newGuy->pos[0]= x;
	newGuy->pos[1] = y;
	newGuy->spawnPos[0]= x;
	newGuy->spawnPos[1] = y;
	newGuy->movPattern = movType;
	g.numOpFor++;

	printf( "OpFor %i created at (%.2f, %.2f) with movePattern = %i\n",
			g.numOpFor, newGuy->pos[0], newGuy->pos[1], newGuy->movPattern);
}

//RENDER OPFOR
void renderOpFor() 
{

	opForShip *draw;

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

void configOpFor(int ID, int destOffset) 
{
	opForShip *Edit = &g.opFor[ID];

	Edit->destOffset = destOffset;

}

//MOVEMENT TYPES
//Press 't' to test this mid game.
//1 - Rush
//Enemy begins at the top of the screen and rushes straight to the bottom.
//Iteration will be used to identify which entity to update the position of.
void updateRush(int iteration)
{
	opForShip *move = &g.opFor[iteration];
	move->pos[1] -= move->speedMul;
}
//EnemyShip compatible.
void EnemyShip::updateRush()
{
	if (moveFlag)
		pos[1] -= speedMul;
}

//2 - Strafing
//Enemy Oscillates between two points while moving to the bottom of the screen.
//Iteration will be used to identify which entity to update the position of.
void updateStrafe(int iteration)
{
	opForShip *move = &g.opFor[iteration];
	move->pos[1] -= move->speedMul;

	move->pos[0] += 10*cos(move->angle);
	move->angle+= 0.1;
}
//EnemyShip compatible..
void EnemyShip::updateStrafe()
{
	if (moveFlag)
		pos[1] -= speedMul;

	pos[0] += 30*cos(angle);
	angle+= 0.1;
}

//3 - Circling
//Enemy follows the path of a circle whose center is consistently moving
//towards the bottom of the screen.
//Iteration will be used to identify which entity to update the position of.
void updateCircle(int iteration) 
{
	opForShip *move = &g.opFor[iteration];

	move->pos[0] += 10*cos(move->angle);
	move->pos[1] += 10*sin(move->angle) - move->speedMul;

	move->angle+= 0.1;
}
//EnemyShip compatible
void EnemyShip::updateCircle() 
{
	pos[0] += 10*cos(angle);

	if (moveFlag)
		pos[1] += 10*sin(angle) - speedMul;
	else
		pos[1] += 10*sin(angle);

	angle+= 0.1;
}

//4 - Bank
//Enemy follows a curve while moving from the top to bottom of the screen.
//Iteration will be used to identify which entity to update the position of.
void updateBank(int iteration)
{
	opForShip *move = &g.opFor[iteration];

	//printf("Opfor %i spawned at (%f, %f)\n", g.numOpFor, move->spawnPos[0], move->spawnPos[1]);
	//printf("Opfor %i Before update at (%f, %f)\n", g.numOpFor, move->pos[0], move->pos[1]);

	float midPoint[2] = {0.0, 0.0};
	
	if (move->destOffset < 450)
		midPoint[0] = 900.0;

	midPoint[1] = move->spawnPos[1];

	move->pos[0] = (pow(1-move->t, 2.0) * move->spawnPos[0]) + (2*(1-move->t)*move->t*midPoint[0]) + 
					(pow(move->t, 2.0) * move->destOffset);
	move->pos[1] = (pow(1-move->t, 2.0) * move->spawnPos[1]) + (2*(1-move->t)*move->t*midPoint[1]) + 
					(pow(move->t, 2.0) * (0));
	move->t += 0.006;

	if (move->t > 1.0)
		move->t = 0;

	//printf("Opfor %i at (%f, %f)\n", g.numOpFor, move->pos[0], move->pos[1]);
}
//EnemyShip compatible
void EnemyShip::updateBank()
{
	if (initFlag == 0) {
		spawnPos[0] = pos[0];
		spawnPos[1] = pos[1];
		initFlag = 1;
	}

	float midPoint[2] = {0.0, 0.0};

	if (destOffset < 450)
		midPoint[0] = 900.0;

	midPoint[1] = spawnPos[1];

	pos[0] = (pow(1-t, 2.0) * spawnPos[0]) + (2*(1-t)*t*midPoint[0]) + 
					(pow(t, 2.0) * destOffset);
	pos[1] = (pow(1-t, 2.0) * spawnPos[1]) + (2*(1-t)*t*midPoint[1]) + 
					(pow(t, 2.0) * (0));
	t += 0.006;

	if (t > 1.0)
		t = 0;

}

//5 - Diagonal Rush
//Enemy will follow a diagonal line from its spawn to the bottom of the screen.
void updateDiagRush(int iteration)
{
	opForShip *move = &g.opFor[iteration];

	move->pos[1] -= move->speedMul;
	move->pos[0] = (move->pos[1]-1000)/-1.13;
}
//EnemyShip Compatible
void EnemyShip::updateDiagRush()
{
	pos[1] -= speedMul;
	pos[0]  = (pos[1]-1000)/-1.13;
}

//Movement Function
//Function uses a switch case to determine what function should be applied to
//each entity in the OpFor array.
void updatePosition()
{

	for (int i = 0; i <= g.numOpFor; i++) {
		opForShip *target = &g.opFor[i];
		
		switch (target->movPattern) {

			case (RUSH):
				updateRush(i);
				break;

			case (STRAFE):
				updateStrafe(i);
				break;

			case (CIRCLING):
				updateCircle(i);
				break;

			case (BANK):
				updateBank(i);
				break;

			case (DIAG_RUSH):
				updateDiagRush(i);
				break;
		}
	}

}

//EnemyShip Compatible
void EnemyShip::updatePosition()
{

	for (int i = 0; i <= g.numOpFor; i++) {
		//opForShip *target = &g.opFor[i];
		
		switch (movPattern) {

			case (RUSH):
				updateRush();
				break;

			case (STRAFE):
				updateStrafe();
				break;

			case (CIRCLING):
				updateCircle();
				break;

			case (BANK):
				updateBank();
				break;

			case (DIAG_RUSH):
				updateDiagRush();
				break;
		}
	}

}
