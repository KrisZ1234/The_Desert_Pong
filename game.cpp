#include "game.h"
#include "graphics.h"
#include "config.h"

#include<iostream>
using namespace std;

void Game::collision()
{
	//checking if ball collides with LEFT wall
	if (ball->getPosX() <= PLAYER_WIDTH + BALL_RADIUS) {
		//collision with playerLeft
		if (ball->getPosY() <= (playerLeft->getPosY() + PLAYER_HEIGHT / 2) &&
			ball->getPosY() >= (playerLeft->getPosY() - PLAYER_HEIGHT / 2)) {
			//collision sound
			graphics::playSound(std::string(ASSET_PATH) + "bump2.mp3", 0.5f, false);
			//set collision effect mode
			playerLeft->setLightUp(true);
			//set the ball in opposite direction and boost its speed 
			ball->setSpeedX(-(ball->getSpeedX()) * SPEED_BOOST);
			//boost players speed
			playerLeft->setSpeed(playerLeft->getSpeed() * PLAYER_SPEED_BOOST);
		}
		//collision with wall
		else {
			//point for player 2
			score2 += 1;
			//lost point sound effect
			graphics::playSound(std::string(ASSET_PATH) + "lost_point.mp3", 0.6f, false);
			//restart ball with random direction
			resetBall();
		}
	}

	//checking if ball collides with RIGHT wall
	if (ball->getPosX() >= CANVAS_WIDTH - PLAYER_WIDTH - BALL_RADIUS) {
		//collision with player2
		if (ball->getPosY() <= (playerRight->getPosY() + PLAYER_HEIGHT / 2) &&
			ball->getPosY() >= (playerRight->getPosY() - PLAYER_HEIGHT / 2)) {
			//collision sound
			graphics::playSound(std::string(ASSET_PATH) + "bump2.mp3", 0.6f, false);
			//make collision effect with the left board
			playerRight->setLightUp(true);
			//set the ball in opposite direction and boost its speed 
			ball->setSpeedX(-(ball->getSpeedX()) * SPEED_BOOST);
			playerRight->setSpeed(playerRight->getSpeed() * PLAYER_SPEED_BOOST);
		}
		//collision with wall
		else {
			//point for player 1
			score1 += 1;
			//bump sound effect
			graphics::playSound(std::string(ASSET_PATH) + "lost_point.mp3", 0.6f, false);
			//restart ball with random direction
			resetBall();
		}
	}
}

void Game::collisionDeathMode(Ball *ball)
{
	//checking if ball collides with Left wall
	if (ball->getPosX() <= PLAYER_WIDTH + SCULL_RADIUS) {
		//collision with player1
		if (ball->getPosY() <= (playerLeft->getPosY() + LONG_HEIGHT / 2 + 8) && 
			ball->getPosY() >= (playerLeft->getPosY() - LONG_HEIGHT / 2 + 8)) {
			//point for player 2
			score2 += 1;
			//bump sound effect
			graphics::playSound(std::string(ASSET_PATH) + "lost_point.mp3", 0.6f, false);
			//send away ball in oposite
			ball->setSpeedX(ball->getSpeedX()*(-1));
			//make collision effect with the left board
			playerLeft->setLightUp(true);
		}
		//collision with wall
		else {
			//bump sound effect				
			graphics::playSound(std::string(ASSET_PATH) + "wallhit.mp3", 0.5f, false);
			//set the ball in opposite direction and boost its speed 
			ball->setSpeedX(-(ball->getSpeedX()));
			//deathBall->setSpeedY(ball->getSpeedY() * randDirection());
		}
	}

	//checking if ball collides with right wall
	if (ball->getPosX() >= CANVAS_WIDTH - PLAYER_WIDTH - SCULL_RADIUS) {
		//collision with player2
		if (ball->getPosY() <= (playerRight->getPosY() + LONG_HEIGHT / 2 + 8) && 
			ball->getPosY() >= (playerRight->getPosY() - LONG_HEIGHT / 2 - 8)) {
			//point for player 1
			score1 += 1;
			//bump sound effect
			graphics::playSound(std::string(ASSET_PATH) + "lost_point.mp3", 0.6f, false);
			//send away randomly
			ball->setSpeedX(ball->getSpeedX()*(-1));
			//make collision effect with the left board
			playerRight->setLightUp(true);
		}
		//collision with wall
		else {
			//bump sound effect				
			graphics::playSound(std::string(ASSET_PATH) + "wallhit.mp3", 0.5f, false);
			//set the ball in opposite direction and boost its speed 
			ball->setSpeedX(-(ball->getSpeedX()));
			//deathBall->setSpeedY(ball->getSpeedY() * randDirection());
		}
	}
}

