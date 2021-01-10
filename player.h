#pragma once
#include "gameObject.h"
#include "config.h"

class Player : public GameObject
{
	//main variables
	float speed = PLAYER_SPEED;
	float posX;
	float posY;
	float playerHeight;

	//special variable
	bool lightUp = false;
	bool longMode = false;

	//for players initial settings
	bool leftPosition;
	bool leftControl;

public:
	//constractor
	//game,posX,posY,leftPosition,leftControl
	Player(const class Game & mygame, float x, float y, bool b, bool b2);

	//overrided methods
	void update() override;
	void draw() override;
	void init() override;

	//getters
	float getPosY() { return posY; }
	float getPosX() { return posX; }
	float getSpeed() { return speed; }
	bool getLightUp() { return lightUp; }

	//setters
	void setPosY(float pos) { posY = pos; }
	void setPosX(float pos) { posX = pos; }
	void setSpeed(float s) { speed = s; }
	void setLightUp(bool b) { lightUp = b; }
	void setLongMode(bool b) { longMode = b; }
};

