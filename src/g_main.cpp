#include "g_main.hpp"

constexpr int WIDTH = 1024;
constexpr int HEIGHT = 576;

int main(int, char**) {
	QuakeLogger::initLogger();
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Error intialising SDL: " << SDL_GetError() << std::endl;
		return 1;
	}
	spdlog::info("SDl2 initialised");
	spdlog::info("Uint8 size: {0}", sizeof(Uint8));

    GEngine* engine = new GEngine("test", false, WIDTH, HEIGHT);
	spdlog::info("Engine instantiated with {0}x{1} window", WIDTH, HEIGHT);

	engine->eventLoop();

	SDL_Quit();

	return 0;
}
