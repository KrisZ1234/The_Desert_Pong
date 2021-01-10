#pragma once
#include "player.h"
#include "ball.h"

class Game
{
	//game statuses
	typedef enum { STATUS_MENU, STATUS_INSTRUCTION, STATUS_PLAY, STATUS_END } status_t;
	status_t status = STATUS_MENU;

	//game objects
	Player * playerRight = nullptr;
	Player * playerLeft = nullptr;
	Ball * ball = nullptr;
	Ball * ball2 = nullptr;

	//boolean variables
	bool playerRight_init = false;
	bool playerLeft_init = false;
	bool debug_mode = false;

	//collisions
	void collision();
	void collisionDeathMode(Ball *ball);

	//instance statuses
	typedef enum { PLAY_MODE, DEATH_MODE } mode_t;
	mode_t mode = PLAY_MODE;

	//update
	void updateInstructionScreen();
	void updateStartScreen();
	void updateLevelScreen();
	void updateEndScreen();

	//draw
	void drawInstructionScreen();
	void drawStartScreen();
	void drawLevelScreen();
	void drawEndScreen();

	//scores
	float score1 = 0;
	float score2 = 0;

	//deathMode var
	void checkDeathMode();
	bool deathMode = false;
	float deathModeInitTime;

	//extra
	void resetBall();
	void resetCollisionEffect();

public:
	//original draw,update,init  
	void update();
	void draw();
	void init();

	//for debugging purposes
	bool getDebugMode() const { return debug_mode; }
	void setDebugMode(bool d) { debug_mode = d; }

	//score getters/setters
	float getScore1() { return score1; }
	float getScore2() { return score2; }
	bool getDeathMode() { return deathMode; }

	//creation&destruction
	Game();
	~Game();
};

