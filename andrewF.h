// Iron Curtain Weapon System - Declarations
// Author: Andrew Folsom
// Date: 02/20/2019

#ifndef WEAPONS_H
#define WEAPONS_H

// Player Weapon Declarations

class Weapon
{
public:
	float fireRate;
	float bulletSpeed;
	float color[3];
public:
	Weapon();
	virtual ~Weapon();
	void setPosition(float*, float*);
	virtual void setVelocity(float*);
	void setColor(float*);
	virtual void fire() =0;
};

class Basic : public Weapon
{
public:
	Basic();
	void fire();
};

class Rapid : public Basic
{
public:
	Rapid();
};

class Scatter : public Weapon
{
public:
	int shotsFired;
	float spread;
	float start;
	float increment;
	float temp;
public:
	Scatter();
	void bulletSpread(float*);
	void fire();
};

class Ring : public Scatter
{
public:
	Ring();
};

class Secondary : public Basic
{
public:
	Secondary();
	void setVelocity(float*);
	void fire();
};

class Missile
{
public:
	float start[3];
	float pos[3];
	float color[3];
	timespec time;
public:
	Missile();
};

#endif