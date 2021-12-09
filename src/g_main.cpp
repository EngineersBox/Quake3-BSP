#include "g_main.h"
#include <SDL2/SDL.h>

int main(int, char**) {
	std::cout << "RUNNING LOAD STUFFS" << std::endl;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Error intializing SDL: " << SDL_GetError() << std::endl;
		return 1;
	}
	std::cout << "SDL successfully initialized" << std::endl;
	SDL_Quit();
	return 0;
}
