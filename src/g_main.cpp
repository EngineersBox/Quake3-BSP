#include "g_main.hpp"

int main(int, char**) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Error intializing SDL: " << SDL_GetError() << std::endl;
		return 1;
	}

	Engine* engine = new Engine("test", false, 640, 480);

	QMapQ3 map;
	bool status = readMap("E:\\Quake Id Tech 2\\models\\q3dm7.bsp", map);
	if (!status) {
		return 1;
	}

	FILE* debugLogFile = fopen("debugLog.txt", "w");
	debugInfo(debugLogFile, map);
	fclose(debugLogFile);

	freeMap(map);

	engine->eventLoop();

	SDL_Quit();

	return 0;
}