void Game::updateInstructionScreen()
{
	//press CTRL to go back to menu
	if (graphics::getKeyState(graphics::SCANCODE_B)) {
		//goto instructions
		status = STATUS_MENU;
	}
}

void Game::updateStartScreen()
{
	//create mouse
	graphics::MouseState ms;
	graphics::getMouseState(ms);

	//press space OR left-click to start game
	if (graphics::getKeyState(graphics::SCANCODE_SPACE) || ms.button_left_pressed) {
		//set first random death time
		deathModeInitTime = graphics::getGlobalTime() + (rand() % 25000 + 15000);

		//for debug purposes
		if (debug_mode) {
			cout << deathModeInitTime << endl;
		}

		//start game
		status = STATUS_PLAY;
	}

	//press CTRL to goto instructions
	if (graphics::getKeyState(graphics::SCANCODE_V)) {
		//goto instructions
		status = STATUS_INSTRUCTION;
	}
}

void Game::updateLevelScreen()
{
	//players creation
	if (!playerRight_init && !playerLeft_init && graphics::getGlobalTime() > 200) {
		playerRight = new Player(*this, CANVAS_WIDTH - PLAYER_WIDTH / 2, CANVAS_HEIGHT / 2, false, false);
		playerLeft = new Player(*this, PLAYER_WIDTH / 2, CANVAS_HEIGHT / 2, true, true);
		playerRight_init = true;
		playerLeft_init = true;
	}

	//first ball creation
	if (!ball && graphics::getGlobalTime() > 200) {
		ball = new Ball(*this, BALL_SPEED, BALL_SPEED, BALL_RADIUS, false);
	}

	//players and ball updates
	if (playerRight) playerRight->update();
	if (playerLeft) playerLeft->update();
	if (ball) ball->update();
	if (ball2) ball2->update();

	//check deathmode
	checkDeathMode();

	//check collisions
	if (mode == PLAY_MODE) {
		collision();
	}
	if (mode == DEATH_MODE) {
		collisionDeathMode(ball);
		collisionDeathMode(ball2);
	}

	//when a player wins proceed to endgame
	if ((score1 >= WIN_SCORE || score2 >= WIN_SCORE) && (mode == PLAY_MODE)) {
		status = STATUS_END;
	}

	//collision color effects restoration 
	resetCollisionEffect();
}

void Game::updateEndScreen()
{
	if (graphics::getKeyState(graphics::SCANCODE_SPACE)) {
		//re-initialize scores
		score1 = 0;
		score2 = 0;

		//re-initialize players positions
		playerLeft->setPosX(PLAYER_WIDTH / 2);
		playerLeft->setPosY(CANVAS_HEIGHT / 2);
		playerRight->setPosX(CANVAS_WIDTH - PLAYER_WIDTH / 2);
		playerRight->setPosY(CANVAS_HEIGHT / 2);

		//reset deathmode time
		deathModeInitTime = graphics::getGlobalTime() + (rand() % 55000 + 40000);

		//play again
		status = STATUS_PLAY;
	}
}

