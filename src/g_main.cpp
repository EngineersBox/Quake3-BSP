#include "g_main.h"
#include <SDL2/SDL.h>
#include <filesystem>
#include <cstdio>

#include "resource/loader/QLoader.h"

using std::filesystem::current_path;

int main(int, char**) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Error intializing SDL: " << SDL_GetError() << std::endl;
		return 1;
	}
	std::cout << "SDL successfully initialized" << std::endl;
	TMapQ3 map;
	bool status = readMap("E:\\Quake Id Tech 2\\models\\final3.bsp", map);
	if (!status) {
		return 1;
	}
	FILE* debugLogFile = fopen("debugLog.txt", "w");
	debugInfo(debugLogFile, map);
	fclose(debugLogFile);
	SDL_Quit();
	return 0;
}
