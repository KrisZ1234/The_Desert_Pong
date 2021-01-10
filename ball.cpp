#include "ball.h"
#include "graphics.h"
#include "config.h"
#include "util.h"
#include "game.h"
#include <random>

Ball::Ball(const Game& mygame, float sx, float sy, float r, bool s)
	: GameObject(mygame)
{
	speedX = sx;
	speedY = sy;
	radius = r;
	skullMode = s;
	init();
}

void Ball::update()
{
	//initial speed/direction
	posX += speedX;
	posY += speedY;

	//collisions
	wallCollisionY();
}

void Ball::draw()
{
	graphics::Brush brush;
	//ball color,glow and shape
	float glow = 0.5f + 0.5f * sinf(graphics::getGlobalTime() / 10);
	brush.outline_opacity = 0.0f;


	//skull mode colors
	if (skullMode) {
		brush.texture = std::string(ASSET_PATH) + "charizard.png";
		//brush.fill_secondary_opacity = 0.0f;
		//graphics::setOrientation(90.f);
		//graphics::setScale(1, 1);
		graphics::drawRect(posX, posY, 75, 75, brush);
		//graphics::drawDisk(posX, posY, radius + EXTRA_RADIUS, br);
	}
	if (!skullMode) {
		brush.fill_color[0] = 1.0f;
		brush.fill_color[1] = 1.0f*glow;
		brush.fill_color[2] = 0.0f;
		brush.texture = "";
		//draw ball
		graphics::drawDisk(posX, posY, radius, brush);
	}
}

void Ball::wallCollisionY()
{
	//collision with upper and lower wall
	if (posY + radius > CANVAS_HEIGHT || posY - radius < 0) {
		//sound effect
		graphics::playSound(std::string(ASSET_PATH) + "wallhit.mp3", 0.6f, false);
		//change direction
		speedY = -speedY;
	}
}
void Ball::init()
{
	//Game-start ball position
	posX = CANVAS_WIDTH / 2;
	posY = CANVAS_HEIGHT / 2;
}

Ball::~Ball()
{
}