void Game::drawInstructionScreen()
{
	//create brush
	graphics::Brush br;

	//set start-screen background
	br.outline_opacity = 0.0f;
	br.texture = std::string(ASSET_PATH) + "bg.png";
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

	//set font colors
	br.outline_opacity = 0.0f;
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 0.0f;
	br.fill_color[2] = 0.0f;
	br.texture = "";
	br.fill_secondary_color[0] = 1.0f;
	br.fill_secondary_color[1] = 0.4f;
	br.fill_secondary_color[2] = 0.4f;
	br.gradient = true;
	br.gradient_dir_u = 1.0f;
	br.gradient_dir_v = 0.0f;

	//start-screen text info
	char info[80];
	sprintf_s(info, "PLAYER 1  moveUp - W / moveDown - S ");
	graphics::drawText(CANVAS_WIDTH / 2 - 220, 50, 20, info, br);
	char info2[80];
	sprintf_s(info2, "PLAYER 2  moveUp - O / moveDown - L ");
	graphics::drawText(CANVAS_WIDTH / 2 - 220, 100, 20, info2, br);
	char info3[80];
	sprintf_s(info3, "First Player to reach 7 wins the game");
	graphics::drawText(CANVAS_WIDTH / 2 - 220, 150, 20, info3, br);
	char info4[80];
	sprintf_s(info4, "Beware of the sudden DEATH ROUNDS");
	graphics::drawText(CANVAS_WIDTH / 2 - 220, 200, 20, info4, br);
	char info5[80];
	sprintf_s(info5, "Avoid the ball !!");
	graphics::drawText(CANVAS_WIDTH / 2 - 200, 250, 20, info5, br);
	char info6[80];
	sprintf_s(info6, "If you reach 7 the game doesn't end");
	graphics::drawText(CANVAS_WIDTH / 2 - 200, 300, 20, info6, br);
	char info7[80];
	sprintf_s(info7, "The player with the highest score wins");
	graphics::drawText(CANVAS_WIDTH / 2 - 200, 350, 20, info7, br);
	char info8[80];
	sprintf_s(info8, "Press CTRL for instructions");
	graphics::drawText(35, CANVAS_HEIGHT - 40, 20, info8, br);
}

void Game::drawStartScreen()
{
	//create brush
	graphics::Brush br;

	//set start-screen background
	br.outline_opacity = 0.0f;
	br.texture = std::string(ASSET_PATH) + "bg.png";
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

	//set font colors
	br.outline_opacity = 0.0f;
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 0.0f;
	br.fill_color[2] = 0.0f;
	br.texture = "";
	br.fill_secondary_color[0] = 1.0f;
	br.fill_secondary_color[1] = 0.4f;
	br.fill_secondary_color[2] = 0.4f;
	br.gradient = true;
	br.gradient_dir_u = 1.0f;
	br.gradient_dir_v = 0.0f;

	//start-screen text info
	char info[40];
	sprintf_s(info, "THE DESERT PONG");
	graphics::drawText(CANVAS_WIDTH / 2 - 265, 150, 70, info, br);
	char info3[80];
	sprintf_s(info3, "Press CTRL for instructions");
	graphics::drawText(35, CANVAS_HEIGHT - 40, 20, info3, br);

	//start-screen glowing text info
	float glow = 0.5f + 0.5f * sinf(graphics::getGlobalTime() / 10);
	br.outline_opacity = 0.0f;
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 1.0f*glow;
	br.fill_color[2] = 0.0f;
	br.texture = "";
	char info2[70];
	sprintf_s(info2, "Press SPACE or LEFT-CLICK to start the game...");
	graphics::drawText(CANVAS_WIDTH / 2 - 230, 240, 20, info2, br);
}

