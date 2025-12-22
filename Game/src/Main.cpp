
#include "Utils/Log.h"
#include "Game.h"

int main(int argc, char** argv) {
	Log::Init();

	Game game;
	game.Run();

	return 0;
}
