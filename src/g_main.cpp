#include "g_main.hpp"

constexpr int WIDTH = 640;
constexpr int HEIGHT = 480;

int main(int, char**) {
	QuakeLogger::initLogger();
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Error intialising SDL: " << SDL_GetError() << std::endl;
		return 1;
	}
	spdlog::info("SDl2 initialised");

	Engine* engine = new Engine("test", false, WIDTH, HEIGHT);
	spdlog::info("Engine instantiated with {0}x{1} window", WIDTH, HEIGHT);

	std::string mapName = "q3dm7.bsp";

	QMapQ3 map;
	bool status = readMap("E:\\Quake Id Tech 2\\models\\" + mapName, map);
	if (!status) {
		return 1;
	}
	spdlog::debug("Imported {0} map", mapName);

	FILE* debugLogFile = fopen("debugLog.txt", "w");
	debugInfo(debugLogFile, map);
	fclose(debugLogFile);

	freeMap(map);

	engine->eventLoop();

	SDL_Quit();

	return 0;
}