void Game::drawLevelScreen()
{
	graphics::Brush br;
	br.outline_opacity = 0.0f;

	if (mode == PLAY_MODE) {
		//main background
		float offset = CANVAS_HEIGHT / 2 * sinf(graphics::getGlobalTime() / BG_SPEED);
		br.texture = std::string(ASSET_PATH) + "bgmain11.png";
		graphics::drawRect(CANVAS_WIDTH / 2, offset, CANVAS_WIDTH, CANVAS_HEIGHT, br);
		br.texture = std::string(ASSET_PATH) + "bgmain22.png";
		graphics::drawRect(CANVAS_WIDTH / 2, offset + CANVAS_HEIGHT, CANVAS_WIDTH, CANVAS_HEIGHT, br);
	}

	if (mode == DEATH_MODE) {
		//death mode background
		float offset = CANVAS_HEIGHT / 2 * sinf(graphics::getGlobalTime() / BG_SPEED * 4);
		br.texture = std::string(ASSET_PATH) + "death1.png";
		graphics::drawRect(CANVAS_WIDTH / 2, offset, CANVAS_WIDTH, CANVAS_HEIGHT, br);
		br.texture = std::string(ASSET_PATH) + "death2.png";
		graphics::drawRect(CANVAS_WIDTH / 2, offset + CANVAS_HEIGHT, CANVAS_WIDTH, CANVAS_HEIGHT, br);

		//text color
		br.outline_opacity = 0.0f;
		br.fill_color[0] = 1.0f;
		br.fill_color[1] = 0.0f;
		br.fill_color[2] = 0.0f;
		br.texture = "";
		br.fill_secondary_color[0] = 1.0f;
		br.fill_secondary_color[1] = 0.4f;
		br.fill_secondary_color[2] = 0.4f;
		br.gradient = true;
		br.gradient_dir_u = 1.0f;
		br.gradient_dir_v = 0.0f;

		//death mode text
		char info[40];
		sprintf_s(info, "DEATH  MODE");
		graphics::drawText(CANVAS_WIDTH / 2 - 100, 60, 45, info, br);

		//glowing color 
		float glow = 0.5f + 0.5f * sinf(graphics::getGlobalTime() / 10);
		br.outline_opacity = 0.0f;
		br.fill_color[0] = 1.0f;
		br.fill_color[1] = 1.0f*glow;
		br.fill_color[2] = 0.0f;
		br.texture = "";

		//beware text
		char info2[70];
		sprintf_s(info2, "don't let the ball touch you !!!");
		graphics::drawText(CANVAS_WIDTH / 2 - 100, CANVAS_HEIGHT - 40, 20, info2, br);
	}

	//players score color
	br.outline_opacity = 0.0f;
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 0.0f;
	br.fill_color[2] = 0.0f;
	br.texture = "";
	br.fill_secondary_color[0] = 1.0f;
	br.fill_secondary_color[1] = 0.4f;
	br.fill_secondary_color[2] = 0.4f;
	br.gradient = true;
	br.gradient_dir_u = 1.0f;
	br.gradient_dir_v = 0.0f;

	//draw player1 score 
	char playerScore1[40];
	sprintf_s(playerScore1, "%6.1f", (float)getScore1());
	graphics::drawText(CANVAS_WIDTH / 4, 50, 40, playerScore1, br);

	//draw player2 score
	char playerScore2[40];
	sprintf_s(playerScore2, "%6.1f", (float)getScore2());
	graphics::drawText(CANVAS_WIDTH * 3 / 4, 50, 40, playerScore2, br);

	//draw ball
	if (ball) ball->draw();
	if (ball2) ball2->draw();

	//draw players
	if (playerRight) playerRight->draw();
	if (playerLeft) playerLeft->draw();

	//players and ball positions for debug purposes
	if (playerRight && debug_mode) {
		char info[40];
		sprintf_s(info, "(%6.1f, %6.1f)", playerRight->getPosX(), playerRight->getPosY());
		graphics::drawText(30, 30, 30, info, br);
	}
	if (playerLeft && debug_mode) {
		char info[40];
		sprintf_s(info, "(%6.1f, %6.1f)", playerLeft->getPosX(), playerLeft->getPosY());
		graphics::drawText(30, 70, 30, info, br);
	}
	if (ball && debug_mode) {
		char info[40];
		sprintf_s(info, "(%6.1f, %6.1f)", ball->getPosX(), ball->getPosY());
		graphics::drawText(30, 100, 30, info, br);
	}
}

void Game::drawEndScreen()
{
	graphics::Brush br;
	br.outline_opacity = 0.0f;

	//background
	br.texture = std::string(ASSET_PATH) + "bg.png";
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

	//font colors
	br.outline_opacity = 0.0f;
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 0.0f;
	br.fill_color[2] = 0.0f;
	br.texture = "";
	br.fill_secondary_color[0] = 1.0f;
	br.fill_secondary_color[1] = 0.3f;
	br.fill_secondary_color[2] = 0.3f;
	br.gradient = true;
	br.gradient_dir_u = 1.0f;
	br.gradient_dir_v = 0.0f;

	//who wins text 
	if (score1 > score2) {
		char info[40];
		sprintf_s(info, "PLAYER 1 WINS!!!");
		graphics::drawText(CANVAS_WIDTH / 2 - 265, 150, 70, info, br);
	}
	if ((score2 > score1)) {
		char info[40];
		sprintf_s(info, "PLAYER 2 WINS!!!");
		graphics::drawText(CANVAS_WIDTH / 2 - 265, 150, 70, info, br);
	}

	//play again glowing color 
	float glow = 0.5f + 0.5f * sinf(graphics::getGlobalTime() / 10);
	br.outline_opacity = 0.0f;
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 1.0f * glow;
	br.fill_color[2] = 0.0f;
	br.texture = "";

	//play again text
	char info2[70];
	sprintf_s(info2, "Press SPACE to play again...");
	graphics::drawText(CANVAS_WIDTH / 2 - 230, 240, 20, info2, br);
}

