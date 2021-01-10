#include "player.h"
#include "graphics.h"
#include "game.h"

Player::Player(const Game & mygame, float x, float y, bool b, bool b2)
	: GameObject(mygame)
{               
	posX = x;   
	posY = y;   
	leftPosition = b;   
	leftControl = b2;
}

void Player::update()
{
	//control Left player
	if (leftPosition) {
		if (graphics::getKeyState(graphics::SCANCODE_W)) {
			posY -= speed * graphics::getDeltaTime() / 10.0f;
		}
		if (graphics::getKeyState(graphics::SCANCODE_S)) {
			posY += speed * graphics::getDeltaTime() / 10.0f;
		}
	}

	//control Right player
	if (!leftPosition) {
		if (graphics::getKeyState(graphics::SCANCODE_O)) {
			posY -= speed * graphics::getDeltaTime() / 10.0f;
		}
		if (graphics::getKeyState(graphics::SCANCODE_L)) {
			posY += speed * graphics::getDeltaTime() / 10.0f;
		}
	}

	//limits
	if (posY < playerHeight / 2) posY = playerHeight / 2;
	if (posY > CANVAS_HEIGHT - playerHeight / 2) posY = CANVAS_HEIGHT - playerHeight / 2;
}

void Player::draw()
{
	//our brush & glow
	graphics::Brush br;
	float glow = 0.5f + 0.5f * sinf(graphics::getGlobalTime() / 10);

	//normal colors
	if (!lightUp) {
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
	}

	//collision effect state
	if (lightUp) {
		br.outline_opacity = 0.0f;
		br.fill_color[0] = 1.0f;
		br.fill_color[1] = 1.0f*glow;
		br.fill_color[2] = 0.0f;
		br.texture = "";
	}

	//board length
	if (!longMode) { playerHeight = PLAYER_HEIGHT; }
	if (longMode){ playerHeight = LONG_HEIGHT; }

	//draw player
	graphics::drawRect(posX, posY, PLAYER_WIDTH, playerHeight, br);
}

void Player::init()
{
}