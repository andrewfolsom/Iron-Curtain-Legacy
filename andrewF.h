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
	~Weapon();
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

// class Scatter : public Weapon
// {
// public:
// 	int shotsFired;
// 	float spread;
// public:
// 	Scatter();
// 	void setVelocity(float*);
// 	void fire();
// };

#endif