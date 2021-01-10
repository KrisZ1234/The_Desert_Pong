#include "graphics.h"
#include "game.h"
#include "config.h"

// The custom callback function that the library calls 
// to check for and set the current application state.
void update(float ms)
{
	//void pointer to Game pointer - reinterfsdf cast
	Game * game = reinterpret_cast<Game *>(graphics::getUserData());

	//calling the update in the game
	game->update();
}

void draw()
{
	//void pointer to Game pointer
	Game * game = reinterpret_cast<Game *>(graphics::getUserData());

	//calling the draw in the game
	game->draw();
}

int main()
{
	Game mygame;

	graphics::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "The Desert Pong");

	graphics::setUserData(&mygame);

	graphics::setDrawFunction(draw);
	graphics::setUpdateFunction(update);

	graphics::setCanvasSize(CANVAS_WIDTH, CANVAS_HEIGHT);
	graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);

	mygame.init();
	mygame.setDebugMode(false);
	graphics::startMessageLoop();

	return 0;
}