void Game::resetBall()
{
	//initialize ball
	ball->init();

	//set random directions (upleft/upRight/downLeft/downRight)
	ball->setSpeedX(BALL_SPEED * randDirection());
	ball->setSpeedY(BALL_SPEED * randDirection());

	//reset players speed
	playerLeft->setSpeed(PLAYER_SPEED);
	playerRight->setSpeed(PLAYER_SPEED);
}

void Game::resetCollisionEffect()
{
	//the collision color effects duration and restoration to initial state
	if ((ball->getPosX() > PLAYER_WIDTH + 60) && (playerLeft->getLightUp() == true)) {
		playerLeft->setLightUp(false);
	}
	if ((ball->getPosX() < CANVAS_WIDTH - 60) && (playerRight->getLightUp() == true)) {
		playerRight->setLightUp(false);
	}
}

void Game::checkDeathMode() 
{
	/*if (graphics::getGlobalTime() > deathModeInitTime && graphics::getGlobalTime() < deathModeInitTime + DEATH_MODE_DURATION) {
	}
	if (graphics::getGlobalTime() >= deathModeInitTime + DEATH_MODE_DURATION) {
	}*/

	//death mode condition
	if (score1 + score2 == WIN_SCORE / 2) {
		//players longModes
		playerRight->setLongMode(true);
		playerLeft->setLongMode(true);
		
		mode = DEATH_MODE;
	}

	if (score1 + score2 == (WIN_SCORE / 2) + 3) {
		//players longModes
		playerRight->setLongMode(false);
		playerLeft->setLongMode(false);
		
		mode = PLAY_MODE;
	}

	//what changes happen at death mode
	if ((mode == PLAY_MODE) && (!deathMode)) {

		//delete previus balls
		delete ball; 
		delete ball2;
		ball = nullptr; 
		ball2 = nullptr;
		//create play_mode ball
		ball = new Ball(*this, BALL_SPEED, BALL_SPEED, BALL_RADIUS, false);

		//change to main music
		graphics::stopMusic();
		graphics::playMusic(std::string(ASSET_PATH) + "gardenia.mp3", 0.5f, false, 0);
		//
		deathMode = true;
	}
	if ((mode == DEATH_MODE) && (deathMode)) {

		//delete previus ball
		delete ball;
		ball = nullptr;
		//create death_mode one
		ball = new Ball(*this, SCULL_SPEED, SCULL_SPEED, SCULL_RADIUS, true);
		ball2 = new Ball(*this, -SCULL_SPEED, -SCULL_SPEED, BALL_RADIUS, true);

		//change to deathMode music
		graphics::stopMusic();
		graphics::playMusic(std::string(ASSET_PATH) + "bloodbath.mp3", 0.5f, false, 0);
		//
		deathMode = false;
	}
}

void Game::update()
{
	if (status == STATUS_MENU) {
		updateStartScreen();
	}
	else if (status == STATUS_INSTRUCTION) {
		updateInstructionScreen();
	}
	else if (status == STATUS_PLAY) {
		updateLevelScreen();
	}
	else {
		updateEndScreen();
	}
}

void Game::draw()
{
	if (status == STATUS_MENU) {
		drawStartScreen();
	}
	else if (status == STATUS_INSTRUCTION) {
		drawInstructionScreen();
	}
	else if (status == STATUS_PLAY) {
		drawLevelScreen();
	}
	else {
		drawEndScreen();
	}
};

void Game::init()
{
	//set game fonts 
	graphics::setFont(std::string(ASSET_PATH) + "BlackBrutal.otf");

	//set intro music
	graphics::playMusic(std::string(ASSET_PATH) + "intro.mp3", 0.5f, false, 0);
}

Game::Game()
{
}

Game::~Game()
{
	if (playerRight) {
		delete playerRight;
	}
	if (playerLeft) {
		delete playerLeft;
	}
	if (ball) {
		delete ball;
	}
}