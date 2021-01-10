#pragma once
#include "graphics.h"
#include "gameObject.h"
#include "config.h"

class Ball : public GameObject
{
	//main variables
	float posX, posY;
	float speedX;
	float speedY;
	float radius;

	//bool variables
	bool active = true;
	bool skullMode = false;

	//
	void wallCollisionY();

public:
	//constractor
	//game,speedX,speedY,radius,skullMode
	Ball(const class Game& mygame, float sx, float sy, float r, bool s);

	//overriden methods
	void update() override;
	void draw() override;
	void init() override;

	//getters & setters 
	float getPosY() { return posY; }
	float getPosX() { return posX; }
	float getSpeedX() { return speedX; }
	float getSpeedY() { return speedY; }
	bool getSkullMode() { return skullMode; }
	void setPosY(float y) { posY = y; }
	void setPosX(float x) { posX = x; }
	void setSpeedX(float x) { speedX = x; }
	void setSpeedY(float y) { speedY = y; }
	void setSkullMode(bool b) { skullMode = b; }

	~Ball();
};
